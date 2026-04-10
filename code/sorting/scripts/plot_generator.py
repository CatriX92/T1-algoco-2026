import pandas as pd
import matplotlib.pyplot as plt
import re
import os

def procesar_log(ruta_archivo, algoritmo):
    datos = []

    # Patrones para capturar notación científica y enteros
    re_mem = r"n=(\d+).*?mem=([\d\.e\-\+]+)"
    re_tiempo = r"tiempo=([\d\.e\-\+]+)"
    
    with open(ruta_archivo, 'r') as f:
        lineas = f.readlines()
        
        # Iteramos de dos en dos porque los datos de un test están en un par de líneas
        for i in range(0, len(lineas) - 1):
            match_n = re.search(re_mem, lineas[i])
            match_t = re.search(re_tiempo, lineas[i+1])
            
            if match_n and match_t:
                try:
                    datos.append({
                        'Algoritmo': algoritmo,
                        'N': int(match_n.group(1)),
                        'Memoria': float(match_n.group(2)),
                        'Tiempo': float(match_t.group(1))
                    })
                except ValueError as e:
                    print(f"Error convirtiendo datos en {algoritmo}: {e}")

    return datos

# --- Resto del código de carga ---
dir_logs = "./data/measurements/"
todos_los_datos = []
for algo in ["quicksort", "mergesort", "sort"]:
    archivo = f"{dir_logs}reporte_{algo}.txt"
    todos_los_datos.extend(procesar_log(archivo, algo.capitalize()))

df = pd.DataFrame(todos_los_datos)
# Agrupamos por N y Algoritmo para promediar las muestras (a, b, c)
df_avg = df.groupby(['N', 'Algoritmo']).mean(numeric_only=True).reset_index()

# --- Generación de Gráfico de Tiempo ---
plt.figure(figsize=(10, 6))
for algo in df_avg['Algoritmo'].unique():
    sub = df_avg[df_avg['Algoritmo'] == algo]
    plt.plot(sub['N'], sub['Tiempo'], marker='o', label=algo)

plt.xscale('log')
plt.yscale('log')
plt.xlabel('Cantidad de elementos (N)')
plt.ylabel('Tiempo (segundos)')
plt.title('N vs Tiempo de Ejecución')
plt.legend()
plt.grid(True, which="both", ls="-", alpha=0.3)
plt.savefig('./data/plots/grafico_tiempo.png')

# --- Generación de Gráfico de Memoria ---
plt.figure(figsize=(10, 6))
for algo in df_avg['Algoritmo'].unique():
    sub = df_avg[df_avg['Algoritmo'] == algo]
    plt.plot(sub['N'], sub['Memoria'], marker='s', label=algo)

plt.xscale('log')
plt.yscale('log')
plt.xlabel('Cantidad de elementos (N)')
plt.ylabel('Memoria (MB)')
plt.title('N vs Uso de Memoria')
plt.legend()
plt.grid(True, which="both", ls="-", alpha=0.3)
plt.savefig('./data/plots/grafico_memoria.png')

print("Gráficos generados con éxito en la carpeta measurements.")