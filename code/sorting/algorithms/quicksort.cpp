// Algoritmo sacado de: https://www.geeksforgeeks.org/cpp/cpp-program-for-quicksort/

#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
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

int partition(vector<int> &vec, int low, int high) {

    // Selecting a random element as the pivot to avoid O(n^2) worst case
    int randomIndex = low + rand() % (high - low + 1);
    swap(vec[randomIndex], vec[high]);
    
    int pivot = vec[high];

    // Index of elemment just before the last element
    // It is used for swapping
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {

        // If current element is smaller than or
        // equal to pivot
        if (vec[j] <= pivot) {
            i++;
            swap(vec[i], vec[j]);
        }
    }

    // Put pivot to its position
    swap(vec[i + 1], vec[high]);

    // Return the point of partition
    return (i + 1);
}

void quickSort(vector<int> &vec, int low, int high) {

    // Iterative quicksort using explicit stack to handle large arrays
    // This avoids stack overflow with millions of elements
    stack<pair<int, int>> stk;
    stk.push({low, high});

    while (!stk.empty()) {
        low = stk.top().first;
        high = stk.top().second;
        stk.pop();

        if (low < high) {
            int pi = partition(vec, low, high);

            // Push left part to stack
            stk.push({low, pi - 1});

            // Push right part to stack
            stk.push({pi + 1, high});
        }
    }
}

void procesarArchivo(const fs::path& inputPath, const fs::path& outDir) {
    ifstream in(inputPath);

    vector<int> arr;
    int x;
    while (in >> x) arr.push_back(x);
    in.close();

    size_t memBytes = arr.size() * sizeof(int);
    cout << "Procesando " << inputPath.filename().string()
         << " | n=" << arr.size()
         << " | ~mem=" << (memBytes / (1024.0*1024.0)) << " MB\n";

    auto start = chrono::high_resolution_clock::now();
    quickSort(arr, 0, (int)arr.size() - 1);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    // Construir nombre de salida: <nombre_entrada_sin_ext>_out.txt en otro directorio
    fs::path outName = inputPath.stem().string() + "_out" + inputPath.extension().string();
    fs::path outPath = outDir / outName;

    ofstream out(outPath);
    for (int v : arr) out << v << ' ';
    out << '\n';
    out.close();

    cout << outPath.string() << " | tiempo=" << elapsed.count() << " s\n";
}

int main() {
    // Semilla del RNG UNA sola vez
    srand(static_cast<unsigned>(time(nullptr)));

    // Asegura que el directorio de salida exista
    fs::create_directories(OUTPUT_DIR);

    // Recolecta archivos .txt de INPUT_DIR
    vector<fs::directory_entry> files;
    for (const auto& entry : fs::directory_iterator(INPUT_DIR)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            files.push_back(entry);
        }
    }

    // Ordena: primero por tamaño (menor a mayor), luego por fecha (más viejo a más nuevo)
    sort(files.begin(), files.end(),
        [](const fs::directory_entry& a, const fs::directory_entry& b) {
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