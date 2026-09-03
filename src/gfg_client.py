import html
import json
import logging
import random
import re
import time
from typing import Any, Dict, List, Optional
import requests

from src.config import config

logger = logging.getLogger("GFGClient")


class GFGClient:
    API_BASE = "https://practiceapi.geeksforgeeks.org/api/vr"
    ORIGIN_API = "https://practiceapiorigin.geeksforgeeks.org/api/latest"
    SITE_BASE = "https://www.geeksforgeeks.org"

    PRACTICE_TOPICS = [
        "array", "string", "tree", "list", "stack", "queue",
        "matrix", "hash", "sort", "graph", "sum", "count", "max", "find"
    ]

    # Language short codes for GFG
    LANG_MAP = {
        "cpp": "cpp",
        "c++": "cpp",
        "python3": "python3",
        "python": "python3",
        "py": "python3",
        "java": "java",
    }

    def __init__(self, cookie: str = ""):
        raw_cookie = cookie or config.gfg_cookie
        self.cookie = self._sanitize_cookie_string(raw_cookie)
        self.http = requests.Session()
        self._setup_headers()

    @staticmethod
    def _sanitize_cookie_string(raw: str) -> str:
        if not raw:
            return ""
        # Remove any leading 'Cookie:' or 'cookie:' prefix
        clean = raw.strip().strip('"').strip("'")
        if clean.lower().startswith("cookie:"):
            clean = clean[7:].strip()

        # Replace all newlines, tabs, and carriage returns
        clean = re.sub(r"[\r\n\t]+", " ", clean).strip()
        # Clean around semicolons
        clean = re.sub(r"\s*;\s*", "; ", clean)
        # Keep only valid ASCII printable characters (space 32 to ~ 126)
        clean = "".join(c for c in clean if 31 < ord(c) < 127).strip()
        return clean

    def _setup_headers(self):
        headers = {
            "User-Agent": (
                "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
                "AppleWebKit/537.36 (KHTML, like Gecko) "
                "Chrome/124.0.0.0 Safari/537.36"
            ),
            "Accept": "application/json, text/plain, */*",
            "Accept-Language": "en-US,en;q=0.9",
            "Referer": f"{self.SITE_BASE}/problem-of-the-day",
            "Origin": self.SITE_BASE,
            "Connection": "keep-alive",
        }
        if self.cookie:
            if "=" in self.cookie:
                headers["Cookie"] = self.cookie
                csrf_match = re.search(r"csrftoken=([^;]+)", self.cookie)
                if csrf_match:
                    csrf_val = csrf_match.group(1).strip()
                    csrf_clean = "".join(c for c in csrf_val if 31 < ord(c) < 127).strip()
                    if csrf_clean:
                        headers["X-CSRFToken"] = csrf_clean
            else:
                headers["Cookie"] = f"sessionid={self.cookie}"

        self.http.headers.update(headers)

    def fetch_today_potd(self) -> Dict[str, Any]:
        """Fetch metadata for today's GeeksforGeeks Problem of the Day."""
        url = f"{self.API_BASE}/problems-of-day/problem/today/"
        response = self.http.get(url, timeout=15)
        response.raise_for_status()
        data = response.json()

        # Extract slug from problem_url if not directly present
        slug = data.get("slug", "")
        if not slug and data.get("problem_url"):
            match = re.search(r"/problems/([^/]+)", data["problem_url"])
            if match:
                slug = match.group(1)
        data["slug"] = slug

        return data

    def fetch_problem_details(self, slug: str) -> Dict[str, Any]:
        """Fetch detailed problem description, constraints, and I/O format."""
        url = f"{self.API_BASE}/problems/{slug}/"
        response = self.http.get(url, timeout=15)
        response.raise_for_status()
        res_data = response.json()
        results = res_data.get("results", {})
        if not results:
            raise ValueError(f"Could not retrieve problem details for slug '{slug}': {res_data}")

        # Clean problem description
        results["cleanContent"] = self._clean_html(results.get("problem_question", ""))
        return results

    def fetch_practice_problems(self, topic: str = "", limit: int = 50) -> List[Dict[str, Any]]:
        """Fetch real practice problems from GeeksforGeeks search API."""
        topics_to_try = [topic] if topic else random.sample(self.PRACTICE_TOPICS, min(3, len(self.PRACTICE_TOPICS)))
        for t in topics_to_try:
            url = f"https://practiceapi.geeksforgeeks.org/api/v1/problems/search/?query={t}&limit={limit}"
            try:
                resp = self.http.get(url, timeout=15)
                if resp.status_code == 200:
                    data = resp.json()
                    problems = data.get("problems", [])
                    if problems:
                        return problems
            except Exception as e:
                logger.warning(f"Failed to fetch GFG practice problems for topic '{t}': {e}")
        return []

    def submit_solution(self, slug: str, code: str, language: str = "cpp") -> str:
        """Submit code to GFG judge and return submission_id."""
        if not self.cookie:
            raise PermissionError("GFG_COOKIE is required to submit solutions to GeeksforGeeks.")

        lang_code = self.LANG_MAP.get(language.lower(), "cpp")
        url = f"{self.ORIGIN_API}/problems/{slug}/submit/compile/"

        payload = {
            "source": self.SITE_BASE,
            "request_type": "solutionCheck",
            "userCode": code,
            "language": lang_code,
        }

        headers = {
            "Referer": f"{self.SITE_BASE}/problems/{slug}/1",
            "Origin": self.SITE_BASE,
            "Content-Type": "application/json",
        }

        response = self.http.post(url, json=payload, headers=headers, timeout=20)
        if response.status_code != 200:
            raise RuntimeError(f"GFG submission failed [HTTP {response.status_code}]: {response.text}")

        res_data = response.json()
        results = res_data.get("results", {})
        submission_id = str(results.get("submission_id") or "")
        if not submission_id:
            raise RuntimeError(f"Unexpected submission response: {res_data}")

        return submission_id

    def poll_verdict(self, submission_id: str, pid: int, max_wait_seconds: int = 45) -> Dict[str, Any]:
        """Poll GFG evaluation status until grading completes."""
        url = f"{self.ORIGIN_API}/problems/submission/submit/result/"
        start_time = time.time()

        payload = {
            "sub_id": submission_id,
            "sub_type": "submit",
            "pid": int(pid),
        }

        headers = {
            "Content-Type": "application/json",
            "Referer": f"{self.SITE_BASE}/",
            "Origin": self.SITE_BASE,
        }
        if self.cookie:
            headers["Cookie"] = self.cookie

        while time.time() - start_time < max_wait_seconds:
            response = self.http.post(url, json=payload, headers=headers, timeout=15)
            if response.status_code == 200:
                data = response.json()
                status = data.get("status")
                view_mode = data.get("view_mode")

                # If status is SUCCESS or view_mode is final, evaluation has completed
                if status == "SUCCESS" or (view_mode and view_mode != "queuing"):
                    return self.parse_verdict(data)

            time.sleep(2.5)

        raise TimeoutError(f"GFG Submission {submission_id} timed out after {max_wait_seconds}s")

    def parse_verdict(self, raw_result: Dict[str, Any]) -> Dict[str, Any]:
        """Normalize GFG verdict object."""
        view_mode = raw_result.get("view_mode", "unknown")
        passed = (view_mode == "correct")

        time_taken = raw_result.get("time", "N/A")
        msg = raw_result.get("message", {})
        err = msg.get("error", "") if isinstance(msg, dict) else str(msg)

        diagnostic = ""
        if not passed:
            if "compil" in view_mode.lower() or err:
                diagnostic = f"Compilation Error:\n{err}"
            elif view_mode == "wrong":
                diagnostic = (
                    f"Verdict: WRONG ANSWER (WA)\n"
                    f"Input: {msg.get('input', 'N/A') if isinstance(msg, dict) else 'N/A'}\n"
                    f"Your Output: {msg.get('user_output', 'N/A') if isinstance(msg, dict) else 'N/A'}\n"
                    f"Expected Output: {msg.get('expected_output', 'N/A') if isinstance(msg, dict) else 'N/A'}"
                )
            elif view_mode == "time":
                diagnostic = f"Verdict: TIME LIMIT EXCEEDED (TLE)\nTime taken: {time_taken}s"
            else:
                diagnostic = f"Verdict: {view_mode}\nDetails: {json.dumps(raw_result, indent=2)}"

        return {
            "passed": passed,
            "view_mode": view_mode,
            "time": time_taken,
            "diagnostic": diagnostic,
            "raw": raw_result,
        }

    @staticmethod
    def _clean_html(raw_html: str) -> str:
        """Convert HTML problem description to clean markdown text."""
        if not raw_html:
            return ""
        text = html.unescape(raw_html)
        text = re.sub(r"<pre>(.*?)</pre>", r"\n```\n\1\n```\n", text, flags=re.DOTALL)
        text = re.sub(r"<code>(.*?)</code>", r"`\1`", text)
        text = re.sub(r"<strong>(.*?)</strong>", r"**\1**", text)
        text = re.sub(r"<em>(.*?)</em>", r"*\1*", text)
        text = re.sub(r"<li>(.*?)</li>", r"- \1\n", text)
        text = re.sub(r"<p[^>]*>", r"\n", text)
        text = re.sub(r"<br\s*/?>", r"\n", text)
        text = re.sub(r"<[^>]+>", "", text)
        text = re.sub(r"\n{3,}", "\n\n", text)
        return text.strip()
