/*
g++ -o mandelbrot_pthread mandelbrot_pthread.cpp -lpthread -std=c++11
./mandelbrot_pthread 4 800 800 mandelbrot_output.txt
 */
#include <iostream>
#include <vector>
#include <complex>
#include <pthread.h>
#include <fstream>
#include <string>
#include <chrono>

const int MAX_ITER = 1000;
const double min_x = -2.0, max_x = 1.0, min_y = -1.5, max_y = 1.5;

struct ThreadData {
  int start, end, width, height;
  std::vector<std::vector<int>> *image;
};

int mandelbrot(const std::complex<double> &c) {
  std::complex<double> z(0.0, 0.0);
  int iter = 0;
  while (std::abs(z) <= 2.0 && iter < MAX_ITER) {
    z = z * z + c;
    iter++;
  }
  return iter;
}

void *computeMandelbrot(void *args) {
  ThreadData *data = static_cast<ThreadData *>(args);
  for (int y = data->start; y < data->end; ++y) {
    for (int x = 0; x < data->width; ++x) {
      double real = min_x + (max_x - min_x) * static_cast<double>(x) / data->width;
      double imag = min_y + (max_y - min_y) * static_cast<double>(y) / data->height;
      std::complex<double> c(real, imag);
      (*data->image)[y][x] = mandelbrot(c);
    }
  }
  return nullptr;
}

int main(int argc, char *argv[]) {
  auto start = std::chrono::high_resolution_clock::now();
  if (argc < 5) {
    std::cerr << "Usage: " << argv[0] << " <num_threads> <width> <height> <output_file>" << std::endl;
    return 1;
  }

  int num_threads = std::stoi(argv[1]);
  int width = std::stoi(argv[2]);
  int height = std::stoi(argv[3]);
  std::string output_file = argv[4];

  std::vector<std::vector<int>> image(height, std::vector<int>(width, 0));
  std::vector<pthread_t> threads(num_threads);
  std::vector<ThreadData> threadData(num_threads);

  int chunk_size = height / num_threads;

  for (int i = 0; i < num_threads; ++i) {
    threadData[i] = {i * chunk_size, (i == num_threads - 1) ? height : (i + 1) * chunk_size, width, height, &image};
    int rc = pthread_create(&threads[i], nullptr, computeMandelbrot, &threadData[i]);
    if (rc) {
      std::cerr << "Error: unable to create thread," << rc << std::endl;
      return 1;
    }
  }

  for (int i = 0; i < num_threads; ++i) {
    int rc = pthread_join(threads[i], nullptr);
    if (rc) {
      std::cerr << "Error: unable to join thread," << rc << std::endl;
      return 1;
    }
  }

  std::ofstream out(output_file);
  if (!out) {
    std::cerr << "Error: could not open file " << output_file << std::endl;
    return 1;
  }
  for (const auto &row : image) {
    for (int val : row) out << val << " ";
    out << "\n";
  }
  out.close();
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end - start;
  std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;

  return 0;
}
