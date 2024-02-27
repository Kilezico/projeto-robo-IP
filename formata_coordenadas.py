# Código para formatar as coordenadas mais automaticamente para podermos 
# perder menos tempo fazendo isso no laboratório.
# Os ângulos devem estar um em cada linha, e a separação entra cada 
# conjunto de ângulos será uma linha em branco.

# Pega as coordenadas do arquivo e armazena numa lista
coordenadas = []
with open("angulos.txt", "r") as f:
    lista = []
    for linha in f.readlines():
        if len(linha.strip()) > 0:
            lista.append(linha.strip())
        else:
            coordenadas.append(lista.copy())
            lista.clear()
if len(lista) > 0:
    coordenadas.append(lista)

# Coloca as coordenadas no arquivo "formatado.txt" prontas para copiar e colar no arquivo .c
with open("formatados.txt", "w") as f:
    for coord, j in zip(coordenadas, range(len(coordenadas))):
        f.write("{")
        for i in range(len(coord)):
            f.write(coord[i])
            f.write(", " if i < len(coord) - 1 else "}")
        f.write(",\n" if j < len(coordenadas) - 1 else "\n")

