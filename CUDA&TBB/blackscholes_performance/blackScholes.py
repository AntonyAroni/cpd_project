import subprocess
import matplotlib.pyplot as plt

# Configura las rutas completas a los ejecutables de C++
pthread_executable = '/home/antony/CLionProjects/cpd_proyect/blackscholes/blackscholes_pthread'
tbb_executable = '/home/antony/CLionProjects/cpd_proyect/blackscholes/blackscholes_tbb'
tbb_and_cuda_executable = '/home/antony/CLionProjects/test_cuda/hola_mundo'

# Configura los valores de num_prices para probar
#num_prices_values = [10, 100, 1000, 10000, 100000]
num_prices_values = [1000, 10000, 100000, 1000000, 10000000]
execution_times_pthread = []
execution_times_tbb = []
execution_times_tbb_and_cuda = []

# Ejecuta el programa pthread con diferentes valores de num_prices
for num_prices in num_prices_values:
    result = subprocess.run([pthread_executable, '4', str(num_prices)], capture_output=True, text=True)
    # Extrae el tiempo de ejecución de la salida
    for line in result.stdout.splitlines():
        if "Execution time" in line:
            time_pthread = float(line.split(":")[1].strip().split()[0])
            execution_times_pthread.append(time_pthread)

# Ejecuta el programa TBB con diferentes valores de num_prices
for num_prices in num_prices_values:
    result = subprocess.run([tbb_executable, str(num_prices)], capture_output=True, text=True)
    # Extrae el tiempo de ejecución de la salida
    for line in result.stdout.splitlines():
        if "Execution time" in line:
            time_tbb = float(line.split(":")[1].strip().split()[0])
            execution_times_tbb.append(time_tbb)

# Ejecuta el programa TBBy CUDA con diferentes valores de num_prices
for num_prices in num_prices_values:
    result = subprocess.run([tbb_and_cuda_executable , str(num_prices)], capture_output=True, text=True)
    # Extrae el tiempo de ejecución de la salida
    for line in result.stdout.splitlines():
        if "Execution time" in line:
            time_tbb_and_cuda = float(line.split(":")[1].strip().split()[0])
            execution_times_tbb_and_cuda.append(time_tbb_and_cuda)

# Grafica los tiempos de ejecución
plt.plot(num_prices_values, execution_times_pthread, label='Pthread', marker='o')
plt.plot(num_prices_values, execution_times_tbb, label='TBB', marker='o')
plt.plot(num_prices_values, execution_times_tbb_and_cuda, label='TBB & CUDA', marker='o')
plt.xlabel('Number of Prices')
plt.ylabel('Execution Time (seconds)')
plt.title('Execution Time of Black-Scholes with Pthread vs TBB vs TBB & CUDA')
plt.legend()
plt.xscale('log')
plt.yscale('log')
plt.grid(True, which="both", ls="--")
plt.show()
