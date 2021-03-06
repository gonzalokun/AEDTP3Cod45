import networkx as nx
import numpy as np
import csv
from numpy import loadtxt
import matplotlib.pyplot as plt
import pylab
import random

cnames = {
'aquamarine':           '#7FFFD4',
'azure':                '#F0FFFF',
'beige':                '#F5F5DC',
'bisque':               '#FFE4C4',
'black':                '#000000',
'blanchedalmond':       '#FFEBCD',
'blue':                 '#0000FF',
'blueviolet':           '#8A2BE2',
'brown':                '#A52A2A',
'burlywood':            '#DEB887',
'cadetblue':            '#5F9EA0',
'chartreuse':           '#7FFF00',
'chocolate':            '#D2691E',
'coral':                '#FF7F50',
'cornflowerblue':       '#6495ED',
'cornsilk':             '#FFF8DC',
'crimson':              '#DC143C',
'cyan':                 '#00FFFF',
'darkblue':             '#00008B',
'darkcyan':             '#008B8B',
'darkgoldenrod':        '#B8860B',
'darkgray':             '#A9A9A9',
'darkgreen':            '#006400',
'darkkhaki':            '#BDB76B',
'darkmagenta':          '#8B008B',
'darkolivegreen':       '#556B2F',
'darkorange':           '#FF8C00',
'darkorchid':           '#9932CC',
'darkred':              '#8B0000',
'darksalmon':           '#E9967A',
'darkseagreen':         '#8FBC8F',
'darkslateblue':        '#483D8B',
'darkslategray':        '#2F4F4F',
'darkturquoise':        '#00CED1',
'darkviolet':           '#9400D3',
'deeppink':             '#FF1493',
'deepskyblue':          '#00BFFF',
'dimgray':              '#696969',
'dodgerblue':           '#1E90FF',
'firebrick':            '#B22222',
'floralwhite':          '#FFFAF0',
'forestgreen':          '#228B22',
'fuchsia':              '#FF00FF',
'gainsboro':            '#DCDCDC',
'ghostwhite':           '#F8F8FF',
'gold':                 '#FFD700',
'goldenrod':            '#DAA520',
'gray':                 '#808080',
'green':                '#008000',
'greenyellow':          '#ADFF2F',
'honeydew':             '#F0FFF0',
'hotpink':              '#FF69B4',
'indianred':            '#CD5C5C',
'indigo':               '#4B0082',
'lime':                 '#00FF00',
'limegreen':            '#32CD32',
'linen':                '#FAF0E6',
'magenta':              '#FF00FF',
'maroon':               '#800000',
'mediumaquamarine':     '#66CDAA',
'mediumblue':           '#0000CD',
'mediumorchid':         '#BA55D3',
'mediumpurple':         '#9370DB',
'mediumseagreen':       '#3CB371',
'mediumslateblue':      '#7B68EE',
'mediumspringgreen':    '#00FA9A',
'mediumturquoise':      '#48D1CC',
'mediumvioletred':      '#C71585',
'midnightblue':         '#191970',
'mintcream':            '#F5FFFA',
'mistyrose':            '#FFE4E1',
'moccasin':             '#FFE4B5',
'navajowhite':          '#FFDEAD',
'navy':                 '#000080',
'oldlace':              '#FDF5E6',
'olive':                '#808000',
'olivedrab':            '#6B8E23',
'orange':               '#FFA500',
'orangered':            '#FF4500',
'orchid':               '#DA70D6',
'palegoldenrod':        '#EEE8AA',
'palegreen':            '#98FB98',
'paleturquoise':        '#AFEEEE',
'palevioletred':        '#DB7093',
'papayawhip':           '#FFEFD5',
'peachpuff':            '#FFDAB9',
'peru':                 '#CD853F',
'pink':                 '#FFC0CB',
'plum':                 '#DDA0DD',
'powderblue':           '#B0E0E6',
'purple':               '#800080',
'red':                  '#FF0000',
'rosybrown':            '#BC8F8F',
'royalblue':            '#4169E1',
'saddlebrown':          '#8B4513',
'salmon':               '#FA8072',
'sandybrown':           '#FAA460',
'seagreen':             '#2E8B57',
'seashell':             '#FFF5EE',
'sienna':               '#A0522D',
'silver':               '#C0C0C0',
'skyblue':              '#87CEEB',
'slateblue':            '#6A5ACD',
'slategray':            '#708090',
'snow':                 '#FFFAFA',
'springgreen':          '#00FF7F',
'steelblue':            '#4682B4',
'tan':                  '#D2B48C',
'teal':                 '#008080',
'thistle':              '#D8BFD8',
'tomato':               '#FF6347',
'turquoise':            '#40E0D0',
'violet':               '#EE82EE',
'wheat':                '#F5DEB3',
'yellow':               '#FFFF00',
'yellowgreen':          '#9ACD32'}

