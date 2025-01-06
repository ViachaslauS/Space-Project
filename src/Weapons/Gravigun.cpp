#include "Weapons/Gravigun.hpp"
#include "GravityZone.hpp"

Gravigun::Gravigun(ObjectsManager& om, int teamId, GravityZoneSystem &gz)
    : BaseWeapon(om, teamId, Projectile(om, teamId, ObjectType::RocketProjectile))
    , m_gravityZones(gz)
    , m_crosshair({ 0.0f, 0.0f }, { 0.0f, 0.0f }, GravityZone::Direction::Right)
{
    m_autoFire = false;
    m_isActive = false;
    m_weaponType = WeaponType::Gravigun;
    m_isManualControlAvailable = true;

    m_iconTexture = LoadTexture("hud/gravigun_ic.png");

    auto rocketTexture = LoadTexture("rocket.png");
    rocketTexture.height = 50;
    rocketTexture.width = 50;

    //Configure bullet params
    m_baseProjectile.texture = rocketTexture;

    m_bounds.x = 0.0f;
    m_bounds.y = 0.0f;

    m_texture = LoadTexture("gravigunWeapon.png");

    setDirection(GravityZone::Direction::Right);
}

void Gravigun::update(float dt)
{
    BaseWeapon::update(dt);

    if (m_isActive)
    {
        handleInput();

        Vector2 mouse = GetMousePosition();

        m_bounds.x = mouse.x - m_bounds.width * 0.5f;
        m_bounds.y = mouse.y - m_bounds.height * 0.5f;

        auto center = helpers::windowCenter();
        auto dir = Vector2Subtract(mouse, center);

        m_weaponAngle = helpers::vecToAngle(dir);

        m_crosshair.bounds = m_bounds;
        m_crosshair.particles.setBounds(m_bounds);
        m_crosshair.update(dt);
    }
}

void Gravigun::renderCrosshair(Vector2 pos) const
{
    if (!m_isActive)
    {
        return;
    }

    m_crosshair.render();

    if (!canShoot())
    {
        /// 
    }
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

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_SPACE))
    {
        shoot();
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

    m_crosshair.particles.setDirection(m_currDirection);
}

void Gravigun::shoot()
{
    if (!canShoot())
    {
        return;
    }

    auto pos = GetMousePosition();
    m_gravityZones.addZone(pos, m_currDirection, m_params.power);
}
