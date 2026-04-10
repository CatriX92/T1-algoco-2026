#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <algorithm>

using namespace std;
namespace fs = std::filesystem;

// --- CONFIGURACIÓN DE RUTAS ---
const string INPUT_DIR = "code/matrix_multiplication/data/matrix_input";
const string OUTPUT_DIR = "code/matrix_multiplication/data/matrix_output";

// --- ALGORITMO "NECIO" ---
void multiplyMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// --- UTILIDADES DE ARCHIVO ---
vector<vector<int>> leerMatriz(const string& path, int n) {
    vector<vector<int>> matrix(n, vector<int>(n));
    ifstream file(path);
    if (!file.is_open()) return {};
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            file >> matrix[i][j];
    return matrix;
}

void guardarMatriz(const string& path, const vector<vector<int>>& matrix) {
    ofstream file(path);
    for (const auto& row : matrix) {
        for (size_t i = 0; i < row.size(); i++)
            file << row[i] << (i == row.size() - 1 ? "" : " ");
        file << "\n";
    }
}

// --- FUNCIÓN UNIFICADA DE PROCESAMIENTO ---
void procesarArchivos(const fs::path& inputPath1, const fs::path& outDir) {
    string filename1 = inputPath1.filename().string();
    
    // 1. Identificar el archivo pareja (_2.txt) y el de salida (_out.txt)
    string base = filename1.substr(0, filename1.find("_1.txt"));
    string filename2 = base + "_2.txt";
    string filenameOut = base + "_out.txt";
    
    fs::path inputPath2 = inputPath1.parent_path() / filename2;
    fs::path outputPath = outDir / filenameOut;

    if (!fs::exists(inputPath2)) return;

    // 2. Extraer 'n' del nombre del archivo (asumiendo formato {n}_{t}_{d}_{m}_1.txt)
    int n = stoi(filename1.substr(0, filename1.find("_")));

    // 3. Cargar matrices
    auto M1 = leerMatriz(inputPath1.string(), n);
    auto M2 = leerMatriz(inputPath2.string(), n);
    vector<vector<int>> Result(n, vector<int>(n, 0));

    cout << "Procesando: " << base << " | n=" << n << "... ";

    // 4. Medir tiempo de la multiplicación
    auto start = chrono::high_resolution_clock::now();
    multiplyMatrices(M1, M2, Result, n);
    auto end = chrono::high_resolution_clock::now();
    
    chrono::duration<double> elapsed = end - start;

    // 5. Guardar resultado
    guardarMatriz(outputPath.string(), Result);

    cout << "Finalizado en: " << elapsed.count() << " s\n";
}

// --- MAIN ---
int main() {
    if (!fs::exists(INPUT_DIR)) {
        cerr << "Error: No se encuentra el directorio de entrada." << endl;
        return 1;
    }
    fs::create_directories(OUTPUT_DIR);

    // Recolectar solo los archivos que terminan en _1.txt para evitar duplicar procesos
    vector<fs::directory_entry> files;
    for (const auto& entry : fs::directory_iterator(INPUT_DIR)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            if (entry.path().string().find("_1.txt") != string::npos) {
                files.push_back(entry);
            }
        }
    }

    // Ordenar archivos por tamaño (como en tu ejemplo)
    sort(files.begin(), files.end(), [](const fs::directory_entry& a, const fs::directory_entry& b) {
        auto sa = fs::file_size(a.path());
        auto sb = fs::file_size(b.path());
        if (sa == sb) return fs::last_write_time(a) < fs::last_write_time(b);
        return sa < sb;
    });

    auto globalStart = chrono::high_resolution_clock::now();

    for (const auto& e : files) {
        procesarArchivos(e.path(), OUTPUT_DIR);
    }

    auto globalEnd = chrono::high_resolution_clock::now();
    cout << "\n========================================\n";
    cout << "Tiempo total de ejecución: "
         << chrono::duration<double>(globalEnd - globalStart).count()
         << " s\n";

    return 0;
}