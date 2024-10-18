# UMark - Universal Memory Benchmark Tool

UMark is a cross-platform command-line tool for benchmarking memory bandwidth, including L1-L3 cache and RAM. It provides results in MB/s and displays system information such as CPU model and total RAM. Based on the benchmark results, UMark calculates an overall performance score.

## Table of Contents

- [Features](#features)
- [System Requirements](#system-requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Benchmark Details](#benchmark-details)
- [Test Cases](#test-cases)
- [Contributing](#contributing)
- [License](#license)

## Features

- Benchmarks L1, L2, and L3 cache, as well as RAM bandwidth
- Displays system information (CPU model, total RAM)
- Calculates an overall performance score based on benchmark results
- Cross-platform support (Windows and Linux)
- Easy to compile and use with standard C++ libraries

## System Requirements

- A C++11 compliant compiler (e.g., GCC, Clang, MSVC)
- Operating System: Windows or Linux
- No additional libraries required (uses standard C++ libraries)

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
   g++ -std=c++11 -O2 -o umark umark.cpp
   ```

## Usage

After compiling, run the executable from the command line:

```bash
./umark        # On Linux
umark.exe      # On Windows
```

The program will display system information, perform the benchmarks, and output the results along with the overall performance score.

## Benchmark Details

UMark performs memory bandwidth benchmarks for:

- **L1 Cache**
- **L2 Cache**
- **L3 Cache**
- **RAM**

### How It Works

- **Memory Operations**: The program allocates memory blocks corresponding to typical cache sizes and RAM, then measures the time taken to perform memory copy operations.
- **Bandwidth Calculation**: Bandwidth is calculated as the size of the data transferred divided by the time taken, resulting in MB/s.
- **Performance Score**: The overall score is the average of the individual bandwidth measurements.

### Customizing Benchmark Parameters

You can adjust the sizes of the memory blocks in the source code to match the specific cache sizes of your CPU:

```cpp
// Adjust these values as needed
size_t sizes[] = {
    32 * 1024,        // L1 Cache Size
    256 * 1024,       // L2 Cache Size
    8 * 1024 * 1024,  // L3 Cache Size
    256 * 1024 * 1024 // RAM Size
};
```

## Test Cases

### Example Output

```bash
System Information:
CPU: Intel(R) Core(TM) i7-8550U CPU @ 1.80GHz
Total RAM: 16384 MB

Benchmark Results:
L1 Cache Bandwidth: 10240 MB/s
L2 Cache Bandwidth: 8192 MB/s
L3 Cache Bandwidth: 6144 MB/s
RAM Bandwidth: 4096 MB/s

Overall Performance Score: 7168 MB/s
```

### Testing on Different Systems

- **High-Performance Desktop**:
  - Expected higher bandwidth values due to faster CPU and RAM.
- **Laptop**:
  - Moderate bandwidth values.
- **Virtual Machine**:
  - May show lower performance due to virtualization overhead.

### Verifying Accuracy

- Run the benchmark multiple times to check for consistency.
- Compare results with other benchmarking tools like AIDA64 or memtest86.

## Contributing

Contributions are welcome! If you'd like to improve UMark, please fork the repository and submit a pull request.

### To Do

- Add support for multithreaded benchmarks.
- Integrate hardware-specific optimizations (e.g., using SIMD instructions).
- Implement additional benchmarks (e.g., latency measurements).

## License

This project is licensed under the MIT License.

---

**Note**: UMark is intended for educational and informational purposes. While it provides basic benchmarking capabilities, for professional or detailed analysis, consider using specialized benchmarking software.

---

If you have any questions or need assistance, please feel free to open an issue or contact the maintainer.

