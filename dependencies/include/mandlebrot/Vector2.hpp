#pragma once

#include "main.h"


class Vector2 {
public:
    Vector2(double x_, double y_);

    double x;
    double y;
};

Vector2::Vector2(double x_, double y_){
    this->x = x_;
    this->y = y_;
}

