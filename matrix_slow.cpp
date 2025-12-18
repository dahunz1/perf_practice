#include <iostream>
#include <vector>
#include <chrono>
#include <random>

// 定义矩阵大小：1024 x 1024
// 这个规模足够让 CPU 的 L1/L2 Cache 装不下，从而暴露性能问题
const int N = 1024;

using Matrix = std::vector<std::vector<double>>;

// 初始化矩阵
void initMatrix(Matrix& M) {
    std::mt19937 gen(42);
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            M[i][j] = dis(gen);
        }
    }
}

int main() {
    Matrix A(N, std::vector<double>(N));
    Matrix B(N, std::vector<double>(N));
    Matrix C(N, std::vector<double>(N, 0.0));

    initMatrix(A);
    initMatrix(B);

    std::cout << "Starting Naive Matrix Multiplication (N=" << N << ")..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    // --- 性能瓶颈区域 ---
    // 经典的 i-j-k 循环
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            double sum = 0.0;
            for (int k = 0; k < N; ++k) {
                // A[i][k] 是按行读取 (连续，快)
                // B[k][j] 是按列读取 (跳跃，极慢！) -> Cache Miss 重灾区
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
    // -------------------

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    std::cout << "Time: " << diff.count() << " s" << std::endl;
    // 防止编译器把计算优化掉，随便打印一个值
    std::cout << "Result C[0][0]: " << C[0][0] << std::endl;

    return 0;
}
