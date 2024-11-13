/*
g++ -o cholesky_tbb cholesky_tbb.cpp -ltbb -std=c++11
./cholesky_tbb 4
*/

#include <iostream>
#include <vector>
#include <cmath>
#include <tbb/tbb.h>
#include <chrono>

bool choleskyDecomposition(std::vector<std::vector<double>>& matrix, int n)
{
    // Iteración sobre cada fila de la matriz
    for (int i = 0; i < n; ++i)
    {
        // Calcular la diagonal de la fila i
        double sum = matrix[i][i];
        for (int k = 0; k < i; ++k)
        {
            sum -= matrix[i][k] * matrix[i][k];
        }

        if (sum <= 0)
        {
            std::cerr << "Matrix is not positive definite" << std::endl;
            return false;
        }

        matrix[i][i] = std::sqrt(sum);

        // Aquí hacemos el cálculo paralelo para las filas posteriores, después de haber actualizado la diagonal
        tbb::parallel_for(tbb::blocked_range<int>(i + 1, n), [&](const tbb::blocked_range<int>& r)
        {
            for (int j = r.begin(); j < r.end(); ++j)
            {
                double sum = matrix[j][i];
                for (int k = 0; k < i; ++k)
                {
                    sum -= matrix[j][k] * matrix[i][k];
                }
                matrix[j][i] = sum / matrix[i][i];
            }
        });
    }
    return true;
}

int main(int argc, char *argv[])
{
    auto start = std::chrono::high_resolution_clock::now();
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <matrix_size>" << std::endl;
        return 1;
    }

    int n = std::stoi(argv[1]);

    std::vector<std::vector<double>> matrix(n, std::vector<double>(n, 1.0));
    for (int i = 0; i < n; ++i)
        matrix[i][i] += n; // Asegura la definitud positiva

    // Configurar la cantidad de hilos en TBB (opcional)
    // tbb::task_scheduler_init init(num_threads); // Si quieres controlar el número de hilos manualmente

    // Llamada a la descomposición de Cholesky
    if (!choleskyDecomposition(matrix, n))
    {
        return 1;
    }

    // Mostrar la matriz triangular inferior resultante
    // for (int i = 0; i < n; ++i)
    // {
    //     for (int j = 0; j <= i; ++j)
    //     {
    //         std::cout << matrix[i][j] << " ";
    //     }
    //     std::cout << std::endl;
    // }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;

    return 0;
}
