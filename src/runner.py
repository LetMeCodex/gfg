import datetime
import json
import logging
import os
import random
import sys
import time
from pathlib import Path
from typing import Any, Dict, Optional

from src.config import config
from src.gfg_client import GFGClient
from src.solver import GFGSolver

logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s [%(levelname)s] %(name)s: %(message)s",
    handlers=[logging.StreamHandler(sys.stdout)],
)
logger = logging.getLogger("GFGPOTDRunner")


def load_json_db(data_path: Path, default_structure: dict) -> dict:
    """Load or initialize database tracking solved GFG problems."""
    if data_path.exists():
        try:
            with open(data_path, "r", encoding="utf-8") as f:
                return json.load(f)
        except Exception as e:
            logger.warning(f"Failed to read {data_path}, creating fresh state: {e}")
    return default_structure


def save_json_db(data_path: Path, data: dict) -> None:
    """Persist database to JSON."""
    data_path.parent.mkdir(parents=True, exist_ok=True)
    with open(data_path, "w", encoding="utf-8") as f:
        json.dump(data, f, indent=2)


def count_practice_solved_today(db: dict) -> int:
    """Count practice problems solved today."""
    today_str = datetime.date.today().isoformat()
    return sum(1 for item in db.get("history", []) if item.get("date") == today_str)


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


def solve_single_gfg_problem(
    slug: str,
    details: Dict[str, Any],
    client: GFGClient,
    solver: GFGSolver,
    is_potd: bool = False
) -> bool:
    """Execute autonomous self-healing solve loop for a GFG problem."""
    problem_name = details.get("problem_name", slug)
    difficulty = details.get("difficulty", "Easy")
    pid = details.get("id") or 0
    today_str = datetime.date.today().isoformat()

    tag = "[POTD]" if is_potd else "[PRACTICE]"
    logger.info(f"{tag} Target: '{problem_name}' [{difficulty}] (PID: {pid})")

    folder_name = f"{today_str}_{slug}" if is_potd else f"practice_{slug}"

    attempt = 1
    previous_code = ""
    diagnostic = ""

    while attempt <= config.max_retries:
        logger.info(f"  Attempt {attempt} of {config.max_retries}...")
        try:
            if attempt == 1:
                sol_code = solver.generate_initial_solution(details, config.language)
            else:
                sol_code = solver.generate_self_healing_solution(
                    problem_details=details,
                    previous_code=previous_code,
                    diagnostic=diagnostic,
                    attempt=attempt - 1,
                    language=config.language
                )

            if config.dry_run:
                logger.info("  [DRY RUN] Skipping live submission.")
                doc = solver.generate_explanation_doc(details, sol_code, attempts=attempt, language=config.language)
                save_solution_files(config.solutions_dir, folder_name, sol_code, doc, config.language)
                return True

            submission_id = client.submit_solution(slug, sol_code, config.language)
            logger.info(f"  Submitted! ID: {submission_id}. Polling verdict...")

            verdict = client.poll_verdict(submission_id, pid=pid)
            logger.info(f"  Verdict: {verdict.get('view_mode')} (Passed: {verdict.get('passed')})")

            if verdict["passed"]:
                logger.info(f"  [ACCEPTED] Problem solved successfully!")
                doc = solver.generate_explanation_doc(details, sol_code, attempts=attempt, language=config.language)
                save_solution_files(config.solutions_dir, folder_name, sol_code, doc, config.language)
                return True
            else:
                logger.warning(f"  Verdict failed: {verdict.get('view_mode')}")
                diagnostic = verdict.get("diagnostic", "Submission failed.")
                previous_code = sol_code
                if attempt < config.max_retries:
                    time.sleep(12)

        except Exception as loop_err:
            logger.error(f"  Error during attempt {attempt}: {loop_err}")
            diagnostic = f"Execution error: {str(loop_err)}"
            previous_code = sol_code if 'sol_code' in locals() else ""
            time.sleep(12)

        attempt += 1

    logger.warning(f"  [FAILED] Could not solve '{problem_name}' after {config.max_retries} attempts.")
    return False


