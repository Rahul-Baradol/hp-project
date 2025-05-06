import matplotlib.pyplot as plt

# Tool configurations and timings (in seconds)
tools = {
    "search_tool -exact": 0.020,
    "search_tool -case": 0.009,
    "search_tool -fuzzy": 0.008,
    "grep -ril": 0.009,
    "rg -l": 0.238,
    "rg -F -l": 0.010,
    "rg -F -i -l": 0.009,
    "rg -F -w -l": 0.012,
    "rg -F -l --no-ignore --no-config": 0.010,
    "rg -F -l --mmap": 0.013,
    "rg -F -l --no-mmap": 0.010,
    "rg -F -l --threads 4": 0.010,
}

tools1 = {
    "search_tool -exact": 0.020,
    "search_tool -case": 0.009,
    "search_tool -fuzzy": 0.008,
    "grep -ril": 0.009,
    "rg -l": 0.238,
    "rg -F -l": 0.010,
    "rg -F -i -l": 0.009,
    "rg -F -w -l": 0.012,
    "rg -F -l --no-ignore --no-config": 0.010,
    "rg -F -l --mmap": 0.013,
    "rg -F -l --no-mmap": 0.010,
    "rg -F -l --threads 4": 0.010,
}

# Sort tools by time
sorted_tools = dict(sorted(tools.items(), key=lambda item: item[1]))

# Plotting
plt.figure(figsize=(12, 6))
bars = plt.barh(list(sorted_tools.keys()), list(sorted_tools.values()), color='skyblue')
plt.xlabel("Time (seconds)")
plt.title("Benchmark: Search Time Comparison for 'apple' in ./complex_fs")

# Annotate times on the bars
for bar in bars:
    width = bar.get_width()
    plt.text(width + 0.001, bar.get_y() + bar.get_height()/2,
             f"{width:.3f}s", va='center', fontsize=9)

plt.tight_layout()
plt.grid(axis='x', linestyle='--', alpha=0.5)
plt.show()
