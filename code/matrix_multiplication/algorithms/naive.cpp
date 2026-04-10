#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <fstream>
#include <filesystem>

#include <sys/resource.h>

using namespace std;
namespace fs = std::filesystem;

void multiplyNaive(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

vector<vector<int>> leerMatriz(const string& path, int n) {
    vector<vector<int>> matrix(n, vector<int>(n));
    ifstream file(path);
    if (!file.is_open()) return {};
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            file >> matrix[i][j];
        }
    }
    return matrix;
}

void guardarMatriz(const string& path, const vector<vector<int>>& matrix) {
    ofstream file(path);
    for (const auto& row : matrix) {
        for (size_t i = 0; i < row.size(); i++) {
            file << row[i] << (i == row.size() - 1 ? "" : " ");
        }
        file << "\n";
    }
}

void procesarArchivos(int n, string t, string d, string m, string input_dir, string output_dir, string algo_type) {

    string prefix = to_string(n) + "_" + t + "_" + d + "_" + m;
    string path1 = input_dir + prefix + "_1.txt";
    string path2 = input_dir + prefix + "_2.txt";
    string pathOut = output_dir + prefix + "_out.txt";

    if (!fs::exists(path1) || !fs::exists(path2)) return;

    auto A = leerMatriz(path1, n);
    auto B = leerMatriz(path2, n);
    vector<vector<int>> C(n, vector<int>(n, 0));

    auto start = chrono::high_resolution_clock::now();
    
    if (algo_type == "naive") {
        multiplyNaive(A, B, C, n);
    } 

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    long memory = usage.ru_maxrss; // En KB (Linux)

    guardarMatriz(pathOut, C);

    cout << algo_type << "\t" << prefix << "\t" 
         << elapsed.count() << "s\t" << memory << "KB" << endl;
}

int main() {
    string INPUT_DIR = "code/matrix_multiplication/data/matrix_input/";
    string OUTPUT_DIR = "code/matrix_multiplication/data/matrix_output/";
    fs::create_directories(OUTPUT_DIR);

    vector<int> N_vals = {16, 64, 256, 1024}; 
    vector<string> T_vals = {"dispersa", "diagonal", "densa"};
    vector<string> D_vals = {"D0", "D10"};
    vector<string> M_vals = {"a", "b", "c"};

    cout << "ALGO\tCONFIG\tTIEMPO\tMEMORIA" << endl;
    cout << "--------------------------------------------------------" << endl;

    for (int n : N_vals) {
        for (const string& t : T_vals) {
            for (const string& d : D_vals) {
                for (const string& m : M_vals) {
                    // Llamada unificada
                    procesarArchivos(n, t, d, m, INPUT_DIR, OUTPUT_DIR, "naive");
                }
            }
        }
    }

    return 0;
}