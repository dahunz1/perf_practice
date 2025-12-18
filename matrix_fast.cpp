#include <iostream>
#include <vector>
#include <chrono>
#include <random>

const int N = 1024;
using Matrix = std::vector<std::vector<double>>;

void initMatrix(Matrix& M) {
    // 同样的初始化代码...
    std::mt19937 gen(42);
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            M[i][j] = dis(gen);
}

int main() {
    Matrix A(N, std::vector<double>(N));
    Matrix B(N, std::vector<double>(N));
    Matrix C(N, std::vector<double>(N, 0.0));

    initMatrix(A);
    initMatrix(B);

    std::cout << "Starting Optimized Matrix Multiplication (N=" << N << ")..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    // --- 优化后的区域 ---
    // 循环顺序改为 i-k-j
    for (int i = 0; i < N; ++i) {
        for (int k = 0; k < N; ++k) {
            // 提前取出 A[i][k]，这在内层循环里是个常数
            double r = A[i][k]; 
            for (int j = 0; j < N; ++j) {
                // C[i][j] 是连续访问 (按行)
                // B[k][j] 是连续访问 (按行) !!! 速度起飞 !!!
                C[i][j] += r * B[k][j];
            }
        }
    }
    // -------------------

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    std::cout << "Time: " << diff.count() << " s" << std::endl;
    std::cout << "Result C[0][0]: " << C[0][0] << std::endl;

    return 0;
}
