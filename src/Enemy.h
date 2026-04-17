#pragma once
#include "Entity.h"
#include "Projectile.h"
#include <vector>

// Типы врагов
enum class EnemyType
{
    SLIME,      // движется к игроку, урон при касании
    SHOOTER,    // держит дистанцию, стреляет
    BERSERKER,  // быстро бросается к игроку
    BOSS        // финальная комната, несколько фаз
};

// Враг — базовая реализация с разным поведением по типу
class Enemy : public Entity
{
public:
    Enemy(EnemyType type, float x, float y, int difficulty = 1);

    // Обновление поведения (ИИ). Может добавить снаряды в out.
    void update(float dt, sf::Vector2f playerPos, std::vector<Projectile>& out);

    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;

    EnemyType getType() const;
    float getDamage() const;
    int getScoreValue() const;

    // Контактный урон — наносит ли урон при касании с игроком
    bool canContactDamage() const;

    // Вспышка урона (после попадания)
    void flashHit();

private:
    EnemyType m_Type;
    sf::CircleShape m_Shape;
    float m_Damage;
    float m_DetectionRadius;
    float m_AttackCooldown;
    float m_AttackTimer;
    float m_HitFlashTimer;

    // Для берсерка — состояние атаки-рывка
    bool  m_Charging;
    float m_ChargeCooldown;
    sf::Vector2f m_ChargeDir;

    // Для босса — номер фазы
    int m_BossPhase;

    // Для шутера — желаемая дистанция до игрока
    float m_PreferredDistance;

    float m_Radius;

    int m_ScoreValue;

    void setupByType(int difficulty);
};
