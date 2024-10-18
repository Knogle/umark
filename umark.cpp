#include <iostream>
#include <cstring>
#include <thread>
#include <iomanip>
#include <cstdint>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/sysinfo.h>
#include <fstream>
#include <time.h>
#endif

using namespace std;

// Function to retrieve the CPU name
string getCPUName() {
    string cpuName = "Unknown";
#ifdef _WIN32
    HKEY hKeyProcessor;
    LONG lError = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
        0, KEY_READ, &hKeyProcessor);
    if (lError == ERROR_SUCCESS) {
        CHAR szBuffer[256];
        DWORD dwBufferSize = sizeof(szBuffer);
        lError = RegQueryValueExA(hKeyProcessor, "ProcessorNameString", NULL, NULL, (LPBYTE)szBuffer, &dwBufferSize);
        if (lError == ERROR_SUCCESS) {
            cpuName = szBuffer;
        }
        RegCloseKey(hKeyProcessor);
    }
#else
    ifstream cpuinfo("/proc/cpuinfo");
    string line;
    while (getline(cpuinfo, line)) {
        if (line.substr(0, 10) == "model name") {
            cpuName = line.substr(line.find(":") + 2);
            break;
        }
    }
    cpuinfo.close();
#endif
    return cpuName;
}

// Function to retrieve the total RAM size in MB
unsigned long long getTotalRAM() {
#ifdef _WIN32
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    return statex.ullTotalPhys / (1024 * 1024);
#else
    struct sysinfo info;
    sysinfo(&info);
    return info.totalram * info.mem_unit / (1024 * 1024);
#endif
}

// High-resolution timer functions

#ifdef _WIN32
typedef LARGE_INTEGER hr_time;

void get_current_time(hr_time* t) {
    QueryPerformanceCounter(t);
}

double get_elapsed_time(hr_time start, hr_time end) {
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart;
}
#else
typedef timespec hr_time;

void get_current_time(hr_time* t) {
    clock_gettime(CLOCK_MONOTONIC, t);
}

double get_elapsed_time(hr_time start, hr_time end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}
#endif

// Benchmark function for memory bandwidth with multiple iterations
double memoryBandwidthBenchmark(size_t dataSize, int iterations, volatile char* sink) {
    // Allocate memory
    char* src = new char[dataSize];
    char* dst = new char[dataSize];
    memset(src, 'A', dataSize);

    // Warm-up to populate caches
    memcpy(dst, src, dataSize);

    // High-resolution timing
    hr_time start_time, end_time;
    get_current_time(&start_time);

    for (int i = 0; i < iterations; ++i) {
        memcpy(dst, src, dataSize);
    }

    get_current_time(&end_time);

    // Prevent optimization by using the destination buffer
    *sink += dst[0];

    // Calculate elapsed time in seconds
    double elapsed_seconds = get_elapsed_time(start_time, end_time);

    // Total data processed in bytes
    double totalData = static_cast<double>(dataSize) * iterations;

    // Convert to megabytes and calculate bandwidth in MB/s
    double bandwidth = (totalData / (1024.0 * 1024.0)) / elapsed_seconds;

    // Clean up
    delete[] src;
    delete[] dst;

    return bandwidth;
}

int main() {
    cout << "System Information:" << endl;
    cout << "CPU: " << getCPUName() << endl;
    cout << "Total RAM: " << getTotalRAM() << " MB" << endl;

    // Benchmark sizes (in bytes)
    // Adjusted to better match typical cache sizes for AMD Ryzen 7 5800X
    size_t sizes[] = {
        64 * 1024,        // L1 Cache Size (64 KB)
        512 * 1024,       // L2 Cache Size (512 KB)
        32 * 1024 * 1024, // L3 Cache Size (32 MB)
        256 * 1024 * 1024 // RAM Size (256 MB for benchmarking purposes)
    };
    const char* labels[] = { "L1 Cache", "L2 Cache", "L3 Cache", "RAM" };
    // Adjusted number of iterations to keep totalData reasonable
    int iterations[] = { 100000, 50000, 10000, 1000 }; // Number of iterations for each size

    cout << "\nBenchmark Results:" << endl;
    double totalScore = 0.0;
    volatile char sink = 0; // To prevent optimization

    for (int i = 0; i < 4; ++i) {
        double bandwidth = memoryBandwidthBenchmark(sizes[i], iterations[i], &sink);
        cout << fixed << setprecision(2);
        cout << labels[i] << " Bandwidth: " << bandwidth << " MB/s" << endl;
        totalScore += bandwidth;
        // Short pause between benchmarks to allow system stabilization
        this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    // Define a score based on the average bandwidth
    double score = totalScore / 4.0;
    cout << "\nOverall Performance Score: " << score << " MB/s" << endl;

    // Use 'sink' to prevent compiler from optimizing away
    cout << "Sink value (for optimization prevention): " << static_cast<int>(sink) << endl;

    return 0;
}

