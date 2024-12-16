#include <iostream>
#include <vector>
#include <chrono>
#include "main.h"

int main(int argc, char* argv[])
{
    auto start = std::chrono::high_resolution_clock::now();

    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <num_prices>" << std::endl;
        return 1;
    }

    int num_prices = std::stoi(argv[1]);

    std::vector<double> prices(num_prices, 50.0);  // Precios iniciales (por ejemplo, 50)
    std::vector<double> results(num_prices);  // Vector para almacenar los resultados

    // Calcular la mitad de los precios con CUDA
    int half = num_prices / 2;

    std::cout << "Using CUDA for the first half of the computation..." << std::endl;
    holaMundoCUDA(prices, results, half);  // Usar CUDA para la primera mitad

    // Calcular la otra mitad con TBB
    std::cout << "Using TBB for the second half of the computation..." << std::endl;
    holaMundoTBB(prices, results, half, num_prices);  // Usar TBB para la segunda mitad

    // Imprimir resultados
    for (int i = 0; i < num_prices; i++)
    {
        std::cout << "Option price " << i << ": " << results[i] << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;

    return 0;
}
