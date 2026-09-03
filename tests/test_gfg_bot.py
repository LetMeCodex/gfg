import sys
from pathlib import Path

if sys.platform == "win32" and hasattr(sys.stdout, "reconfigure"):
    try:
        sys.stdout.reconfigure(encoding="utf-8")
    except Exception:
        pass

# Add project root to sys.path
sys.path.insert(0, str(Path(__file__).parent.parent))

from src.gfg_client import GFGClient
from src.solver import GFGSolver


def test_potd_fetch():
    print("[1/4] Testing GFG POTD API Discovery...")
    client = GFGClient()
    potd = client.fetch_today_potd()
    assert potd.get("problem_name"), "Problem name must exist"
    assert potd.get("slug"), "Problem slug must exist"
    print(f"  [OK] Today's POTD: {potd.get('problem_name')} [{potd.get('difficulty')}]")
    print(f"       Slug: {potd.get('slug')} | Date: {potd.get('date')}")


def test_problem_details():
    print("[2/4] Testing GFG Problem Details Fetcher...")
    client = GFGClient()
    potd = client.fetch_today_potd()
    slug = potd.get("slug")
    details = client.fetch_problem_details(slug)
    assert details.get("problem_name"), "Details must include problem_name"
    assert len(details.get("cleanContent", "")) > 10, "Clean markdown description must exist"
    print(f"  [OK] Fetched details for '{slug}'. Content length: {len(details.get('cleanContent'))} chars")


def test_verdict_parser():
    print("[3/4] Testing GFG Verdict Parsing Logic...")
    client = GFGClient()
    # Correct AC
    ac = client.parse_verdict({"view_mode": "correct", "time_taken": "0.04", "memory_taken": "15MB"})
    assert ac["passed"] is True
    assert ac["view_mode"] == "correct"

    # Wrong Answer
    wa = client.parse_verdict({
        "view_mode": "wrong",
        "input": "2\n1 2\n",
        "user_output": "3",
        "expected_output": "4"
    })
    assert wa["passed"] is False
    assert "WRONG ANSWER" in wa["diagnostic"]
    print("  [OK] Verdict parsing verified for both Correct and WA.")


def test_code_cleaner():
    print("[4/4] Testing Code Extractor...")
    solver = GFGSolver()
    raw = """Here is the GFG solution:
```cpp
class Solution {
  public:
    int maxFruits(vector<int>& arr, int m) {
        return 0;
    }
};
```
Hope this helps!
"""
    clean = solver.extract_clean_code(raw)
    assert clean.startswith("class Solution")
    assert not clean.endswith("```")
    assert "Hope this helps!" not in clean
    print("  [OK] Clean code extraction verified.")


if __name__ == "__main__":
    print("=== Running GeeksforGeeks Bot Component Tests ===")
    test_potd_fetch()
    test_problem_details()
    test_verdict_parser()
    test_code_cleaner()
    print("\n[SUCCESS] All 4 GFG test suites PASSED successfully!")
