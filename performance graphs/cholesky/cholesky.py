import subprocess
import matplotlib.pyplot as plt

# Configura las rutas completas a los ejecutables de C++
pthread_executable = '/home/antony/CLionProjects/cpd_proyect/cholesky/cholesky_pthread'
tbb_executable = '/home/antony/CLionProjects/cpd_proyect/cholesky/cholesky_tbb'

# Configura los valores de num_prices para probar
#num_prices_values = [10, 50, 100, 150, 200, 250]
num_prices_values = [500, 1000, 1500, 2000, 2500]
execution_times_pthread = []
execution_times_tbb = []

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

# Grafica los tiempos de ejecución
plt.plot(num_prices_values, execution_times_pthread, label='Pthread', marker='o')
plt.plot(num_prices_values, execution_times_tbb, label='TBB', marker='o')
plt.xlabel('Levels')
plt.ylabel('Execution Time (seconds)')
plt.title('Execution Time of Cholesky with Pthread vs TBB')
plt.legend()
plt.yscale('log')
plt.grid(True, which="both", ls="--")
plt.show()
