#include <thread>
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

    std::vector<double> prices(num_prices, 50.0);  // Precios iniciales
    std::vector<double> results(num_prices);      // Vector para almacenar los resultados

    int half = num_prices / 2;

    // Crear hilos para CUDA y TBB
    std::thread cuda_thread(holaMundoCUDA, std::ref(prices), std::ref(results), half);
    std::thread tbb_thread(holaMundoTBB, std::ref(prices), std::ref(results), half, num_prices);

    // Esperar a que ambos hilos terminen
    cuda_thread.join();
    tbb_thread.join();

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