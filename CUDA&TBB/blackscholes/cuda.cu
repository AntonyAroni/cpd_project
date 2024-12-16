#include <iostream>
#include <cmath>
#include <cuda_runtime.h>
#include "main.h"

const double RISK_FREE = 0.02;
const double VOLATILITY = 0.30;

__device__ double blackScholesFormulaCUDA(double S, double K, double T)
{
    // Reemplazar funciones de C++ con las versiones de CUDA
    double d1 = (logf(S / K) + (RISK_FREE + 0.5 * VOLATILITY * VOLATILITY) * T) / (VOLATILITY * sqrtf(T));
    double d2 = d1 - VOLATILITY * sqrtf(T);
    double callPrice = S * 0.5 * (1 + erff(d1 / sqrtf(2))) - K * expf(-RISK_FREE * T) * 0.5 * (1 + erff(d2 / sqrtf(2)));
    return callPrice;
}

__global__ void calculatePricesCUDA(double* prices, double* results, int num_prices)
{
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    if (idx < num_prices)
    {
        results[idx] = blackScholesFormulaCUDA(prices[idx], 100, 1); // K=100, T=1
    }
}

void holaMundoCUDA(std::vector<double>& prices, std::vector<double>& results, int num_prices)
{
    double* d_prices;
    double* d_results;

    cudaMalloc(&d_prices, num_prices * sizeof(double));
    cudaMalloc(&d_results, num_prices * sizeof(double));

    cudaMemcpy(d_prices, prices.data(), num_prices * sizeof(double), cudaMemcpyHostToDevice);

    int blockSize = 256;
    int numBlocks = (num_prices + blockSize - 1) / blockSize;

    calculatePricesCUDA<<<numBlocks, blockSize>>>(d_prices, d_results, num_prices);

    cudaMemcpy(results.data(), d_results, num_prices * sizeof(double), cudaMemcpyDeviceToHost);

    cudaFree(d_prices);
    cudaFree(d_results);
}
