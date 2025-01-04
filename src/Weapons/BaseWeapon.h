#pragma once

#include "raylib.h"
#include "Weapons/Projectile.h"
#include "GameObject.h"

#include <vector>

class BaseWeapon : public GameObject
{
public:
    BaseWeapon(int teamId);

public:
    const Vector2& getPos() const;
    void setPos(const Vector2& pos);

    virtual void update(float dt);
    virtual void render();

protected:
    virtual void shoot();
    virtual const Vector2& getNearestEnemyPosition() const;

protected:
    float m_weaponCooldown = 0.0f;
    float m_currentCooldown = 0.0f;

    Texture m_texture;
    std::vector<Projectile*> m_projectiles;

    Projectile m_baseProjectile;
};