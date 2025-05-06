# 🔍 Parallel File Search Tool

## Compilation Instructions

Depending on your system and compiler setup, here are the possible commands:

<details>
<summary><strong>g++ (on Windows / Linux / WSL / Homebrew GCC)</strong></summary>

```bash
# If using g++ with OpenMP support (like g++-14 from Homebrew):
g++ -std=c++17 -fopenmp search.cpp -o search_tool


# Without OpenMP (no parallelism):
g++ -std=c++17 search.cpp -o search_tool
```

</details>


<details>
<summary><strong>clang++ (macOS with Homebrew's libomp)</strong></summary>

```bash
# With OpenMP support using libomp
clang++ -std=c++17 \
  -I/opt/homebrew/opt/libomp/include \
  -L/opt/homebrew/opt/libomp/lib \
  -lomp -fopenmp \
  search.cpp -o search_tool
```

</details>

---

## Test File System Generation (Shell Scripts)

You can use these helper scripts to create a test directory with multiple nested folders and files. A small percentage of the files will contain the target word.


**Usage**:

```bash
chmod +x generate_complex.sh generate_simple.sh

# Generate with your search word (example: "apple")
./generate_complex.sh apple
./generate_simple.sh apple
```

---

## Running the Search Tool

After compiling your binary (let's assume it’s called `search_tool`):

```bash
./search_tool <path_to_root_dir> <search_word> [--mode=exact|case|fuzzy]
```

### Examples:

```bash
# Exact match in complex test dir
./search_tool test_dir_complex apple --mode=exact

# Case-insensitive match
./search_tool test_dir_complex apple --mode=case

# Fuzzy match (finds words with small typo differences)
./search_tool test_dir_complex aple --mode=fuzzy
```

---

## Output Behavior

- All logs and debug messages go to `log.txt`
- **Matches (i.e., files where word was found)** are **also printed live to the terminal**

## Benchmarking 

The tool we are benchmarking against is ripgrep 
- To install ripgrep in Linux

```bash
sudo apt update && sudo apt install ripgrep
```

just run the benchmark script and the output will be available in the benchmark_result.log
```bash
./benchmark.sh
```


---