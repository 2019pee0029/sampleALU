# GitHub Submission Guide for sampleALU Project

## Step 1: Initialize Git Repository (Local)

```bash
cd ~/sampleALU
git init
git config user.name "Your Name"
git config user.email "your.email@example.com"
```

## Step 2: Create .gitignore File

```bash
# Create .gitignore to exclude build artifacts
cat > .gitignore << 'EOF'
# Build directories
build/
CMakeFiles/
cmake_install.cmake
CMakeCache.txt

# Executables
*.exe
*.out
*.o
*.a
*.so

# IDE files
.vscode/
.idea/
*.code-workspace

# OS files
.DS_Store
Thumbs.db
EOF
```

## Step 3: Add Files to Git

```bash
git add .
git status  # Verify files to be committed
```

## Step 4: Make Initial Commit

```bash
git commit -m "Initial commit: ALU Integration project with parametrized adder/subtractor"
```

## Step 5: Create GitHub Repository

1. Go to https://github.com/new
2. Enter repository name: `sampleALU`
3. Description: "Arithmetic Logic Unit (ALU) combining parametrized adder and subtractor with comprehensive test coverage"
4. Choose: Public or Private
5. Do NOT initialize with README (you already have one)
6. Click "Create repository"

## Step 6: Connect Local Repo to GitHub

After creating the repository, you'll see instructions. Run:

```bash
git branch -M main
git remote add origin https://github.com/YOUR_USERNAME/sampleALU.git
git push -u origin main
```

Replace `YOUR_USERNAME` with your actual GitHub username.

## Step 7: Verify on GitHub

- Visit: https://github.com/YOUR_USERNAME/sampleALU
- Verify all files are there
- README should display automatically

## Optional: Add GitHub Actions (Automated Testing)

Create `.github/workflows/build.yml`:

```yaml
name: Build and Test

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - name: Install SystemC
        run: |
          sudo apt-get update
          sudo apt-get install -y cmake ninja-build
      - name: Build
        run: |
          mkdir -p build
          cd build
          cmake ..
          cmake --build .
      - name: Run Tests
        run: ./build/testbench/tb_alu.exe
```

## Useful Git Commands

```bash
# Check status
git status

# View commit history
git log

# Make changes and push
git add .
git commit -m "Commit message"
git push

# Clone the repository
git clone https://github.com/YOUR_USERNAME/sampleALU.git

# Pull latest changes
git pull
```

## Project Structure on GitHub

```
sampleALU/
├── include/
│   ├── parametrized_adder.h
│   └── alu.h
├── testbench/
│   └── tb_alu.cpp
├── scripts/
│   └── run.sh
├── CMakeLists.txt
├── README.md
├── .gitignore
└── .github/
    └── workflows/
        └── build.yml (optional)
```

## Sharing the Repository

Once on GitHub, you can share:
- **Clone URL**: `https://github.com/YOUR_USERNAME/sampleALU.git`
- **Direct Link**: `https://github.com/YOUR_USERNAME/sampleALU`

## Next Steps

1. Add topics to repository: `alu`, `systemc`, `hardware-design`, `arithmetic-logic-unit`
2. Add a license (MIT, GPL, etc.)
3. Enable GitHub Pages for documentation
4. Set up branch protection rules
