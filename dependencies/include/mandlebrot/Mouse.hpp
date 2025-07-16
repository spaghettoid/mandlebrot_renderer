#pragma once

#include "main.h"

class Mouse {
public:
    Mouse();

    void update_delta(Vector2 coords);

    Vector2 current;
    Vector2 past;
    Vector2 delta;

    bool is_holding = false;
    bool is_in_window = true;
};

Mouse::Mouse()
    : current(0.0, 0.0), past(0.0, 0.0), delta(0.0, 0.0)
{
}

void Mouse::update_delta(Vector2 coords){
    delta.x = past.x - coords.x;
    delta.y = past.y - coords.y;

    // delta.x = coords.x - past.x;
    // delta.y = coords.y - past.y;
    past.x = coords.x;
    past.y = coords.y;
}