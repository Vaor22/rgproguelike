#pragma once
#include <SFML/Graphics.hpp>

// Снаряд (выпускается игроком или врагами)
class Projectile
{
public:
    Projectile(sf::Vector2f pos, sf::Vector2f velocity, float damage,
               bool fromPlayer, sf::Color color = sf::Color::Yellow);

    void update(float dt);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    bool isExpired() const;
    bool isFromPlayer() const;
    float getDamage() const;
    sf::Vector2f getPosition() const;

    // Пометить снаряд как удалённый (например, после попадания)
    void kill();
    bool isDead() const;

private:
    sf::CircleShape m_Shape;
    sf::Vector2f m_Position;
    sf::Vector2f m_Velocity;
    float m_Damage;
    bool m_FromPlayer;
    bool m_Dead;
    float m_Lifetime;
    static constexpr float MAX_LIFETIME = 3.0f;
};
