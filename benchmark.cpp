#include <iostream>
#include <chrono>
#include <cstring>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/sysinfo.h>
#include <fstream>
#endif

using namespace std;
using namespace std::chrono;

// Funktion zum Abrufen des CPU-Namens
string getCPUName() {
    string cpuName = "Unbekannt";
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

// Funktion zum Abrufen der Gesamtspeichergröße in MB
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

// Benchmark-Funktion für Speicherbandbreite
double memoryBandwidthBenchmark(size_t dataSize) {
    char* src = new char[dataSize];
    char* dst = new char[dataSize];
    memset(src, 'A', dataSize);

    auto start = high_resolution_clock::now();
    memcpy(dst, src, dataSize);
    auto end = high_resolution_clock::now();

    duration<double> diff = end - start;
    double bandwidth = (dataSize / (1024.0 * 1024.0)) / diff.count(); // MB/s

    delete[] src;
    delete[] dst;
    return bandwidth;
}

int main() {
    cout << "Systeminformationen:" << endl;
    cout << "CPU: " << getCPUName() << endl;
    cout << "Gesamter RAM: " << getTotalRAM() << " MB" << endl;

    // Benchmark-Größen (in Bytes)
    size_t sizes[] = {
        32 * 1024,        // L1 Cache Größe
        256 * 1024,       // L2 Cache Größe
        8 * 1024 * 1024,  // L3 Cache Größe
        256 * 1024 * 1024 // RAM Größe
    };
    const char* labels[] = { "L1 Cache", "L2 Cache", "L3 Cache", "RAM" };

    cout << "\nBenchmark-Ergebnisse:" << endl;
    double totalScore = 0.0;
    for (int i = 0; i < 4; ++i) {
        double bandwidth = memoryBandwidthBenchmark(sizes[i]);
        cout << labels[i] << " Bandbreite: " << bandwidth << " MB/s" << endl;
        totalScore += bandwidth;
        // Kurze Pause zwischen den Benchmarks
        this_thread::sleep_for(milliseconds(500));
    }

    // Definiere einen Score basierend auf der Gesamtbandbreite
    double score = totalScore / 4.0;
    cout << "\nGesamtscore: " << score << endl;

    return 0;
}
