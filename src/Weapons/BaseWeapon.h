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

enum class WeaponType
{
    Gravigun,
    Laser,
    RocketLauncher,

    Count
};

class BaseWeapon : public GameObject
{
public:
    BaseWeapon(ObjectsManager &om, int teamId, Projectile baseProjectile);
    ~BaseWeapon();

    struct WeaponParam
    {
        float weaponCooldown = 1.0f;
        float weaponDamage = 1.0f;
    };

    void applyParams(WeaponParam newParams);

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

    bool isManualControlAvailable() const;

    Texture getWeaponTexture() const;
    Texture getWeaponIcon() const;

    WeaponType getWeaponType() const;


protected:
    virtual void shoot();
    virtual const Vector2 getSpeedToEnemy();
    void calculateDirAngle(const Vector2 dir);

protected:
    WeaponType m_weaponType = WeaponType::Count;

    float m_currCooldown = 0.0f;

    WeaponParam m_params;

    bool m_isManualControlAvailable = false;
    bool m_autoFire = true;
    bool m_isActive = false;

    Texture m_texture;
    Texture m_iconTexture;

    std::vector<Projectile*> m_projectiles;
    std::vector<Projectile*> m_deleteCandidateProjectiles;

    Projectile m_baseProjectile;

    float m_weaponAngle = 0.0f;;
};
