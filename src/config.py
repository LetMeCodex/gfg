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
    max_retries: int = Field(default_factory=lambda: int(os.getenv("MAX_RETRIES", "3")))

    # File Paths
    data_file: Path = Field(default_factory=lambda: Path(os.getenv("DATA_FILE", "data/solved_potd.json")))
    solutions_dir: Path = Field(default_factory=lambda: Path(os.getenv("SOLUTIONS_DIR", "solutions")))

    # Dry Run Mode
    dry_run: bool = Field(default_factory=lambda: os.getenv("DRY_RUN", "false").lower() in ("true", "1", "yes"))


config = BotConfig()
