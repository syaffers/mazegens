import random
import networkx as nx
import numpy as np
import matplotlib.pyplot as plt

m = 32
n = 32
num_vertices = m * n
num_edges = 2 * m * n - 1
G = nx.Graph()

for i in range(m):
    for j in range(n):
        if i < (m - 1):
            G.add_edge((i,j), (i+1, j))
        if j < (n - 1):
            G.add_edge((i,j), (i, j+1))

num_mazes = 1
mazes = []

# Generate maps.
for _ in range(num_mazes):
    # Reset maze matrix.
    maze = np.zeros((2*m+1, 2*n+1), dtype=np.int)
    # Reset visited nodes.
    for node in G.nodes():
        maze[2*node[0]+1, 2*node[1]+1] = 1
        G.nodes[node]['visited'] = False

    # Pick a random starting node.
    current = random.randint(0, m-1), random.randint(0, n-1)
    G.nodes[current]['visited'] = True
    history = [current]

    # Break the walls!
    while True:
        # Get all reachable neighbors from current node.
        reachable = [node for node in nx.neighbors(G, current) if not G.nodes[node]['visited']]
        # If there are reachable neighbors from current node, get a
        # random neighbor, set it to be visited and put node in
        # history.
        if len(reachable) > 0:
            prev = current
            current = random.choice(reachable)

            dm = current[0] - prev[0]
            dn = current[1] - prev[1]
            maze[2 * prev[0] + dm + 1, 2 * prev[1] + dn + 1] = 1

            G.nodes[current]['visited'] = True
            history.append(current)
        # If there are no reachable neighbors, check if there are nodes
        # left to pop from history
        elif len(history) > 0:
            current = history.pop()
        else:
            break

    start = random.choice([node for node in G.nodes])
    end = random.choice([node for node in G.nodes])
    mazes.append(maze.flatten())
    plt.imshow(maze)
    plt.show()

mazes = np.array(mazes)
