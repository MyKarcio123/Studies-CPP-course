#pragma once
class GameObject {
public:
    virtual void start() = 0;
    virtual void update() = 0;
    virtual ~GameObject() {};
};