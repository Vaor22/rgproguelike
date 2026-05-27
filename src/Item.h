#pragma once
#include <SFML/Graphics.hpp>
#include <string>

enum class ItemType
{
    HEALTH,
    DAMAGE_UP,
    SPEED_UP,
    SHIELD,
    COIN
};

class Item
{
public:
    Item(ItemType type, sf::Vector2f position, int cost = 0);
    Item(const Item& other);
    ~Item() = default;

    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const;
    ItemType getType() const;
    bool isCollected() const;
    void collect();
    int getCost() const;
    sf::Vector2f getPosition() const { return m_Position; }

    static std::string getDescription(ItemType type);
    static sf::Color getColor(ItemType type);
    static std::string getLabel(ItemType type);

    void updateAnimation(float dt); // для анимации монетки (вращение + покачивание)

private:
    ItemType m_Type;
    sf::Vector2f m_Position;
    sf::RectangleShape m_Shape;   // для обычных предметов
    sf::Sprite m_Sprite;          // для монетки
    sf::Text m_Label;
    bool m_Collected;
    int m_Cost;

    float m_RotationAngle;
    float m_BobOffset;
};