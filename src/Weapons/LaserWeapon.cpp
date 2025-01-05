#include "Weapons/LaserWeapon.h"

#include <raymath.h>

namespace
{
    constexpr Rectangle CrosshairTextureRect
    {
        96, 208,
        32, 32
    };
}

LaserWeapon::LaserWeapon(ObjectsManager& om, int teamId)
    : BaseWeapon(om, teamId, Projectile(om, teamId, ObjectType::LaserProjectile))
{
    auto laserTexture = LoadTexture("laser.png");
    laserTexture.height = 50;
    laserTexture.width = 50;
    m_weaponCooldown = 1.5f;

    //Configure bullet params
    m_baseProjectile.texture = laserTexture;
    m_baseProjectile.damage = 50.0f;

    m_crosshair.crosshairTexture = LoadTexture("arrows.png");
    m_crosshair.textureRect = CrosshairTextureRect;
}

void LaserWeapon::update(float dt)
{
    BaseWeapon::update(dt);
}

void LaserWeapon::renderCrosshair(Vector2 pos) const
{
    const Vector2 TextureSize = { m_crosshair.textureRect.width, m_crosshair.textureRect.height };
    const Vector2 renderPos = Vector2Add(pos, Vector2Scale(TextureSize, -0.5f));

    DrawTextureRec(m_crosshair.crosshairTexture, m_crosshair.textureRect, renderPos, WHITE);
}
