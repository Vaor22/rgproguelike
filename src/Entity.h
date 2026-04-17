#pragma once
#include <SFML/Graphics.hpp>

// Базовый класс для игровых объектов (игрок, враги)
class Entity
{
protected:
    sf::Vector2f m_Position;
    float m_Speed;
    int m_HP;
    int m_MaxHP;
    bool m_Alive;

public:
    Entity(float x, float y, float speed, int maxHP);
    virtual ~Entity() = default;

    virtual void draw(sf::RenderWindow& window) = 0;
    virtual sf::FloatRect getBounds() const = 0;

    void takeDamage(int damage);
    void heal(int amount);

    bool isAlive() const;
    int getHP() const;
    int getMaxHP() const;
    float getSpeed() const;
    void setSpeed(float speed);
    sf::Vector2f getPosition() const;
    sf::Vector2f getCenter() const;
    void setPosition(float x, float y);
    void setPosition(sf::Vector2f pos);
};
