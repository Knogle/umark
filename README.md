# UMark - Universal Memory Benchmark Tool

UMark is a cross-platform command-line tool for benchmarking memory bandwidth, including L1-L3 cache and RAM. It provides results in MB/s and displays system information such as CPU model and total RAM. Based on the benchmark results, UMark calculates an overall performance score.

## Table of Contents

- [Features](#features)
- [System Requirements](#system-requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Benchmark Details](#benchmark-details)
  - [How It Works](#how-it-works)
  - [Preventing Compiler Optimizations](#preventing-compiler-optimizations)
  - [Adjusting Benchmark Parameters](#adjusting-benchmark-parameters)
- [Example Output](#example-output)
- [Limitations and Recommendations](#limitations-and-recommendations)
- [Test Cases](#test-cases)
- [Contributing](#contributing)
- [License](#license)

## Features

- Benchmarks L1, L2, and L3 cache, as well as RAM bandwidth
- Displays system information (CPU model, total RAM)
- Calculates an overall performance score based on benchmark results
- Cross-platform support (Windows and Linux)
- Uses high-precision hardware timers for accurate measurements
- Prevents compiler optimizations that could skew results

## System Requirements

- A C++11 compliant compiler (e.g., GCC, Clang, MSVC)
- Operating System: Windows or Linux
- No additional libraries required (uses standard C++ libraries)
  - On Linux, may need to link with `-lrt` and `-lpthread`

## Installation

### Windows

1. **Prerequisites**:
   - Install Visual Studio with C++ development tools or MinGW-w64 for GCC.

2. **Download the Source Code**:
   - Save the `umark.cpp` source file to your local machine.

3. **Compile with Visual Studio Command Prompt**:
   ```bash
   cl /EHsc /O2 umark.cpp /Feumark.exe
   ```

   **Or compile with MinGW-w64 GCC**:
   ```bash
   g++ -std=c++11 -O2 -o umark.exe umark.cpp
   ```

### Linux

1. **Prerequisites**:
   - Install GCC or Clang.

2. **Download the Source Code**:
   - Save the `umark.cpp` source file to your local machine.

3. **Compile**:
   ```bash
   g++ -std=c++11 -O2 -o umark umark.cpp -lrt -lpthread
   ```

   - The `-lrt` flag links the real-time library, which may be necessary for `clock_gettime` on some systems.
   - The `-lpthread` flag links the pthreads library, which is used for sleeping between benchmarks.

## Usage

After compiling, run the executable from the command line:

```bash
./umark        # On Linux
umark.exe      # On Windows
```

The program will display system information, perform the benchmarks, and output the results along with the overall performance score.

## Benchmark Details

### How It Works

UMark performs memory bandwidth benchmarks for:

- **L1 Cache**
- **L2 Cache**
- **L3 Cache**
- **RAM**

**Benchmark Process:**

1. **Memory Allocation**: Allocates source (`src`) and destination (`dst`) buffers corresponding to typical cache sizes and RAM.

2. **Warm-up Run**: Performs an initial `memcpy` to warm up the cache and reduce one-time overhead.

3. **Memory Operations**: Repeats `memcpy` operations for a specified number of iterations, measuring the total time taken.

4. **Bandwidth Calculation**: Calculates bandwidth as the total data transferred divided by the total time taken, resulting in MB/s.

5. **Preventing Compiler Optimizations**: Uses a volatile `sink` variable to prevent the compiler from optimizing away the `memcpy` operations.

6. **Performance Score**: The overall score is the average of the individual bandwidth measurements.

### Preventing Compiler Optimizations

To ensure accurate measurements, UMark prevents the compiler from optimizing away the `memcpy` operations by:

- **Using a Volatile Sink Variable**: After the benchmark loop, the code updates a volatile `sink` variable with a value from the destination buffer.

- **Displaying Sink Value**: The sink value is displayed at the end of the program to ensure it's used, reinforcing to the compiler that the data is needed.

### Adjusting Benchmark Parameters

#### Cache Sizes and Iterations

The default cache sizes and iterations are set to typical values but can be adjusted to match your system's specifications:

```cpp
// Benchmark sizes (in bytes)
size_t sizes[] = {
    64 * 1024,        // L1 Cache Size (64 KB)
    512 * 1024,       // L2 Cache Size (512 KB)
    32 * 1024 * 1024, // L3 Cache Size (32 MB)
    256 * 1024 * 1024 // RAM Size (256 MB)
};

// Number of iterations for each size
int iterations[] = { 100000, 50000, 10000, 1000 };
```

- **Adjusting Sizes**: Modify the `sizes` array to reflect your CPU's cache sizes.

- **Adjusting Iterations**: The `iterations` array determines how many times the memory operation is performed. Adjust these values to balance between benchmark duration and accuracy.

#### Preventing Excessive Memory Usage

Be cautious with the number of iterations for larger data sizes to prevent excessive memory usage and long execution times.

- **Total Data Processed**: Calculated as `dataSize * iterations`. Ensure this value is reasonable for your system.

#### Example Adjustments

If you find the benchmark runs too long or uses too much memory, consider reducing the number of iterations:

```cpp
int iterations[] = { 50000, 25000, 5000, 500 };
```

## Example Output

```
System Information:
CPU: AMD Ryzen 7 5800X 8-Core Processor
Total RAM: 31998 MB

Benchmark Results:
L1 Cache Bandwidth: 48000.00 MB/s
L2 Cache Bandwidth: 26000.00 MB/s
L3 Cache Bandwidth: 22000.00 MB/s
RAM Bandwidth: 32000.00 MB/s

Overall Performance Score: 32000.00 MB/s
Sink value (for optimization prevention): 65
```

**Note**: The above values are illustrative. Actual bandwidth will vary based on system specifications, current load, and other factors.

## Limitations and Recommendations

### Limitations

- **Simplification**: The benchmark provides a simplified measurement of memory bandwidth and may not capture all aspects of memory performance.

- **System Variability**: Results can vary due to system load, background processes, and hardware differences.

- **Compiler Optimizations**: Despite precautions, some compilers might still optimize code in unexpected ways.

### Recommendations

- **Run Multiple Times**: Execute the benchmark several times and consider averaging the results for consistency.

- **Minimal System Load**: Close unnecessary applications to prevent interference with the benchmark.

- **Adjust Parameters**: Tailor the cache sizes and iterations to match your system and achieve reasonable execution times.

- **Validation**: Cross-verify results with other benchmarking tools for accuracy.

- **Advanced Benchmarking**: For more detailed analysis, consider integrating specialized libraries or tools.

## Test Cases

### Testing on Different Systems

- **High-Performance Desktop**: Expect higher bandwidth values due to faster CPU and RAM.

- **Laptop**: Moderate bandwidth values.

- **Virtual Machine**: May show lower performance due to virtualization overhead.

### Verifying Accuracy

- **Consistency**: Run the benchmark multiple times to check for consistent results.

- **Comparison**: Compare results with other benchmarking tools like AIDA64 or memtest86.

## Contributing

Contributions are welcome! If you'd like to improve UMark, please fork the repository and submit a pull request.

### To Do

- Add support for multithreaded benchmarks.
- Implement different memory access patterns (sequential, random).
- Integrate hardware-specific optimizations (e.g., using SIMD instructions).
- Include memory latency measurements.

## License

This project is licensed under the MIT License.

---

**Note**: UMark is intended for educational and informational purposes. While it provides basic benchmarking capabilities, for professional or detailed analysis, consider using specialized benchmarking software.

