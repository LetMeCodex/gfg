import os
from pathlib import Path
from pydantic import BaseModel, Field


class BotConfig(BaseModel):
    # API & Session Credentials
    gemini_api_key: str = Field(default_factory=lambda: os.getenv("GEMINI_API_KEY", ""))
    gemini_model: str = Field(default_factory=lambda: os.getenv("GEMINI_MODEL", "gemini-2.5-flash"))
    gfg_cookie: str = Field(default_factory=lambda: os.getenv("GFG_COOKIE", ""))

    # Execution & Language Settings
    language: str = Field(default_factory=lambda: os.getenv("LANGUAGE", "cpp"))  # "cpp" or "python3"
    max_retries: int = Field(default_factory=lambda: int(os.getenv("MAX_RETRIES", "4")))

    # Practice Grinder & Randomization
    enable_practice_grind: bool = Field(default_factory=lambda: os.getenv("ENABLE_PRACTICE_GRIND", "true").lower() in ("true", "1", "yes"))
    daily_target_min: int = Field(default_factory=lambda: int(os.getenv("DAILY_TARGET_MIN", "6")))
    daily_target_max: int = Field(default_factory=lambda: int(os.getenv("DAILY_TARGET_MAX", "14")))
    min_problems_per_run: int = Field(default_factory=lambda: int(os.getenv("MIN_PROBLEMS_PER_RUN", "2")))
    max_problems_per_run: int = Field(default_factory=lambda: int(os.getenv("MAX_PROBLEMS_PER_RUN", "4")))
    min_delay_seconds: int = Field(default_factory=lambda: int(os.getenv("MIN_DELAY_SECONDS", "40")))
    max_delay_seconds: int = Field(default_factory=lambda: int(os.getenv("MAX_DELAY_SECONDS", "95")))

    # File Paths
    data_file: Path = Field(default_factory=lambda: Path(os.getenv("DATA_FILE", "data/solved_potd.json")))
    practice_data_file: Path = Field(default_factory=lambda: Path(os.getenv("PRACTICE_DATA_FILE", "data/solved_practice.json")))
    solutions_dir: Path = Field(default_factory=lambda: Path(os.getenv("SOLUTIONS_DIR", "solutions")))

    # Dry Run Mode
    dry_run: bool = Field(default_factory=lambda: os.getenv("DRY_RUN", "false").lower() in ("true", "1", "yes"))


config = BotConfig()
