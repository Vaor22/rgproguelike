#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Level.h"

// Heads-Up Display: HP, счёт, этаж, монеты, иконки предметов, миникарта
class HUD
{
public:
    HUD();

    // Обновить HUD текущими значениями
    void update(const Player& player, int score, int highScore, const Level& level);

    // Отрисовать HUD
    void draw(sf::RenderWindow& window);

    // Сообщение на экране (например, "Press E to buy")
    void setMessage(const std::string& msg, float duration = 2.0f);
    void tickMessage(float dt);

private:
    // HP bar
    sf::RectangleShape m_HpBarBg;
    sf::RectangleShape m_HpBarFill;
    sf::Text m_HpText;

    // Счёт и прочее
    sf::Text m_ScoreText;
    sf::Text m_HighScoreText;
    sf::Text m_FloorText;
    sf::Text m_CoinText;

    // Иконки активных предметов
    std::vector<sf::RectangleShape> m_ItemIcons;
    std::vector<sf::Text> m_ItemLabels;

    // Миникарта
    std::vector<sf::RectangleShape> m_MinimapCells;

    // Сообщение
    sf::Text m_Message;
    float m_MessageTimer;

    bool m_FontReady;
};
