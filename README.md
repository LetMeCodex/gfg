# 🟢 GeeksforGeeks POTD Autonomous Solver (Streak & Swag Engine)

An autonomous, serverless bot designed to solve the **GeeksforGeeks Problem of the Day (POTD)** every single morning via **GitHub Actions**!

Maintain an unbroken daily streak, accumulate **GeekBits** (redeemable for free GFG T-shirts, bags, and courses), and build an exceptional DSA profile for campus placements.

---

## ⚡ Key Features

- 🎯 **Daily POTD Autonomous Solver**: Wakes up automatically at 05:45 AM IST right after GFG resets the daily problem.
- 🪙 **GeekBits & Free Swag Accumulation**: Daily streak preservation earns bonus GeekBits to claim official GFG T-Shirts, Hoodies, Laptop Bags, and Bottles.
- 🛡️ **Multi-Model Gemini Solver**: Uses Gemini 2.5 Flash with resilient fallbacks and self-healing loops to refactor failed attempts.
- 📂 **Auto-Documentation & Commits**: Formats clean GitHub-flavored markdown solutions and auto-commits to your repository.

---

## 🛠️ Step-by-Step Setup Guide

### Step 1: Create a New GitHub Repository
Create a repository (e.g. `gfg-daily-potd` or `gfg-bot`) on your GitHub account ([github.com/new](https://github.com/new)).

Push this codebase to your repository:
```bash
git init
git branch -M main
git remote add origin https://github.com/<your-username>/<your-repo-name>.git
git add .
git commit -m "feat: initial release of gfg potd bot"
git push -u origin main
```

### Step 2: Grab Your GeeksforGeeks Cookie (`GFG_COOKIE`)
1. Open [geeksforgeeks.org](https://www.geeksforgeeks.org) and log in.
2. Press `F12` (or right-click → **Inspect**) → Go to **Application** tab (or **Storage** in Firefox).
3. Under **Cookies**, click `https://www.geeksforgeeks.org`.
4. Look for the cookies:
   - `gfg_website_session`
   - `gfg_session`
   - Or in the **Network** tab, click any request to `geeksforgeeks.org` and copy the entire `Cookie` request header.

### Step 3: Add GitHub Secrets
In your GitHub repository:
1. Go to **Settings** → **Secrets and variables** → **Actions**.
2. Click **New repository secret** and add:
   - `GEMINI_API_KEY`: Your Gemini API key from [Google AI Studio](https://aistudio.google.com/).
   - `GFG_COOKIE`: Your GeeksforGeeks cookie string from Step 2.

### Step 4: Enable Workflow Write Permissions
1. Go to **Settings** → **Actions** → **General**.
2. Under **Workflow permissions**, select **"Read and write permissions"**.
3. Click **Save**.

---

## 🧪 Testing

### Manual 1-Click Run
Go to your repo's **Actions** tab → Select **GeeksforGeeks POTD Daily Solver** → Click **Run workflow**!

### Local Dry Run
```bash
set DRY_RUN=true
set GEMINI_API_KEY="your-gemini-key"
python -m src.runner
```
