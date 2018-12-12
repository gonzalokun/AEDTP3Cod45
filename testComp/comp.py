cont = 0

tINIT = 1000
tNOW = tINIT
tMIN = 1
coef = 0.99

while(tNOW >= tMIN):
    cont += 1
    tNOW *= coef

print("tINIT = " + str(tINIT))
print("tMIN = " + str(tMIN))
print("COEF = " + str(coef))
print("SE REALIZARON " + str(cont) + " CICLOS")
