#include <algorithm>
#include <iostream>
#include <time.h>
#include <random>
#include <vector>
#define W 16
#define H 16

typedef unsigned int ui;
typedef std::pair<size_t, size_t> p;
typedef std::vector<p> vecp;
typedef std::vector<ui> veci;
typedef std::vector<veci> matrix;

bool is_pair_in_vector(p, vecp);
void get_extents(p, vecp&);
void get_neighbors(p, vecp&);
void print_maze(matrix&);

int main(void)
{
    ui a, b;
    std::cin >> a >> b;

    const ui w = a + 2, h = b + 2;
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<> four(0, 4);
    std::uniform_int_distribution<> wr(1, w-2);
    std::uniform_int_distribution<> hr(1, h-2);

    matrix maze;
    vecp history;
    vecp visited;

    // Initialization.
    size_t sx = hr(rng), sy = wr(rng);
    p cur_pos(sx, sy);
    visited.push_back(cur_pos);

    // Auxiliary vectors.
    bool is_neighbor_found;
    bool is_neighbor_visited;
    bool is_extent_visited;
    vecp neighbors;
    vecp neighbor_extents;

    // Fill maze.
    for (size_t i = 0; i < h; i++)
    {
        veci row;
        for (size_t j = 0; j < w; j++)
        {
            row.push_back(1);
        }
        maze.push_back(row);
    }
    // Set starting position.
    maze[sx][sy] = 0;

    // Fill edges of the map as visited nodes.
    for (size_t i = 0; i < h; i++)
    {
        p edge_top(i, 0);
        p edge_bottom(i, w-1);
        visited.push_back(edge_top);
        visited.push_back(edge_bottom);
    }
    for (size_t j = 0; j < w; j++)
    {
        p edge_left(0, j);
        p edge_right(h-1, j);
        visited.push_back(edge_left);
        visited.push_back(edge_right);
    }

    // Iterative DFS.
    while (true)
    {
        neighbors.clear();
        is_neighbor_found = false;
        get_neighbors(cur_pos, neighbors);
        std::shuffle(neighbors.begin(), neighbors.end(), rng);

        for (auto neighbor : neighbors)
        {
            neighbor_extents.clear();
            is_neighbor_visited = is_pair_in_vector(neighbor, visited);

            if (is_neighbor_visited)
                continue;

            ui extent_visited_count = 0;
            get_extents(neighbor, neighbor_extents);

            for (auto extent : neighbor_extents)
            {
                is_extent_visited = is_pair_in_vector(extent, visited);

                if (is_extent_visited)
                    extent_visited_count += 1;
            }

            if ((!is_neighbor_visited) && (extent_visited_count < 3))
            {
                history.push_back(neighbor);
                visited.push_back(neighbor);

                maze[neighbor.first][neighbor.second] = 0;

                is_neighbor_found = true;
                break;
            }
        }

        if (history.size() == 0)
            break;

        if (!is_neighbor_found)
        {
            p last = history.back();
            history.pop_back();
            cur_pos.first = last.first;
            cur_pos.second = last.second;
        }
    }
    print_maze(maze);

}

/**
 * Checks if a pair is in a vector.
 *
 * Arguments:
 *  ui pair pos: the position under consideration.
 *  ui pair vec: the vector to check against.
 *
 * Returns:
 *  boolean: if the pair is in the vector.
 */
bool is_pair_in_vector(p pos, vecp vec)
{
    bool is_pair_in_vector = false;
    for (auto vpos : vec)
    {
        is_pair_in_vector = (pos.first == vpos.first) && (pos.second == vpos.second);
        if (is_pair_in_vector)
            break;
    }

    return is_pair_in_vector;
}

/**
 * Gets the next neighbor positions which are reachable from (x, y).
 *
 * Arguments:
 *  ui x: the x position
 *  ui y: the y position
 *
 * Returns:
 *  pair vec: vector of all points which are reachable from (x, y).
 */
void get_neighbors(p pos, vecp &neighbors)
{
    ui x = pos.first;
    ui y = pos.second;

    p t(x-1, y);
    p b(x+1, y);
    p l(x, y-1);
    p r(x, y+1);

    neighbors.push_back(t);
    neighbors.push_back(b);
    neighbors.push_back(l);
    neighbors.push_back(r);
}

/**
 * Gets the next extent (i.e. neighbor + corners) positions which are reachable from (x, y).
 *
 * Arguments:
 *  ui x: the x position
 *  ui y: the y position
 *
 * Returns:
 *  pair vec: vector of all extents which are reachable from (x, y).
 */
void get_extents(p pos, vecp &neighbors)
{
    ui x = pos.first;
    ui y = pos.second;
    get_neighbors(pos, neighbors);

    p tl(x-1, y-1);
    p tr(x-1, y+1);
    p bl(x+1, y-1);
    p br(x+1, y+1);

    neighbors.push_back(tl);
    neighbors.push_back(tr);
    neighbors.push_back(bl);
    neighbors.push_back(br);
}

/**
 * Subroutine to print the maze out
 *
 * Arguments:
 *  matrix maze: the maze object.
 */
void print_maze(matrix& maze)
{
    size_t h = maze.size();
    size_t w = maze[0].size();
    for (size_t i = 1; i < (h-1); i++)
    {
        for (size_t j = 1; j < (w-1); j++)
        {
            if (maze[i][j] == 1)
                std::cout << "#";
            else
                std::cout << ".";
        }
        std::cout << std::endl;
    }
}
