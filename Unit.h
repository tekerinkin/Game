//
// Created by Дима Шестаков on 30.11.2021.
//

#pragma once

#include <iostream>
#include "Graph.h"


const short fighter_attack_dist = 1;
const short hit_cost = 1;
const short move_cost = 2;


class Unit {
protected:
    int hp_;
    int ini_;
    char avatar_;
public:
    explicit Unit(int hp = 0, int ini = 0) : hp_(hp), ini_(ini) {
        std::cout << "Input character's avatar : ";
        std::cin >> avatar_;
    }

    void process_damage(int strength) {
        hp_ > strength ? hp_ -= strength : hp_ = 0;
    }

    virtual void status_output() const {
        std::cout << "{Player " << avatar_ << "; hp = " << hp_;
        std::cout << "; ini_ = " << ini_ << " }\n\n";
    }

    virtual void unit_action(Unit &opponent, Field* f) = 0;

    virtual std::pair<int, int> get_position() const = 0;

    virtual char get_avatar() const {
        return avatar_;
    }

    virtual bool is_alive() const
    {
        return hp_ != 0;
    }

    virtual int get_initiative() const
    {
        return ini_;
    }
};


class Fighter : public Unit {
private:
    int strength_;
    int endurance_;
    std::pair<int, int> coordinates; //first - abscissa, second - ordinate
public:
    Fighter(int x, int y) :
            strength_(10), endurance_(8), Unit() {
        coordinates = std::make_pair(x, y);
        hp_ = strength_ * endurance_;
        ini_ = endurance_ * 5;
    }

    void hit_opponent(Unit &opponent) const {
        opponent.process_damage(strength_);
    }

    void move(Unit &opponent, Field* f) {
        Graph g(f);
        auto op_pos = opponent.get_position();
        coordinates = g.next_step(coordinates, op_pos);
    }

    void unit_action(Unit &opponent, Field* f) override {
        std::pair<int, int> op_pos = opponent.get_position();
        if (abs(coordinates.first - op_pos.first) <= fighter_attack_dist
            && abs(coordinates.second - op_pos.second) <= fighter_attack_dist) {
            hit_opponent(opponent);
            ini_ -= hit_cost;
        } else {
            move(opponent, f);
            ini_ -= move_cost;
        }
        ini_ += endurance_/2;
    }

    std::pair<int, int> get_position() const override {
        return coordinates;

    }

    char get_avatar() const override
    {
        return this->avatar_;
    }

    bool is_alive() const override
    {
        return this->hp_ != 0;
    }

    void status_output() const override
    {
        std::cout << "{Player " << avatar_ << "; hp = " << hp_;
        std::cout << "; ini_ = " << ini_ << " }\n\n";
    }

    int get_initiative() const override
    {
        return ini_;
    }
};


class Ranger : public Unit
{
private:
    int strength_;
    int endurance_;
    std::pair<int, int> coordinates; //first - abscissa, second - ordinate
public:
    Ranger(int x, int y) :
            strength_(6), endurance_(6), Unit() {
        coordinates = std::make_pair(x, y);
        hp_ = strength_ * endurance_;
        ini_ = endurance_ * 5;
    }

    void hit_opponent(Unit &opponent) const {
        opponent.process_damage(strength_);
    }


    void unit_action(Unit &opponent, Field* f) override {
        std::pair<int, int> op_pos = opponent.get_position();
        if (abs(coordinates.first - op_pos.first) <= fighter_attack_dist
            && abs(coordinates.second - op_pos.second) <= fighter_attack_dist) {
            if(coordinates.first - op_pos.first)
            {
                if(f->passable(coordinates.first + (-1)*(coordinates.first - op_pos.first),
                              coordinates.second))
                    coordinates.first = coordinates.first + (-1)*(coordinates.first - op_pos.first);
                else
                    if(f->passable(coordinates.first, coordinates.second+1))
                        coordinates.second += 1;
                    else if(f->passable(coordinates.first, coordinates.second-1))
                        coordinates.second -= 1;
            }

            ini_ -= move_cost;
        } else {
            hit_opponent(opponent);
            ini_ -= hit_cost;
        }
        ini_ += endurance_/2;
    }

    std::pair<int, int> get_position() const override {
        return coordinates;

    }

    char get_avatar() const override
    {
        return this->avatar_;
    }

    bool is_alive() const override
    {
        return this->hp_ != 0;
    }

    void status_output() const override
    {
        std::cout << "{Player " << avatar_ << "; hp = " << hp_;
        std::cout << "; ini_ = " << ini_ << " }\n\n";
    }

    int get_initiative() const override
    {
        return ini_;
    }
};
