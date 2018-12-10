#Procesa el input que recive el programa de C++ para que sea más fácil de leer ahí
import os

#Acá lo cambias por la que queres generar
entrada = open("input.vrp", "r")

salida = open("inputCPP.txt", "w")
for linea in entrada:
    separada = linea.split(" : ")
 
    #if linea == separada[0]:
        #print("iguales")
        #separada = linea.split(":")
    
    #print(linea)
    print(separada)
    
    #input()
    if separada[0] == 'NAME':
        salida.write(separada[1])
    elif separada[0] == 'COMMENT' or separada[0] == 'COMMENT ':
        separada2 = separada[1][1:len(separada[1])-2].split(", ")
        stringKOPT = separada2[1].split(": ")[1]
        stringCOSTOOPT = separada2[2].split(": ")[1]
        salida.write(stringKOPT + "\n")
        salida.write(stringCOSTOOPT + "\n")
    elif separada[0] == "DIMENSION":
        salida.write(separada[1])
    elif separada[0] == "CAPACITY":
        salida.write(separada[1])
    elif separada[0] == "DEMAND_SECTION \n" or separada[0] == "DEMAND_SECTION\n" or separada[0] == "DEMAND_SECTION\t\t\n":
        salida.write("Dem\n")
    elif separada[0] == "DEPOT_SECTION \n" or separada[0] == "DEPOT_SECTION\n" or separada[0] == "DEPOT_SECTION\t\t\n":
        salida.write("Dep\n")
    elif separada[0] in ['TYPE', 'EDGE_WEIGHT_TYPE', 'NODE_COORD_SECTION \n', 'NODE_COORD_SECTION\n', 'NODE_COORD_SECTION\t\t\n', 'NODE_COORD_SECTION\t\n', 'DISTANCE', 'EDGE_WEIGHT_FORMAT','NODE_COORD_TYPE','EOF \n']:
        continue
    else:
        salida.write(separada[0])

entrada.close()
salida.close()
