#include "Enemy.h"
#include <cmath>

Enemy::Enemy(EnemyType type, float x, float y, int difficulty)
    : Entity(x, y, 0.f, 1)
    , m_Type(type)
    , m_Damage(0.f)
    , m_DetectionRadius(0.f)
    , m_AttackCooldown(1.f)
    , m_AttackTimer(0.f)
    , m_HitFlashTimer(0.f)
    , m_Charging(false)
    , m_ChargeCooldown(0.f)
    , m_ChargeDir(0.f, 0.f)
    , m_BossPhase(1)
    , m_PreferredDistance(250.f)
    , m_Radius(20.f)
    , m_ScoreValue(10)
{
    setupByType(difficulty);
    m_Shape.setRadius(m_Radius);
    m_Shape.setOrigin(m_Radius, m_Radius);
    m_Shape.setPosition(m_Position);

    sf::Color fillColor, outlineColor;
    switch (m_Type)
    {
    case EnemyType::SLIME:
        fillColor = sf::Color(160, 60, 200);
        outlineColor = sf::Color(80, 30, 120);
        break;
    case EnemyType::SHOOTER:
        fillColor = sf::Color(230, 140, 30);
        outlineColor = sf::Color(140, 80, 10);
        break;
    case EnemyType::BERSERKER:
        fillColor = sf::Color(180, 40, 40);
        outlineColor = sf::Color(90, 10, 10);
        break;
    case EnemyType::BOSS:
        fillColor = sf::Color(150, 0, 60);
        outlineColor = sf::Color(255, 220, 40);
        break;
    }
    m_Shape.setFillColor(fillColor);
    m_Shape.setOutlineColor(outlineColor);
    m_Shape.setOutlineThickness(2.f);
}

void Enemy::setupByType(int difficulty)
{
    float diffMul = 1.f + 0.25f * static_cast<float>(difficulty - 1);

    switch (m_Type)
    {
    case EnemyType::SLIME:
        m_MaxHP = static_cast<int>(60 * diffMul);
        m_HP = m_MaxHP;
        m_Speed = 95.f;
        m_Damage = 12.f * diffMul;
        m_DetectionRadius = 450.f;
        m_AttackCooldown = 0.f;
        m_Radius = 20.f;
        m_ScoreValue = 10;
        break;
    case EnemyType::SHOOTER:
        m_MaxHP = static_cast<int>(45 * diffMul);
        m_HP = m_MaxHP;
        m_Speed = 70.f;
        m_Damage = 8.f * diffMul;
        m_DetectionRadius = 520.f;
        m_AttackCooldown = 1.5f;
        m_PreferredDistance = 260.f;
        m_Radius = 18.f;
        m_ScoreValue = 15;
        break;
    case EnemyType::BERSERKER:
        m_MaxHP = static_cast<int>(85 * diffMul);
        m_HP = m_MaxHP;
        m_Speed = 165.f;
        m_Damage = 22.f * diffMul;
        m_DetectionRadius = 400.f;
        m_AttackCooldown = 2.0f;
        m_Radius = 24.f;
        m_ScoreValue = 20;
        break;
    case EnemyType::BOSS:
        m_MaxHP = static_cast<int>(400 * diffMul);
        m_HP = m_MaxHP;
        m_Speed = 80.f;
        m_Damage = 20.f * diffMul;
        m_DetectionRadius = 1000.f;
        m_AttackCooldown = 1.8f;
        m_Radius = 42.f;
        m_ScoreValue = 250;
        break;
    }
}

