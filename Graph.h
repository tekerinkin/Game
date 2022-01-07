//
// Created by Дима Шестаков on 30.11.2021.
//

#pragma once

#include <array>
#include <queue>
#include <map>
#include "globals.h"


using position = std::pair<int, int>;
const int cost = 1;

struct SquareGrid {
protected:
    std::array<position, 4> DIRS{position{1, 0}, position{0, 1},
                                 position{0, -1}, position{-1, 0}};

public:
    SquareGrid() = default;

    bool in_bounds(position pos, Field* f) const {
        return 0 <= pos.first && pos.first < f->get_size()
               && 0 <= pos.second && pos.second < f->get_size();
    }

    bool passable(position pos, Field* f) const {
        return !f->is_obstacle(pos.first, pos.second);
    }

    std::vector<position> neighbours(position pos, Field* f) const {
        auto[x, y] = pos;
        std::vector<position> results;
        for (auto[dx, dy] : DIRS) {
            position next = std::make_pair(x + dx, y + dy);
            if (in_bounds(next, f) && passable(next, f))
                results.push_back(next);
        }

        if ((x + y) % 2 == 0)
            std::reverse(begin(results), end(results));

        return results;
    }
};


template<class T, class Priority_T>
struct PriorityQueue {
private:
    using element = std::pair<Priority_T, T>;
    std::priority_queue<element, std::vector<element>,
            std::greater<element>> elements;

public:
    bool empty() const {
        return elements.empty();
    }

    void push(T item, Priority_T priority) {
        elements.emplace(priority, item);
    }

    T pop() {
        T best_tem = elements.top().second;
        elements.pop();
        return best_tem;
    }
};



class Graph
{
private:
    std::map<position, position> came_from;
    std::map<position, position> cost_so_far;
    SquareGrid sg_;
    Field* f_;
public:
    Graph(Field* f)
    {
        f_ = f;
    }
    void dijkstra_search(const SquareGrid &graph, position start, position goal,
                         std::map<position, position> &came_from,
                         std::map<position, int> &cost_so_far, Field* f) {
        PriorityQueue<position, int> queue;
        queue.push(start, 0);

        came_from[start] = start;
        cost_so_far[start] = 0;

        while (!queue.empty()) {
            auto curr = queue.pop();

            if (curr == goal)
                break;

            for (auto &next : graph.neighbours(curr, f)) {
                int new_cost = cost_so_far[curr] + cost;
                if (!cost_so_far.count(next) || new_cost < cost_so_far[next]) {
                    cost_so_far[next] = new_cost;
                    came_from[next] = curr;
                    queue.push(next, new_cost);
                }
            }
        }
    }

    std::vector<position> reconstruct_path(position start, position goal,
                                           std::map<position, position> &came_from) {
        std::vector<position> path;
        position curr = goal;
        path.push_back(curr);
        while (curr != start) {
            curr = came_from[curr];
            path.push_back(curr);
        }
        std::reverse(begin(path), end(path));
        return path;
    }

    position next_step(position& start, position& goal)
    {
        return reconstruct_path(start, goal, came_from)[0];
    }
};
