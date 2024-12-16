
#include <tbb/parallel_for.h>
#include <cmath>
#include <vector>
#include <iostream>

const double RISK_FREE = 0.02;
const double VOLATILITY = 0.30;

double blackScholesFormula(double S, double K, double T)
{
    double d1 = (std::log(S / K) + (RISK_FREE + 0.5 * VOLATILITY * VOLATILITY) * T) / (VOLATILITY * std::sqrt(T));
    double d2 = d1 - VOLATILITY * std::sqrt(T);
    double callPrice = S * 0.5 * (1 + std::erf(d1 / std::sqrt(2))) - K * std::exp(-RISK_FREE * T) * 0.5 * (1 + std::erf(d2 / std::sqrt(2)));
    return callPrice;
}

void holaMundoTBB(std::vector<double>& prices, std::vector<double>& results, int start, int end)
{
    tbb::parallel_for(start, end, [&](int i)
    {
        results[i] = blackScholesFormula(prices[i], 100, 1); // K=100, T=1
    });
}