void Enemy::update(float dt, sf::Vector2f playerPos, std::vector<Projectile>& out)
{
    if (!m_Alive) return;

    if (m_HitFlashTimer > 0.f)
    {
        m_HitFlashTimer -= dt;
    }

    if (m_AttackTimer > 0.f)  m_AttackTimer -= dt;
    if (m_ChargeCooldown > 0.f) m_ChargeCooldown -= dt;

    sf::Vector2f diff = playerPos - m_Position;
    float dist = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    if (dist > m_DetectionRadius && m_Type != EnemyType::BOSS)
        return; // вне радиуса — не реагирует

    sf::Vector2f dir(0.f, 0.f);
    if (dist > 1e-4f) dir = diff / dist;

    switch (m_Type)
    {
    case EnemyType::SLIME:
        // Идёт к игроку
        m_Position += dir * m_Speed * dt;
        break;

    case EnemyType::SHOOTER:
        // Держит дистанцию: приближается если далеко, отступает если близко
        if (dist > m_PreferredDistance + 30.f)
            m_Position += dir * m_Speed * dt;
        else if (dist < m_PreferredDistance - 30.f)
            m_Position -= dir * m_Speed * dt;
        // Стрельба
        if (m_AttackTimer <= 0.f && dist < m_DetectionRadius)
        {
            const float PROJ_SPEED = 340.f;
            out.emplace_back(m_Position, dir * PROJ_SPEED, m_Damage, false,
                             sf::Color(255, 120, 40));
            m_AttackTimer = m_AttackCooldown;
        }
        break;

    case EnemyType::BERSERKER:
        // Рывки: ждёт, затем быстро бросается
        if (m_Charging)
        {
            m_Position += m_ChargeDir * (m_Speed * 1.4f) * dt;
            m_AttackTimer -= dt;
            if (m_AttackTimer <= 0.f)
            {
                m_Charging = false;
                m_ChargeCooldown = m_AttackCooldown;
            }
        }
        else
        {
            // Медленное преследование
            m_Position += dir * (m_Speed * 0.4f) * dt;
            if (m_ChargeCooldown <= 0.f && dist < m_DetectionRadius)
            {
                m_Charging = true;
                m_ChargeDir = dir;
                m_AttackTimer = 0.6f; // длительность рывка
            }
        }
        break;

    case EnemyType::BOSS:
    {
        // Фазы по HP
        m_BossPhase = (m_HP > m_MaxHP / 2) ? 1 : 2;
        // Движение — медленное преследование
        m_Position += dir * m_Speed * dt;

        if (m_AttackTimer <= 0.f)
        {
            int projCount = (m_BossPhase == 1) ? 3 : 5;
            float spread = (m_BossPhase == 1) ? 0.4f : 0.8f;
            const float PROJ_SPEED = 320.f;
            for (int i = 0; i < projCount; ++i)
            {
                float t = (projCount == 1) ? 0.f
                    : (static_cast<float>(i) / (projCount - 1) - 0.5f) * spread;
                float c = std::cos(t), s = std::sin(t);
                sf::Vector2f d{ dir.x * c - dir.y * s, dir.x * s + dir.y * c };
                out.emplace_back(m_Position, d * PROJ_SPEED, m_Damage * 0.8f,
                                 false, sf::Color(255, 80, 100));
            }
            m_AttackTimer = (m_BossPhase == 1) ? 2.0f : 1.2f;
        }
        break;
    }
    }

    m_Shape.setPosition(m_Position);

    // Цвет — вспышка при попадании
    sf::Color normal;
    switch (m_Type)
    {
    case EnemyType::SLIME:    normal = sf::Color(160, 60, 200); break;
    case EnemyType::SHOOTER:  normal = sf::Color(230, 140, 30); break;
    case EnemyType::BERSERKER:normal = m_Charging ? sf::Color(255, 80, 80)
                                                   : sf::Color(180, 40, 40); break;
    case EnemyType::BOSS:     normal = (m_BossPhase == 2) ? sf::Color(200, 20, 80)
                                                           : sf::Color(150, 0, 60); break;
    }
    if (m_HitFlashTimer > 0.f)
        m_Shape.setFillColor(sf::Color(255, 255, 255));
    else
        m_Shape.setFillColor(normal);
}

void Enemy::draw(sf::RenderWindow& window)
{
    if (!m_Alive) return;
    window.draw(m_Shape);

    // Полоска HP над врагом (для берсерка и босса)
    if (m_Type == EnemyType::BOSS || m_Type == EnemyType::BERSERKER)
    {
        float barWidth = m_Radius * 2.f;
        float barHeight = 5.f;
        sf::RectangleShape bg({ barWidth, barHeight });
        bg.setFillColor(sf::Color(30, 30, 30, 200));
        bg.setPosition(m_Position.x - m_Radius, m_Position.y - m_Radius - 10.f);
        window.draw(bg);

        float frac = static_cast<float>(m_HP) / static_cast<float>(m_MaxHP);
        sf::RectangleShape fg({ barWidth * frac, barHeight });
        fg.setFillColor(sf::Color(220, 60, 60));
        fg.setPosition(m_Position.x - m_Radius, m_Position.y - m_Radius - 10.f);
        window.draw(fg);
    }
}

sf::FloatRect Enemy::getBounds() const
{
    return sf::FloatRect(m_Position.x - m_Radius, m_Position.y - m_Radius,
                         m_Radius * 2.f, m_Radius * 2.f);
}

EnemyType Enemy::getType() const { return m_Type; }
float Enemy::getDamage() const { return m_Damage; }
int Enemy::getScoreValue() const { return m_ScoreValue; }

bool Enemy::canContactDamage() const
{
    // Все, кроме шутера, наносят урон при касании
    return m_Type != EnemyType::SHOOTER;
}

void Enemy::flashHit()
{
    m_HitFlashTimer = 0.1f;
}
