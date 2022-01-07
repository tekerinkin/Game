//
// Created by Дима Шестаков on 30.11.2021.
//

#pragma once

#include <vector>
#include <random>
#include <cstdlib>
#include "Unit.h"


using position = std::pair<int, int>;


template<typename Prob = double>
bool binomial_trial(const Prob p = 0.5) {
    static auto dev = std::random_device();
    static auto gen = std::mt19937{dev()};
    static auto dist = std::uniform_real_distribution<Prob>(0, 1);
    return (dist(gen) < p);
}

int random_num(size_t n) {
    static auto dev = std::random_device();
    static auto gen = std::mt19937{dev()};
    static auto dist = std::uniform_int_distribution<int>(4, n - 2);
    return dist(gen);
}


Field::Field(size_t n) : size_(n) {
    field_.resize(n * n);
    int cnt = 0; // Counter for obstacles;
    int num_of_obstacles = random_num(n);
    for (size_t i = 0; i < n * n; ++i) {
        if (cnt == num_of_obstacles)
            field_[i] = "░";
        else {
            if (binomial_trial(0.5)) {
                field_[i] = "▓";
                cnt++;
            } else
                field_[i] = "░";
        }
    }
}


bool Field::is_obstacle(int x, int y) const {
    return field_[y * size_ + x] == "▓";
}

size_t Field::get_size() const {
    return size_;
}

void Field::print() const {
    int cnt = 0;
    for (int i = 0; i < size_ * size_; ++i) {
        cnt++;
        std::cout << field_[i] << " ";
        if (cnt == size_) {
            std::cout << "\n";
            cnt = 0;
        }
    }
}

bool Field::in_field(int x, int y) const {
    return (x >=0 && x < size_) &&
            (y >= 0 && y < size_);
}

bool Field::passable(int x, int y) const
{
    return !is_obstacle(x, y) && in_field(x, y);
}

std::string Field::get_char(int index) const
{
    return field_[index];
}

void clear() {
    std::cout << "\x1B[2J\x1B[H";
}

void print(const Unit& uni1, const Unit& uni2, const Field& f)
{
    std::cout << "==============================================\n\n" << std::endl;
    int cnt = 0;
    auto [x1, y1] = uni1.get_position();
    auto [x2, y2] = uni2.get_position();
    size_t size = f.get_size();

    for(int i = 0; i < size*size; ++i)
    {
        cnt++;
        if(i == y1*size+x1)
            std::cout << uni1.get_avatar() << " ";
        else if(i == y2*size+x2)
            std::cout << uni2.get_avatar() << " ";
        else
            std::cout << f.get_char(i) << " ";

        if(cnt == size) {
            std::cout << "\n";
            cnt = 0;
        }
    }

    std::cout << "\n";

    uni1.status_output();
    uni2.status_output();

    std::cout << "==============================================\n\n" << std::endl;
}

void game()
{
    Field f(10);
    Fighter fighter(random_num(10), random_num(10));
    Ranger ranger(random_num(10), random_num(10));

    auto cnt = 0;
    char c;
    clear();

    std::cout << "==============================================" << std::endl;
    std::cout << "\n"
                 "░██████╗████████╗░█████╗░██████╗░████████╗\n"
                 "██╔════╝╚══██╔══╝██╔══██╗██╔══██╗╚══██╔══╝\n"
                 "╚█████╗░░░░██║░░░███████║██████╔╝░░░██║░░░\n"
                 "░╚═══██╗░░░██║░░░██╔══██║██╔══██╗░░░██║░░░\n"
                 "██████╔╝░░░██║░░░██║░░██║██║░░██║░░░██║░░░\n"
                 "╚═════╝░░░░╚═╝░░░╚═╝░░╚═╝╚═╝░░╚═╝░░░╚═╝░░░\n";
    std::cout << "==============================================\n\n" << std::endl;

    std::cout << "Input s to start: ";
    std::cin >> c;



    while(cnt < 100 && fighter.is_alive() && ranger.is_alive())
    {
        clear();
        print(fighter, ranger, f);
        if (fighter.get_initiative() > ranger.get_initiative())
        {
            fighter.unit_action(ranger, &f);
            ranger.unit_action(fighter, &f);
        }
        else
        {
            ranger.unit_action(fighter, &f);
            fighter.unit_action(ranger, &f);
        }

        clear();
        print(fighter, ranger, f);
        cnt++;
    }
}
