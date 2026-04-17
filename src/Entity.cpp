#include "Entity.h"

Entity::Entity(float x, float y, float speed, int maxHP)
    : m_Position(x, y)
    , m_Speed(speed)
    , m_HP(maxHP)
    , m_MaxHP(maxHP)
    , m_Alive(true)
{
}

void Entity::takeDamage(int damage)
{
    if (!m_Alive) return;
    m_HP -= damage;
    if (m_HP <= 0)
    {
        m_HP = 0;
        m_Alive = false;
    }
}

void Entity::heal(int amount)
{
    m_HP += amount;
    if (m_HP > m_MaxHP)
        m_HP = m_MaxHP;
}

bool Entity::isAlive() const { return m_Alive; }
int Entity::getHP() const { return m_HP; }
int Entity::getMaxHP() const { return m_MaxHP; }
float Entity::getSpeed() const { return m_Speed; }
void Entity::setSpeed(float speed) { m_Speed = speed; }

sf::Vector2f Entity::getPosition() const { return m_Position; }

sf::Vector2f Entity::getCenter() const
{
    auto b = getBounds();
    return { b.left + b.width / 2.f, b.top + b.height / 2.f };
}

void Entity::setPosition(float x, float y)
{
    m_Position = { x, y };
}

void Entity::setPosition(sf::Vector2f pos)
{
    m_Position = pos;
}
