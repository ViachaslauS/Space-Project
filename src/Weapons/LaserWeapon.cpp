#include "ObjectsManager.h"
#include "Weapons/LaserWeapon.h"

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
    m_weaponType = WeaponType::Laser;

    auto laserTexture = LoadTexture("laser.png");
    laserTexture.width = 50;
    laserTexture.height = 50;

    //Configure bullet params
    m_baseProjectile.texture = laserTexture;
    m_baseProjectile.damage = 50.0f;

    m_crosshair.crosshairTexture = LoadTexture("arrows.png");
    m_crosshair.textureRect = CrosshairTextureRect;

    m_texture = LoadTexture("laserWeapon.png");
}

void LaserWeapon::shoot() {
    auto projectile = new Projectile(m_baseProjectile);
    auto w = projectile->texture.width;
    auto h = projectile->texture.height;
    auto w2 = projectile->texture.width * 0.5f;
    auto h2 = projectile->texture.height * 0.5f;
    auto pos = center();
    auto r = projectile->texture.height * 0.15f;
    Vector2 c1 { r + 0.25f * w, 3.0f * r };
    Vector2 c2 { r + w2, 3.0f * r };

    // projectile->setPos(pos + Vector2Normalize(velocity)); ??? was in merge conflict
    projectile->setPos(pos);

    m_objectManager.getPhysics().createCapsuleBody(pos, c1, c2, r, projectile, true);
    auto velocity = getSpeedToEnemy();
    projectile->setVelocity(velocity);
    projectile->setState(Projectile::State::Alive);
    projectile->onDieSignal.add([this, projectile]() {
       m_deleteCandidateProjectiles.push_back(projectile);
    });
    m_projectiles.push_back(projectile);
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
