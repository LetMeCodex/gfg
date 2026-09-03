import html
import json
import logging
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
        self.cookie = raw_cookie.strip().strip('"').strip("'") if raw_cookie else ""
        self.http = requests.Session()
        self._setup_headers()

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
            headers["Cookie"] = self.cookie

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

    def poll_verdict(self, submission_id: str, max_wait_seconds: int = 45) -> Dict[str, Any]:
        """Poll GFG evaluation status until grading completes."""
        url = f"{self.ORIGIN_API}/problems/submission/submit/result/"
        start_time = time.time()

        payload = {
            "subId": submission_id,
            "reqType": "solutionCheck",
        }

        headers = {
            "Content-Type": "application/json",
            "Referer": f"{self.SITE_BASE}/",
            "Origin": self.SITE_BASE,
        }

        while time.time() - start_time < max_wait_seconds:
            response = self.http.post(url, json=payload, headers=headers, timeout=15)
            if response.status_code == 200:
                data = response.json()
                results = data.get("results", {}) or data.get("response", {})
                status = results.get("status")
                view_mode = results.get("view_mode")

                # If status is SUCCESS or calculated, grading has finished
                if status in ("SUCCESS", "calculated") or (view_mode and view_mode != "queuing"):
                    return self.parse_verdict(results)

            time.sleep(2.5)

        raise TimeoutError(f"GFG Submission {submission_id} timed out after {max_wait_seconds}s")

    def parse_verdict(self, raw_result: Dict[str, Any]) -> Dict[str, Any]:
        """Normalize GFG verdict object."""
        view_mode = raw_result.get("view_mode", "unknown")
        passed = (view_mode == "correct")

        time_taken = raw_result.get("time_taken", "N/A")
        memory_taken = raw_result.get("memory_taken", "N/A")

        diagnostic = ""
        if not passed:
            if view_mode == "wrong":
                diagnostic = (
                    f"Verdict: WRONG ANSWER (WA)\n"
                    f"Input: {raw_result.get('input', 'N/A')}\n"
                    f"Your Output: {raw_result.get('user_output', 'N/A')}\n"
                    f"Expected Output: {raw_result.get('expected_output', 'N/A')}"
                )
            elif view_mode == "time":
                diagnostic = f"Verdict: TIME LIMIT EXCEEDED (TLE)\nTime taken: {time_taken}s"
            elif view_mode == "compilation_error":
                diagnostic = f"Verdict: COMPILATION ERROR\nError Log:\n{raw_result.get('compilation_error', '')}"
            else:
                diagnostic = f"Verdict: {view_mode}\nDetails: {json.dumps(raw_result, indent=2)}"

        return {
            "passed": passed,
            "view_mode": view_mode,
            "time": time_taken,
            "memory": memory_taken,
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
