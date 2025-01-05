#pragma once

#include "BaseShip.h"

class ObjectsManager;

class BaseEnemyShip : public BaseShip
{
public:
    BaseEnemyShip(ObjectsManager &om);

    virtual void initialize() override;

    virtual void update(float dt) override;
    virtual void render() override;
    virtual void onCollision(GameObject* obj) override;

    void setVelocity(const Vector2& velocity);
};
