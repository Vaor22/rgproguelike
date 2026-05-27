#include "Item.h"
#include "AssetManager.h"
#include <cmath>

Item::Item(ItemType type, sf::Vector2f position, int cost)
    : m_Type(type)
    , m_Position(position)
    , m_Collected(false)
    , m_Cost(cost)
    , m_RotationAngle(0.f)
    , m_BobOffset(0.f)
{
    float size = (type == ItemType::COIN) ? 24.f : 30.f;

    if (type == ItemType::COIN) {
        const sf::Texture* tex = AssetManager::instance().getTexture("money");
        if (tex) {
            m_Sprite.setTexture(*tex);
            sf::Vector2u texSize = tex->getSize();
            float scale = size / texSize.x;
            m_Sprite.setScale(scale, scale);
            m_Sprite.setOrigin(texSize.x / 2.f, texSize.y / 2.f);
            m_Sprite.setPosition(m_Position);
        }
        else {
            // fallback: прямоугольник, если текстура не загружена
            m_Shape.setSize({ size, size });
            m_Shape.setOrigin(size / 2.f, size / 2.f);
            m_Shape.setFillColor(getColor(type));
            m_Shape.setOutlineColor(sf::Color::White);
            m_Shape.setOutlineThickness(1.f);
            m_Shape.setPosition(m_Position);
        }
    }
    else {
        m_Shape.setSize({ size, size });
        m_Shape.setOrigin(size / 2.f, size / 2.f);
        m_Shape.setFillColor(getColor(type));
        m_Shape.setOutlineColor(sf::Color::White);
        m_Shape.setOutlineThickness(1.f);
        m_Shape.setPosition(m_Position);
    }

    if (AssetManager::instance().isFontLoaded()) {
        const auto& font = AssetManager::instance().getFont();
        m_Label.setFont(font);
        m_Label.setString(getLabel(type));
        m_Label.setCharacterSize(type == ItemType::COIN ? 14 : 16);
        m_Label.setFillColor(sf::Color::White);
        auto lb = m_Label.getLocalBounds();
        m_Label.setOrigin(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f);
        m_Label.setPosition(m_Position);
    }
}

Item::Item(const Item& other)
    : m_Type(other.m_Type)
    , m_Position(other.m_Position)
    , m_Collected(other.m_Collected)
    , m_Cost(other.m_Cost)
    , m_RotationAngle(0.f)
    , m_BobOffset(0.f)
{
    float size = (m_Type == ItemType::COIN) ? 24.f : 30.f;

    if (m_Type == ItemType::COIN) {
        const sf::Texture* tex = AssetManager::instance().getTexture("money");
        if (tex) {
            m_Sprite.setTexture(*tex);
            sf::Vector2u texSize = tex->getSize();
            float scale = size / texSize.x;
            m_Sprite.setScale(scale, scale);
            m_Sprite.setOrigin(texSize.x / 2.f, texSize.y / 2.f);
            m_Sprite.setPosition(m_Position);
        }
        else {
            m_Shape.setSize({ size, size });
            m_Shape.setOrigin(size / 2.f, size / 2.f);
            m_Shape.setFillColor(getColor(m_Type));
            m_Shape.setOutlineColor(sf::Color::White);
            m_Shape.setOutlineThickness(1.f);
            m_Shape.setPosition(m_Position);
        }
    }
    else {
        m_Shape.setSize({ size, size });
        m_Shape.setOrigin(size / 2.f, size / 2.f);
        m_Shape.setFillColor(getColor(m_Type));
        m_Shape.setOutlineColor(sf::Color::White);
        m_Shape.setOutlineThickness(1.f);
        m_Shape.setPosition(m_Position);
    }

    if (AssetManager::instance().isFontLoaded()) {
        const auto& font = AssetManager::instance().getFont();
        m_Label.setFont(font);
        m_Label.setString(getLabel(m_Type));
        m_Label.setCharacterSize(m_Type == ItemType::COIN ? 14 : 16);
        m_Label.setFillColor(sf::Color::White);
        auto lb = m_Label.getLocalBounds();
        m_Label.setOrigin(lb.left + lb.width / 2.f, lb.top + lb.height / 2.f);
        m_Label.setPosition(m_Position);
    }
}

void Item::updateAnimation(float dt) {
    if (m_Type != ItemType::COIN) return;
    // Вращение монетки
    m_RotationAngle += dt * 180.f; // 180 градусов в секунду
    if (m_RotationAngle >= 360.f) m_RotationAngle -= 360.f;
    // Покачивание вверх-вниз
    static float time = 0.f;
    time += dt;
    m_BobOffset = std::sin(time * 5.f) * 3.f;
}

void Item::draw(sf::RenderWindow& window) {
    if (m_Collected) return;

    if (m_Type == ItemType::COIN) {
        if (m_Sprite.getTexture()) {
            m_Sprite.setPosition(m_Position.x, m_Position.y + m_BobOffset);
            m_Sprite.setRotation(m_RotationAngle);
            window.draw(m_Sprite);
        }
        else {
            m_Shape.setPosition(m_Position.x, m_Position.y + m_BobOffset);
            window.draw(m_Shape);
        }
        if (AssetManager::instance().isFontLoaded()) {
            m_Label.setPosition(m_Position.x, m_Position.y + m_BobOffset);
            window.draw(m_Label);
        }
    }
    else {
        m_Shape.setPosition(m_Position);
        window.draw(m_Shape);
        if (AssetManager::instance().isFontLoaded()) {
            m_Label.setPosition(m_Position);
            window.draw(m_Label);
        }
    }
}

sf::FloatRect Item::getBounds() const {
    if (m_Type == ItemType::COIN && m_Sprite.getTexture()) {
        return m_Sprite.getGlobalBounds();
    }
    else {
        return m_Shape.getGlobalBounds();
    }
}

ItemType Item::getType() const { return m_Type; }
bool Item::isCollected() const { return m_Collected; }
void Item::collect() { m_Collected = true; }
int Item::getCost() const { return m_Cost; }

std::string Item::getDescription(ItemType type) {
    static const char* desc[] = { "HP +30", "DMG +10", "SPD +50", "Shield", "Coin" };
    return desc[static_cast<int>(type)];
}

sf::Color Item::getColor(ItemType type) {
    static const sf::Color colors[] = {
        sf::Color(50, 180, 50),   // HEALTH
        sf::Color(200, 50, 50),   // DAMAGE_UP
        sf::Color(50, 100, 220),  // SPEED_UP
        sf::Color(50, 200, 200),  // SHIELD
        sf::Color(240, 200, 40)   // COIN
    };
    return colors[static_cast<int>(type)];
}

std::string Item::getLabel(ItemType type) {
    static const char* labels[] = { "+", "D", "S", "O", "$" };
    return labels[static_cast<int>(type)];
}