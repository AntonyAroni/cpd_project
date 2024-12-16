#ifndef HOLA_MUNDO_H
#define HOLA_MUNDO_H

#include <vector>

void holaMundoCUDA(std::vector<double>& prices, std::vector<double>& results, int num_prices);
void holaMundoTBB(std::vector<double>& prices, std::vector<double>& results, int start, int end);

#endif // HOLA_MUNDO_H
