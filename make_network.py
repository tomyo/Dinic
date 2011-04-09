#!/usr/bin/python
# encoding: utf-8

from __future__ import with_statement

import sys
import optparse
import subprocess
from random import randrange, sample

try:
    import networkx as nx
except ImportError:
    print "\nInstalen networkx:\n"
    print "    sudo easy_install networkx\n"
    sys.exit(1)

#max_weight = (2 ** 32)
max_weight = 30

def create_graph(n, m):
    """
    Crea un grafo a partir de path aleatorios de s a t.
    Argumentos:
        n   Cantidad de nodos (Incluyendo s y t)
        m   Cantidad de aristas

    """
    G = nx.DiGraph()
    total_edges = set()

    while G.number_of_edges() < m:

        #Cantidad de vertices intermedios en el path (desde s a t)
        lenght_path = randrange(0, (n - 2) + 1)

        # Lista de nodos (Excluyendo s y t)
        nodes = range(2, n+1)
        path = sample(nodes, lenght_path)

        # Agregamos s y t
        path.insert(0, 0)
        path.append(1)

        # Sacamos las aristas ya incluidas
        path_edges = set(zip(path, path[1:]))
        path_edges = path_edges - total_edges

        # Creamos las capacidades
        capacities = [randrange(0, max_weight) for x in path_edges]
        path = zip(path, path[1:], capacities)

        G.add_weighted_edges_from(path)

    return G


def min_path_max_flow(G, s, t):
    """
    Valor del flow maximal del camino minimo desde s a t.

    Devuelve el flow y el path minimo:
        return (flow, path)
    """
    #Tiene el orden inverso al nuestro, algunos casos no van a dar bien
    path = nx.shortest_path(G, s, t)

    if len(path) == 2:
        flow = G.get_edge_data(0, 1)["weight"]
    else:
        spath = nx.subgraph(G, path)
        flow, _ = nx.ford_fulkerson(spath, s, t, capacity='weight')

    return flow, path


def main():
    parser = optparse.OptionParser(usage="%prog [options] n [m]")
    parser.add_option("-n", "--nodes", dest="nodes",
        help=u"Cantidad de nodos.", type="int")
    parser.add_option("-m","-e","--edges", dest="edges",
        help=u"Cantidad de aristas.", type="int")
    parser.add_option("-g", dest="draw", action="store_true",
        help=u"Dibujar el grafo?", default=False)
    options, args = parser.parse_args()

    draw = options.draw
    if draw:
        try:
            import pygraphviz
        except ImportError:
            print "Hace falta pygraphviz para dibujar:\n"
            print "    sudo apt-get install graphviz python-pygraphviz\n"
            sys.exit(1)

    # El numero de nodos no es opcional.
    # Se pasa como argumento o explicito (Con -n --nodes).
    nodes = options.nodes
    if not nodes:
        if len(args) >= 1:
            nodes = int(args[0])
        else:
           parser.print_help()
           sys.exit(1)

    # El numero de aristas es opcional.
    # Se pasa como argumento o explicito (-m -e --edges).
    edges = options.edges
    if not edges:
        if len(args) >= 2:
            edges = int(args[1])
        else:
            edges = randrange(nodes, nodes*(nodes - 2) + 1)

    if not nodes - 1 <= edges <= nodes*(nodes - 2):
        print "\nLa cantidad de aristas no hace un grafo posible.\n"
        sys.exit(1)

    G = create_graph(nodes, edges)

    for x, ndict in G.adjacency_iter():
        for y, nattr in ndict.items():
            print "%s %s %s" % (x, y, nattr["weight"])

    if draw:
        nx.drawing.write_dot(G, "graph.dot")
        subprocess.Popen(["circo -Tpng -Ograph ./graph.dot &"],
                shell=True)

    # Guardo el resultado del bfs en un archivo.
    # No lo imprimo para no ensuciar la salida del grafo.
    flow, path = min_path_max_flow(G, 0, 1)
    path = [str(x) for x in path]
    with open("result", 'w') as f:
        f.write("Existe el camino: " + ' '.join(path) + '\n')
        f.write("Con flujo: " + str(flow) + '\n')


if __name__ == '__main__':
    main()
