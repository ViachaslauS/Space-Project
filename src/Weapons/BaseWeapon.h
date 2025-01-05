#pragma once

#include "raylib.h"
#include "Weapons/Projectile.h"
#include "GameObject.h"

#include <vector>

struct ObjectsManager;
struct CrosshairInfo
{
    Texture crosshairTexture;
    Rectangle textureRect;
};

class BaseWeapon : public GameObject
{
public:
    BaseWeapon(ObjectsManager &om, int teamId);

    const Vector2& getPos() const;
    void setPos(const Vector2& pos);

    virtual void update(float dt);
    virtual void render();

    virtual void renderCrosshair(Vector2 Pos) const;
    virtual bool hasCustomCrosshair() const {
        return false;
    }

    void setActive(bool newActive);
    bool isActiveWeapon() const;

    Texture getWeaponTexture() const;

protected:
    virtual void shoot();
    virtual const Vector2& getNearestEnemyPosition() const;

protected:
    float m_weaponCooldown = 0.0f;
    float m_currentCooldown = 0.0f;

    bool m_autoFire = true;
    bool m_isActive = true;

    Texture m_texture;
    std::vector<Projectile*> m_projectiles;

    Projectile m_baseProjectile;
};
