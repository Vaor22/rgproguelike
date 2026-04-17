#pragma once
#include "Entity.h"
#include "Projectile.h"
#include "Item.h"
#include <vector>

// Игрок — движение, атака, dash, статы
class Player : public Entity
{
public:
    Player();

    // Сброс состояния при новой игре
    void reset();

    // Обработка ввода (клавиатура): движение + dash
    void handleInput(float dt);

    // Обновление позиции, таймеров
    void update(float dt);

    // Отрисовка
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;

    // Атака — создаёт снаряд в направлении точки worldTarget
    // Возвращает true, если выстрел произошёл (с учётом скорострельности)
    bool tryShoot(sf::Vector2f worldTarget, std::vector<Projectile>& outProjectiles);

    // Получить урон с учётом щита и i-frames
    void damage(int dmg);

    // Применить эффект подобранного предмета
    void applyItem(ItemType type);

    // Характеристики
    float getDamage() const;
    void setDamage(float d);
    int getCoins() const;
    void addCoins(int amount);
    bool hasShield() const;

    // Желаемое смещение (для обработки коллизий в Game)
    sf::Vector2f getDesiredDelta() const;
    // Подтвердить перемещение (с учётом коллизий)
    void applyMovement(sf::Vector2f delta);

    // Список активных предметов (для HUD)
    const std::vector<ItemType>& getItems() const;

    // Состояние dash — для визуального эффекта
    bool isDashing() const;
    bool isInvincible() const;

private:
    sf::RectangleShape m_Shape;
    sf::Vector2f m_Desired;

    // Статы
    float m_Damage;
    float m_FireRate;      // секунд между выстрелами
    float m_FireTimer;
    int m_Coins;

    // Dash
    float m_DashCooldown;
    float m_DashTimer;
    float m_DashDuration;
    float m_DashSpeedMult;
    bool m_IsDashing;
    sf::Vector2f m_DashDir;

    // Невосприимчивость после удара
    float m_IFrameTimer;
    static constexpr float I_FRAME_DURATION = 1.0f;

    // Щит (поглощает один удар)
    bool m_HasShield;

    // Инвентарь (для отображения)
    std::vector<ItemType> m_Items;

    // Ввод — кеширование нажатий
    bool m_Up{}, m_Down{}, m_Left{}, m_Right{};
};
