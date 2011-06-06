#!/usr/bin/env python
# encoding: utf-8

gamma_plus = 0
gamma_minus = 1

def capacidad_corte(min_cut, network):
    """
    Calcula la capacidad del corte min_cup en el network "network".
    min_cut es una lista de nodos.
    """
    cap = 0
    for node in min_cut:
        Rplus  = network[node][gamma_plus]

        for (neighbour, capacity) in Rplus:
            if neighbour not in min_cut:
                cap += capacity

    return cap

def network_create(file_name):
    f = open(file_name, "r")
    network = {}
    while(True):
        try:
            t = [int(x) for x in f.readline().split(" ")]
            edge = tuple(t)
        except:
            break

        if edge[0] not in network:
            network[edge[0]] = ([],[])
        if edge[1] not in network:
            network[edge[1]] = ([],[])
        network[edge[0]][gamma_plus].insert(0, (edge[1], edge[2]))
        network[edge[1]][gamma_minus].insert(0, (edge[0], edge[2]))

    return network
