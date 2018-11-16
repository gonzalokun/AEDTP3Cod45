#Procesa el input que recive el programa de C++ para que sea más fácil de leer ahí
import os

#Acá lo cambias por la que queres generar
entrada = open("A-n32-k5.vrp", "r")

salida = open("inputCPP.txt", "w")
for linea in entrada:
    separada = linea.split(" : ")
    print(separada)

    if separada[0] == "DIMENSION":
        salida.write(separada[1])
    elif separada[0] == "CAPACITY":
        salida.write(separada[1])
    elif separada[0] == "DEMAND_SECTION \n":
        salida.write("Dem\n")
    elif separada[0] == "DEPOT_SECTION \n":
        salida.write("Dep\n")
    elif separada[0] in ['NAME', 'COMMENT', 'TYPE', 'EDGE_WEIGHT_TYPE', 'NODE_COORD_SECTION \n', 'EOF\n']:
        continue
    else:
        salida.write(separada[0])

entrada.close()
salida.close()
