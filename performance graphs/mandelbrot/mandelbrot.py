import subprocess
import matplotlib.pyplot as plt

# Rutas a los ejecutables
pthread_executable = '/home/antony/CLionProjects/cpd_proyect/mandelbrot/mandelbrot_pthread'
tbb_executable = '/home/antony/CLionProjects/cpd_proyect/mandelbrot/mandelbrot_tbb'

# Valores de resolución para probar (ej. diferentes tamaños de imagen)
resolutions = [500, 1000, 1500, 2000, 2500]
execution_times_pthread = []
execution_times_tbb = []

# Ejecuta el programa pthread con diferentes valores de resolución
for res in resolutions:
    result = subprocess.run([pthread_executable, '4', str(res), str(res), 'output_pthread.txt'], capture_output=True, text=True)
    for line in result.stdout.splitlines():
        if "Execution time" in line:
            time_pthread = float(line.split(":")[1].strip().split()[0])
            execution_times_pthread.append(time_pthread)

# Ejecuta el programa TBB con diferentes valores de resolución
for res in resolutions:
    result = subprocess.run([tbb_executable, str(res), str(res), 'output_tbb.txt'], capture_output=True, text=True)
    for line in result.stdout.splitlines():
        if "Execution time" in line:
            time_tbb = float(line.split(":")[1].strip().split()[0])
            execution_times_tbb.append(time_tbb)

# Grafica los tiempos de ejecución
plt.plot(resolutions, execution_times_pthread, label='Pthread', marker='o')
plt.plot(resolutions, execution_times_tbb, label='TBB', marker='o')
plt.xlabel('Resolution (width and height)')
plt.ylabel('Execution Time (seconds)')
plt.title('Execution Time of Mandelbrot with Pthread vs TBB')
plt.legend()
plt.yscale('log')
plt.grid(True, which="both", ls="--")
plt.show()
