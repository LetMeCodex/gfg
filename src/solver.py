import json
import logging
import re
import time
from typing import Any, Dict, List, Optional
from google import genai
from google.genai import types

from src.config import config

logger = logging.getLogger("GFGSolver")


class GFGSolver:
    CANDIDATE_MODELS = [
        "gemini-2.5-flash",
        "gemini-2.5-flash-lite",
        "gemini-3.1-flash-lite",
    ]

    def __init__(self, api_key: str = "", model_name: str = ""):
        self.api_key = api_key or config.gemini_api_key
        primary = model_name or config.gemini_model
        models = [primary] if primary else []
        for m in self.CANDIDATE_MODELS:
            if m not in models:
                models.append(m)
        self.candidate_models = models

        if not self.api_key:
            self.client = None
        else:
            self.client = genai.Client(api_key=self.api_key)

    def _ensure_client(self):
        if not self.client:
            raise ValueError("GEMINI_API_KEY is not set. Please provide a valid key.")

    def _generate_with_resilience(self, prompt: str) -> str:
        """Call Gemini models with fallback across multiple models and exponential backoff."""
        self._ensure_client()
        last_err = None

        for model in self.candidate_models:
            for try_idx in range(2):
                try:
                    logger.info(f"Generating GFG solution via model '{model}' (try {try_idx + 1}/2)...")
                    response = self.client.models.generate_content(
                        model=model,
                        contents=prompt,
                        config=types.GenerateContentConfig(temperature=0.2)
                    )
                    if response and response.text:
                        return response.text
                except Exception as e:
                    last_err = e
                    err_str = str(e)
                    logger.warning(f"Model '{model}' try {try_idx + 1} failed: {err_str[:120]}")
                    if "503" in err_str or "429" in err_str or "UNAVAILABLE" in err_str:
                        time.sleep(3 * (try_idx + 1))
                    else:
                        break

        raise RuntimeError(f"All Gemini candidate models failed for GFG. Last error: {last_err}")

    def extract_clean_code(self, raw_text: str) -> str:
        """Extract clean source code from markdown fences."""
        text = raw_text.strip()
        match = re.search(r"```(?:[a-zA-Z0-9_\+\-]+)?\n(.*?)```", text, re.DOTALL)
        if match:
            return match.group(1).strip()
        return text

    def generate_initial_solution(
        self, problem_details: Dict[str, Any], language: str = "cpp"
    ) -> str:
        """Generate optimal C++ / Python class Solution for GFG POTD."""
        title = problem_details.get("problem_name", "")
        difficulty = problem_details.get("difficulty", "Easy")
        content = problem_details.get("cleanContent", "")
        constraints = problem_details.get("constraints_display", "")
        input_fmt = problem_details.get("input_format", {})

        lang_instructions = ""
        if "cpp" in language.lower() or "c++" in language.lower():
            lang_instructions = """Language: C++17
- Implement the requested solution inside `class Solution { public: ... };`
- Do NOT write `int main()`. GeeksforGeeks appends an internal driver code that calls your class method.
- Use standard library types (e.g. `vector<int>`, `string`, `long long`).
- Ensure optimal time and space complexity to prevent TLE.
"""
        else:
            lang_instructions = """Language: Python 3
- Implement inside `class Solution: def methodName(self, ...):`
- Do NOT write driver code / execution script.
"""

        prompt = f"""You are a Competitive Programming Grandmaster solving today's GeeksforGeeks Problem of the Day.

Problem Title: {title}
Difficulty: {difficulty}

Problem Statement & Examples:
{content}

Constraints:
{constraints}

Input Format Specification:
{json.dumps(input_fmt, indent=2) if isinstance(input_fmt, dict) else input_fmt}

{lang_instructions}

Critical Requirements:
1. Deduce the exact function signature and method name from the problem statement, examples, and input format.
2. Return ONLY the complete, compileable `class Solution` code inside a ```{language} ... ``` code block. No conversational prose.
"""
        raw = self._generate_with_resilience(prompt)
        return self.extract_clean_code(raw)

    def generate_self_healing_solution(
        self,
        problem_details: Dict[str, Any],
        previous_code: str,
        diagnostic: str,
        attempt: int,
        language: str = "cpp"
    ) -> str:
        """Self-healing reflection prompt to debug and refactor failed GFG submission."""
        title = problem_details.get("problem_name", "")
        content = problem_details.get("cleanContent", "")

        prompt = f"""You are an Expert Algorithmist debugging a failed GeeksforGeeks POTD submission.

Problem: {title}
Description:
{content}

--------------------------------------------------
YOUR PREVIOUS ATTEMPT (Attempt #{attempt}):
```{language}
{previous_code}
```

GEEKSFORGEEKS JUDGE DIAGNOSTIC:
{diagnostic}
--------------------------------------------------

DEBUGGING INSTRUCTIONS:
1. If COMPILATION ERROR:
   - Check the exact class method name and parameter types. If the compiler error says:
     `'class Solution' has no member named 'XYZ'`, you MUST rename your function to EXACTLY `XYZ`.
2. If WRONG ANSWER (WA):
   - Check if you missed circular array wrapping (e.g. `(i + j) % n`), edge cases (m >= n, m = 1, single element), or 64-bit integer overflow (use `long long` in C++).
3. If TIME LIMIT EXCEEDED (TLE):
   - Switch to an $O(N)$ or $O(N \\log N)$ optimal technique (e.g., sliding window, prefix sums, two pointers).
4. CRITICAL FORMAT ENFORCEMENT:
   - You MUST return the ENTIRE, COMPLETE `class Solution { public: ... };` code block.
   - NEVER return just an isolated if-statement or partial code snippet.
   - Return ONLY the compileable code inside a ```{language} ... ``` block.
"""
        raw = self._generate_with_resilience(prompt)
        return self.extract_clean_code(raw)

    def generate_explanation_doc(
        self, problem_details: Dict[str, Any], code_solution: str, attempts: int, language: str = "cpp"
    ) -> str:
        """Generate markdown writeup for GitHub repo."""
        title = problem_details.get("problem_name", "")
        difficulty = problem_details.get("difficulty", "Easy")
        slug = problem_details.get("slug", "")

        prompt = f"""Generate a high-quality GitHub solution writeup for:
GeeksforGeeks POTD: [{title}](https://www.geeksforgeeks.org/problems/{slug}/1)
Difficulty: {difficulty}
Solved in: {attempts} attempt(s)

Solution Code:
```{language}
{code_solution}
```

Include:
# [{title}](https://www.geeksforgeeks.org/problems/{slug}/1)
- **Platform**: GeeksforGeeks (Problem of the Day)
- **Difficulty**: {difficulty}
- **Solved In**: {attempts} attempt(s)
## Problem Summary
## Intuition & Approach
## Complexity Analysis
- **Time Complexity**: $O(...)$
- **Space Complexity**: $O(...)$
## Solution Code
"""
        return self._generate_with_resilience(prompt).strip()
