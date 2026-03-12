#pragma once

class Timer
{
public:
    void start_timer();
    void update(float dt);
    float get_time();
private:
    bool started_ = false;
    float elapsed_time_ = 0.0f;
};