def run():
    logger.info("=== Starting GeeksforGeeks POTD & Practice Grinder ===")

    client = GFGClient()
    solver = GFGSolver()

    if not solver.api_key:
        logger.error("CRITICAL: GEMINI_API_KEY is missing!")
        sys.exit(1)

    if not config.dry_run and not client.cookie:
        logger.error("CRITICAL: GFG_COOKIE is missing!")
        sys.exit(1)

    # 1. Phase 1: Daily POTD Check (Streak & GeekBits)
    logger.info("\n--- Phase 1: Daily POTD Check ---")
    potd_db = load_json_db(config.data_file, {"total_solved": 0, "current_streak": 0, "solved_dates": [], "history": []})
    solved_dates = set(potd_db.get("solved_dates", []))

    try:
        potd_meta = client.fetch_today_potd()
        problem_name = potd_meta.get("problem_name", "Unknown")
        slug = potd_meta.get("slug", "")
        difficulty = potd_meta.get("difficulty", "N/A")
        raw_date = potd_meta.get("date", str(datetime.date.today()))
        potd_date = raw_date.split(" ")[0]

        if potd_date in solved_dates:
            logger.info(f"Today's POTD ({potd_date}) is already solved! Streak intact.")
        else:
            logger.info(f"Solving today's POTD: '{problem_name}'...")
            details = client.fetch_problem_details(slug)
            details["slug"] = slug
            passed = solve_single_gfg_problem(slug, details, client, solver, is_potd=True)

            if passed:
                potd_db["solved_dates"].append(potd_date)
                potd_db["total_solved"] = len(potd_db["solved_dates"])
                potd_db["current_streak"] = potd_db.get("current_streak", 0) + 1
                potd_db["history"].append({
                    "date": potd_date,
                    "problem_name": problem_name,
                    "slug": slug,
                    "difficulty": difficulty,
                    "time": time.strftime("%Y-%m-%d %H:%M:%S")
                })
                save_json_db(config.data_file, potd_db)
                logger.info(f"Streak Updated: {potd_db['current_streak']} days! GeekBit added! 🪙")
                delay = random.randint(config.min_delay_seconds, config.max_delay_seconds)
                logger.info(f"POTD finished. Resting {delay}s before practice grind...")
                time.sleep(delay)
    except Exception as e:
        logger.warning(f"Error during POTD check: {e}")

    # 2. Phase 2: Practice Archive Grinder
    if not config.enable_practice_grind:
        logger.info("Practice grind is disabled in config.")
        return

    logger.info("\n--- Phase 2: GFG Practice Archive Grinder ---")
    practice_db = load_json_db(config.practice_data_file, {"total_solved": 0, "solved_slugs": [], "history": []})
    solved_practice_slugs = set(practice_db.get("solved_slugs", []))
    today_str = datetime.date.today().isoformat()

    solved_today = count_practice_solved_today(practice_db)
    date_seed = int(today_str.replace("-", ""))
    rng = random.Random(date_seed)
    daily_quota = rng.randint(config.daily_target_min, config.daily_target_max)

    logger.info(f"Practice solved today: {solved_today}/{daily_quota} problems.")
    if solved_today >= daily_quota:
        logger.info(f"Daily practice quota of {daily_quota} reached! Exiting.")
        return

    batch_size = random.randint(config.min_problems_per_run, config.max_problems_per_run)
    remaining_quota = daily_quota - solved_today
    target_count = min(batch_size, remaining_quota)
    logger.info(f"Targeting {target_count} practice problems for this run slot.")

    candidates = client.fetch_practice_problems(limit=60)
    unsolved = [c for c in candidates if c.get("slug") and c["slug"] not in solved_practice_slugs]

    if not unsolved:
        logger.info("No unsolved practice candidates found in fetched batch.")
        return

    random.shuffle(unsolved)
    solved_in_this_run = 0

    for cand in unsolved:
        if solved_in_this_run >= target_count:
            break

        cand_slug = cand["slug"]
        logger.info(f"\n[Practice {solved_in_this_run + 1}/{target_count}] Fetching details for: {cand.get('problem_name')}")
        try:
            full_details = client.fetch_problem_details(cand_slug)
            full_details["slug"] = cand_slug
            passed = solve_single_gfg_problem(cand_slug, full_details, client, solver, is_potd=False)

            if passed:
                practice_db["solved_slugs"].append(cand_slug)
                practice_db["total_solved"] = len(practice_db["solved_slugs"])
                practice_db["history"].append({
                    "date": today_str,
                    "problem_name": full_details.get("problem_name"),
                    "slug": cand_slug,
                    "difficulty": full_details.get("difficulty"),
                    "time": time.strftime("%Y-%m-%d %H:%M:%S")
                })
                save_json_db(config.practice_data_file, practice_db)
                solved_practice_slugs.add(cand_slug)
                solved_in_this_run += 1

                if solved_in_this_run < target_count:
                    delay = random.randint(config.min_delay_seconds, config.max_delay_seconds)
                    logger.info(f"Human pacing delay: resting {delay}s before next problem...")
                    time.sleep(delay)
        except Exception as err:
            logger.warning(f"Skipping practice problem {cand_slug} due to error: {err}")
            continue

    logger.info(f"\n=== GFG Run Finished: Solved {solved_in_this_run} practice problems ===")
    logger.info(f"Total Practice Solved to Date: {practice_db['total_solved']}")


if __name__ == "__main__":
    run()
