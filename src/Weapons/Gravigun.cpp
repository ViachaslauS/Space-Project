#include "Weapons/Gravigun.hpp"
#include "GravityZone.hpp"

namespace
{
    const char* GravizoneBGNPPath = "gravizone_bg_np.png";
    const NPatchInfo GravizoneNPatch =
    {
        .source{ 0, 0, 5, 1 },
        .left{0},
        .top{0},
        .right{0},
        .bottom{0},
        .layout{NPATCH_NINE_PATCH}
    };
}

Gravigun::Gravigun(ObjectsManager& om, int teamId, GravityZoneSystem &gz)
    : BaseWeapon(om, teamId)
    , m_gravityZones(gz)
{
    m_autoFire = false;
    m_isActive = false;

    auto rocketTexture = LoadTexture("rocket.png");
    rocketTexture.height = 50;
    rocketTexture.width = 50;
    m_weaponCooldown = 4.0f;
    //Configure bullet params
    m_baseProjectile = {
        rocketTexture,
        Vector2{0,0},
        Vector2{0,0},
        100.0f,
        teamId,
        Projectile::State::Unused,
    };

    m_gravizoneBG = LoadTexture(GravizoneBGNPPath);

    m_bounds.x = 0.0f;
    m_bounds.y = 0.0f;

    setDirection(GravityZone::Direction::Right);
}

void Gravigun::update(float dt)
{
    BaseWeapon::update(dt);

    if (m_isActive)
    {
        handleInput();

        m_bounds.x = GetMousePosition().x - m_bounds.width * 0.5f;
        m_bounds.y = GetMousePosition().y - m_bounds.height * 0.5f;

        m_particles.setBounds(m_bounds);
        m_particles.update(dt);
    }

}

void Gravigun::renderCrosshair(Vector2 pos) const
{
    if (!m_isActive)
    {
        return;
    }

    m_particles.render();
    DrawTextureNPatch(m_gravizoneBG, GravizoneNPatch, m_bounds, {}, 0, WHITE);
}

void Gravigun::setParams(Params params)
{
    m_params = params;
}

Gravigun::Params Gravigun::getParams() const
{
    return m_params;
}

void Gravigun::handleInput()
{
    if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))
    {
        setDirection(GravityZone::Direction::Left);
    }
    else if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))
    {
        setDirection(GravityZone::Direction::Top);
    }
    else if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT))
    {
        setDirection(GravityZone::Direction::Right);
    }
    else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))
    {
        setDirection(GravityZone::Direction::Down);
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
    {
        auto pos = GetMousePosition();
        m_gravityZones.addZone(pos, m_currDirection, 3.0f, m_bounds.width, m_bounds.height);
    }
}

void Gravigun::setDirection(GravityZone::Direction newDirection)
{
    m_currDirection = newDirection;

    switch (m_currDirection)
    {
    case GravityZone::Direction::Top:
        m_bounds.width = m_params.size.x;
        m_bounds.height = m_params.size.y;
        break;
    case GravityZone::Direction::Right:
        m_bounds.width = m_params.size.y;
        m_bounds.height = m_params.size.x;
        break;
    case GravityZone::Direction::Down:
        m_bounds.width = m_params.size.x;
        m_bounds.height = m_params.size.y;
        break;
    case GravityZone::Direction::Left:
        m_bounds.width = m_params.size.y;
        m_bounds.height = m_params.size.x;
        break;
    default:
        break;
    }

    m_particles.changeDirection(newDirection);
}