#Estos no se pueden ver en el graficador
#'white':                '#FFFFFF',
#'whitesmoke':           '#F5F5F5',

def graficarGrafo(nombre):
    G = nx.Graph()
    path = 'grafos/' + nombre + ".csv"
    f = plt.figure(nombre)
    with open(path) as csvfile:
        spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
        for row in spamreader:
            #print(n)
            G.add_edge(row[0], row[1], weight=row[2])
    pos = nx.spring_layout(G)

    #print(list(G.edges()))
    edge_weights = {(u,v):d['weight'] for u,v,d in G.edges(data=True)}
    #nx.draw(G, pos = pos, with_labels=True)
    # nodes
    #nx.draw_networkx_nodes(G, pos, node_size=1)
    #edge

    #colors = range(20)
    #plt.axis([0,10000,0,10000])
    nx.draw(G, pos,node_color='r',
        width=0.5, edge_cmap=plt.cm.Blues, with_labels=True)

    #nx.draw_networkx_edges(G,pos,edges=ejes,width=1, alpha=0.0)# edge_color='g', style='dashed')
    nx.draw_networkx_edge_labels(G,pos,font_size=1,edge_labels=edge_weights)

    #nx.draw(G, pos = pos, with_labels=True)
    #nx.draw_networkx_edge_labels(G, pos)
    #nx.draw(G, with_labels=True)
    #plt.draw()



def graficarNodos(nombre):
    G = nx.Graph()
    path = 'grafos/' + nombre + ".csv"
    f = plt.figure(nombre)
    lista = []
    fixed_positions = {}
    dic_colores = {}
    with open(path) as csvfile:
        spamreader = csv.reader(csvfile, delimiter=',', quotechar='|')
        for row in spamreader:
            #row0 = indice, row1=posx, row2=posy, row3=clusterpertenece
            lista = lista + row
            fixed_positions[row[0]] = (float(row[1]),float(row[2]))
            dic_colores[row[3]] = str(row[3])
            G.add_node(row[0],cluster=row[3] ,pos=(row[1],row[2]))
        #G.add_nodes_from(lista)
    fixed_nodes = fixed_positions.keys()
    #necesito lista de g.nodes.size pposiciones con los dic_colores
    colores = []
    listacnames=[]
    for i in ((cnames.keys()) ):
        listacnames += [(cnames[i])]

    print(G.nodes())

    cantNodosCluster = {}

    for i in range (len(list(G.nodes()))):
        #print("VALOR DE I: " + str(i))
        #colores += [listacnames[int(2 + int(dic_colores[G.node[str(i+1)]['cluster']])) % (len(listacnames))]]
        print(str(G.node[str(i+1)]['cluster']))
        if str(G.node[str(i+1)]['cluster']) not in cantNodosCluster.keys():
            cantNodosCluster[str(G.node[str(i+1)]['cluster'])] = 1
        else:
            cantNodosCluster[str(G.node[str(i+1)]['cluster'])] += 1
    
    print(cantNodosCluster)

    for elem in cantNodosCluster.keys():
        listaColores = [i for i in listacnames if (i not in colores)]
        color = random.choice(listaColores)

        for i in range(cantNodosCluster[elem]):
            colores += [color]
    
    #colores.append('#FF0000')

    print(dic_colores)
    #print(len(dic_colores.keys()))

    pos = nx.spring_layout(G,pos=fixed_positions, fixed = fixed_nodes)
    nx.draw_networkx_nodes(G,pos,node_color=colores,node_size=7)
    #pos = nx.get_node_attributes(G,'pos')
    #pos = nx.spring_layout(G)
    #nx.draw(G,pos,with_labels=True)
    #nx.draw_networkx_nodes(G,pos=pos,with_labels='True')
    #nx.draw(G, pos=pos, with_labels=True, node_size=35)


def main():
    graficarNodos("output")
    #graficarNodos("nodosP")
    #graficarNodos("nodosKClust")
    plt.show()

main()
