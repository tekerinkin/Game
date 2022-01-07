//
// Created by Дима Шестаков on 12.12.2021.
//

#pragma once
#include "Unit.h"


class Field {
protected:
    std::vector<std::string>field_;
    size_t size_;
public:
    explicit Field(size_t n);

    ~Field() = default;

public:
    bool is_obstacle(int x, int y) const;

    size_t get_size() const;

    void print() const;

    bool in_field(int x, int y) const;

    bool passable(int x, int y) const;

    std::string get_char(int index) const;

};


