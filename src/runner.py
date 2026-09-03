import datetime
import json
import logging
import os
import sys
import time
from pathlib import Path

from src.config import config
from src.gfg_client import GFGClient
from src.solver import GFGSolver

logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s [%(levelname)s] %(name)s: %(message)s",
    handlers=[logging.StreamHandler(sys.stdout)],
)
logger = logging.getLogger("GFGPOTDRunner")


def load_potd_database(data_path: Path) -> dict:
    """Load or initialize database tracking solved GFG POTD."""
    if data_path.exists():
        try:
            with open(data_path, "r", encoding="utf-8") as f:
                return json.load(f)
        except Exception as e:
            logger.warning(f"Failed to read {data_path}, creating fresh state: {e}")

    return {"total_solved": 0, "current_streak": 0, "solved_dates": [], "history": []}


def save_potd_database(data_path: Path, data: dict) -> None:
    """Persist solved POTD tracking to JSON."""
    data_path.parent.mkdir(parents=True, exist_ok=True)
    with open(data_path, "w", encoding="utf-8") as f:
        json.dump(data, f, indent=2)


def save_solution_files(
    solutions_dir: Path,
    folder_name: str,
    code: str,
    doc: str,
    language: str
) -> None:
    """Save code and explanation markdown inside solution folder."""
    target_dir = solutions_dir / folder_name
    target_dir.mkdir(parents=True, exist_ok=True)

    ext = "cpp" if "cpp" in language.lower() or "c++" in language.lower() else "py"
    code_path = target_dir / f"solution.{ext}"
    with open(code_path, "w", encoding="utf-8") as f:
        f.write(code)

    readme_path = target_dir / "README.md"
    with open(readme_path, "w", encoding="utf-8") as f:
        f.write(doc)

    logger.info(f"Saved POTD solution files at: {target_dir}")


def run():
    logger.info("=== Starting GeeksforGeeks POTD Autonomous Runner ===")

    client = GFGClient()
    solver = GFGSolver()

    # Pre-flight Credentials Verification
    if not solver.api_key:
        logger.error(
            "CRITICAL: GEMINI_API_KEY is missing! Please configure it in your environment or GitHub Secrets."
        )
        sys.exit(1)

    if not config.dry_run and not client.cookie:
        logger.error(
            "CRITICAL: GFG_COOKIE is missing! Please add your GeeksforGeeks session cookie to GitHub Secrets."
        )
        sys.exit(1)

    # 1. Fetch Today's POTD
    logger.info("Fetching today's Problem of the Day from GFG...")
    try:
        potd_meta = client.fetch_today_potd()
    except Exception as e:
        logger.error(f"Failed to fetch today's POTD: {e}")
        sys.exit(1)

    problem_name = potd_meta.get("problem_name", "Unknown Problem")
    slug = potd_meta.get("slug", "")
    difficulty = potd_meta.get("difficulty", "N/A")
    raw_date = potd_meta.get("date", str(datetime.date.today()))
    potd_date = raw_date.split(" ")[0]

    logger.info(f"Target POTD: {problem_name} [{difficulty}]")
    logger.info(f"Problem Slug: {slug} | Date: {potd_date}")
    logger.info(f"URL: {potd_meta.get('problem_url')}")

    # 2. Check Database for Existing Solution
    db = load_potd_database(config.data_file)
    solved_dates = set(db.get("solved_dates", []))

    if potd_date in solved_dates:
        logger.info(f"Today's POTD ({potd_date}) has already been solved! Streak intact.")
        return

    # 3. Fetch Problem Details
    logger.info(f"Fetching full problem description and constraints for '{slug}'...")
    try:
        details = client.fetch_problem_details(slug)
        details["slug"] = slug
    except Exception as e:
        logger.error(f"Failed to fetch problem details: {e}")
        sys.exit(1)

    pid = details.get("id") or potd_meta.get("problem_id") or 0
    folder_name = f"{potd_date}_{slug}"
    attempt = 1
    previous_code = ""
    diagnostic = ""
    is_solved = False

    while attempt <= config.max_retries:
        logger.info(f"\n--- POTD Attempt {attempt} of {config.max_retries} ---")
        try:
            if attempt == 1:
                logger.info("Generating initial class Solution using Gemini...")
                sol_code = solver.generate_initial_solution(details, config.language)
            else:
                logger.info("Triggering Self-Healing Reflection loop...")
                sol_code = solver.generate_self_healing_solution(
                    problem_details=details,
                    previous_code=previous_code,
                    diagnostic=diagnostic,
                    attempt=attempt - 1,
                    language=config.language
                )

            logger.info(f"Generated solution ({len(sol_code)} chars). Preview:\n{sol_code[:180]}...\n")

            if config.dry_run:
                logger.info("[DRY RUN] Skipping live submission.")
                doc = solver.generate_explanation_doc(details, sol_code, attempts=attempt, language=config.language)
                save_solution_files(config.solutions_dir, folder_name, sol_code, doc, config.language)
                is_solved = True
                break

            # Live Submission
            logger.info("Submitting solution to GeeksforGeeks Online Judge...")
            submission_id = client.submit_solution(slug, sol_code, config.language)
            logger.info(f"Submitted successfully! Submission ID: {submission_id}. Polling verdict...")

            verdict = client.poll_verdict(submission_id, pid=pid)
            logger.info(f"Verdict: {verdict.get('view_mode')} (Passed: {verdict.get('passed')})")

            if verdict["passed"]:
                logger.info(f"[ACCEPTED] Today's GFG POTD solved successfully!")
                is_solved = True

                doc = solver.generate_explanation_doc(details, sol_code, attempts=attempt, language=config.language)
                save_solution_files(config.solutions_dir, folder_name, sol_code, doc, config.language)

                # Update database
                db["solved_dates"].append(potd_date)
                db["total_solved"] = len(db["solved_dates"])
                db["current_streak"] = db.get("current_streak", 0) + 1
                db["history"].append({
                    "date": potd_date,
                    "problem_name": problem_name,
                    "slug": slug,
                    "difficulty": difficulty,
                    "attempts": attempt,
                    "time": time.strftime("%Y-%m-%d %H:%M:%S")
                })
                save_potd_database(config.data_file, db)
                logger.info(f"Streak Updated: {db['current_streak']} days! GeekBit added! 🎉")
                break
            else:
                logger.warning(f"Verdict failed: {verdict.get('view_mode')}")
                diagnostic = verdict.get("diagnostic", "Submission failed.")
                previous_code = sol_code
                if attempt < config.max_retries:
                    logger.info("Backing off 12s before next self-healing attempt...")
                    time.sleep(12)

        except Exception as loop_err:
            logger.error(f"Error during attempt {attempt}: {loop_err}")
            diagnostic = f"Execution error: {str(loop_err)}"
            previous_code = sol_code if 'sol_code' in locals() else ""
            time.sleep(12)

        attempt += 1

    if not is_solved and not config.dry_run:
        logger.error(f"Failed to solve today's POTD after {config.max_retries} attempts.")
        sys.exit(1)


if __name__ == "__main__":
    run()
