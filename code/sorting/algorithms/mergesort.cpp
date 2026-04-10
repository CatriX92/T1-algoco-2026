// Algoritmo sacado de: https://www.geeksforgeeks.org/cpp/cpp-program-for-merge-sort/
// C++ program for the implementation of merge sort


#include <algorithm>
#include <vector>
#include <queue>
#include <cmath>
#include <climits>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include <chrono>

using namespace std;
namespace fs = filesystem;

const fs::path INPUT_DIR  = "data/array_input";
const fs::path OUTPUT_DIR = "data/array_output";

void merge(vector<int>& arr, int left, int mid, int right){
                         
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Create temp vectors
    vector<int> L(n1), R(n2);

    // Copy data to temp vectors L[] and R[]
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0;
    int k = left;

    // Merge the temp vectors back 
    // into arr[left..right]
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(vector<int>& arr, int left, int right){
    
    if (left >= right)
        return;

    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

void procesarArchivo(const fs::path& inputPath, const fs::path& outDir) {
    ifstream in(inputPath);

    vector<int> arr;
    int x;
    while (in >> x) arr.push_back(x);
    in.close();

    if (arr.empty()) { cerr << "⚠ Archivo vacío: " << inputPath << "\n"; return; }

    size_t memBytes = arr.size() * sizeof(int);
    cout << "Procesando " << inputPath.filename().string()
         << " | n=" << arr.size()
         << " | ~mem=" << (memBytes / (1024.0*1024.0)) << " MB\n";

    auto start = chrono::high_resolution_clock::now();
    mergeSort(arr, 0, arr.size() - 1);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    fs::path outName = inputPath.stem().string() + "_out" + inputPath.extension().string();
    fs::path outPath = outDir / outName;

    ofstream out(outPath);
    for (int v : arr) out << v << ' ';
    out << '\n';
    out.close();

    cout << outPath.string() << " | tiempo=" << elapsed.count() << " s\n";
}

int main() {
    // Asegura que el directorio de salida exista
    fs::create_directories(OUTPUT_DIR);

    // Recolecta archivos .txt de INPUT_DIR
    vector<fs::directory_entry> files;
    for (const auto& entry : fs::directory_iterator(INPUT_DIR)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            files.push_back(entry);
        }
    }

    sort(files.begin(), files.end(), [](const fs::directory_entry& a, const fs::directory_entry& b) {
            auto sa = fs::file_size(a.path());
            auto sb = fs::file_size(b.path());
            if (sa == sb) return fs::last_write_time(a) < fs::last_write_time(b);
            return sa < sb;
        });

    auto globalStart = chrono::high_resolution_clock::now();
    for (const auto& e : files) procesarArchivo(e.path(), OUTPUT_DIR);
    auto globalEnd = chrono::high_resolution_clock::now();
    cout << "\nTiempo total: "
         << chrono::duration<double>(globalEnd - globalStart).count()
         << " s\n";

    return 0;
}