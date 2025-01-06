#include "Weapons/Gravigun.hpp"
#include "GravityZone.hpp"

#include <string>

#include <external/reasings.h.>

namespace
{
    const char* crosshairPath = "crosshair.png";
    const char* crosshairEdgePath = "crosshair_border.png";
    const char* arrowPath = "arrow.png";
}

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

    m_crosshairGravi = LoadTexture(crosshairPath);
    m_crosshairGraviBack = LoadTexture(crosshairEdgePath);
    m_arrowTexture = LoadTexture(arrowPath);
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

    // todo: ?
    // m_crosshair.render();

    pos.x -= m_crosshairGravi.width * 0.5f;
    pos.y -= m_crosshairGravi.height * 0.5f;

    DrawTextureV(m_crosshairGravi, pos, WHITE);

    if (canShoot())
    {
        DrawTextureV(m_crosshairGraviBack, pos, WHITE);
    }
    else
    {
        auto reloadProgress = getReloadProgress();

        constexpr float step1_Duration = 0.1f;
        constexpr float step2_Duration = 0.8f;
        constexpr float step3_Duration = 0.1f;

        static_assert(step1_Duration + step2_Duration + step3_Duration == 1.0f);

        const Rectangle bordersRect
        {
            0.0f, 0.0f, m_crosshairGraviBack.width, m_crosshairGraviBack.height
        };

        // first step - decrease scale
        if (reloadProgress < step1_Duration)
        {
            const float internalProgress = reloadProgress / step1_Duration;

            const float converterdProgress = 1.0f - EaseBackIn(internalProgress, 0.0f, 1.0f, 1.0f);

            const Rectangle borderPos
            {
                pos.x + m_crosshairGravi.width * 0.5f,
                pos.y + m_crosshairGravi.height * 0.5f,
                bordersRect.width * converterdProgress,
                bordersRect.height * converterdProgress
            };

            DrawTexturePro(m_crosshairGraviBack, bordersRect, borderPos, { borderPos.width * 0.5f, borderPos.height * 0.5f }, 0.0f, WHITE);
        }
        // second step - rotate and increase scale
        else if (reloadProgress < step1_Duration + step2_Duration)
        {
            const float internalProgress = (reloadProgress - step1_Duration) / step2_Duration;

            const float converterdProgress = EaseCircOut(internalProgress, 0.0f, 1.0f, 1.0f);

            const Rectangle borderPos
            {
                pos.x + m_crosshairGravi.width * 0.5f,
                pos.y + m_crosshairGravi.height * 0.5f,
                bordersRect.width * converterdProgress,
                bordersRect.height * converterdProgress
            };

            DrawTexturePro(m_crosshairGraviBack, bordersRect, borderPos, { borderPos.width * 0.5f, borderPos.height * 0.5f }, converterdProgress * (360.0f * 4.0f), WHITE);
        }
        // third step - show end
        else
        {
            const float internalProgress = (reloadProgress - step1_Duration - step2_Duration) / step3_Duration;  
            const float converterdProgress = 1.0f + helpers::lerpTudaSuda(internalProgress) * 0.1f;

            const Rectangle borderPos
            {
                pos.x + m_crosshairGravi.width * 0.5f,
                pos.y + m_crosshairGravi.height * 0.5f,
                bordersRect.width * converterdProgress,
                bordersRect.height * converterdProgress
            };

            DrawTexturePro(m_crosshairGraviBack, bordersRect, borderPos, { borderPos.width * 0.5f, borderPos.height * 0.5f }, 0.0f, WHITE);
        }
    }

    // draw arrow info
    {
        const Vector2 ScreenSize{ GetScreenWidth(), GetScreenHeight() };

        const std::string textInfo = std::string("wasd / arrows to\nselect gravizone direction");
        const float fontSize = 25.0f;

        const Rectangle arrowRectBase
        {
            0.0f, 0.0f,
            m_arrowTexture.width, m_arrowTexture.height
        };

        const Rectangle arrowRectDest
        {
            ScreenSize.x - std::max(m_arrowTexture.width, m_arrowTexture.height) * 0.7f,
            ScreenSize.y - std::max(m_arrowTexture.width, m_arrowTexture.height) * 0.7f,

            arrowRectBase.width, arrowRectBase.height
        };

        const Vector2 textPos
        {
            arrowRectDest.x - 16.0f * fontSize,
            ScreenSize.y - fontSize * 2.0f - 25.0f
        };

        float angle = 0.0f;
        switch (m_currDirection)
        {
        case GravityZone::Direction::Left: angle = 90.0f; break;
        case GravityZone::Direction::Top: angle = 180.0f; break;
        case GravityZone::Direction::Right: angle = -90.0f; break;
        case GravityZone::Direction::Down: angle = 0.0f; break;
        }

        DrawText(textInfo.c_str(), textPos.x, textPos.y, fontSize, WHITE);

        DrawTexturePro(m_arrowTexture, arrowRectBase, arrowRectDest, { m_arrowTexture.width * 0.5f, m_arrowTexture.height * 0.5f }, angle, WHITE);
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

    m_currCooldown = 0.0f;
}
