#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Item.h"
#include "Room.h"
#include "Level.h"
#include "HUD.h"
#include "UpgradeSelectionUI.h"
#include "SettingsMenu.h"

struct WorldItem
{
    Item item;
    int roomIndex;
    bool collected;
    WorldItem(const Item& i, int idx) : item(i), roomIndex(idx), collected(false) {}
};

enum class GameState
{
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER,
    VICTORY
};

class Game
{
public:
    Game();
    void run();

private:
    static constexpr int WIN_WIDTH = 1280;
    static constexpr int WIN_HEIGHT = 720;
    static constexpr int MAX_FLOORS = 3;

    UpgradeSelectionUI m_UpgradeUI;
    sf::RenderWindow m_Window;
    GameState m_State;

    Player m_Player;
    Room m_Room;
    Level m_Level;
    HUD m_HUD;

    std::vector<std::unique_ptr<Enemy>> m_Enemies;
    std::vector<Projectile> m_Projectiles;
    std::vector<WorldItem> m_WorldItems;

    int m_Score;
    int m_HighScore;

    int m_MenuIndex;
    std::vector<std::string> m_MenuItems;
    sf::Text m_Title;
    sf::Text m_Subtitle;
    std::vector<sf::Text> m_MenuTexts;
    sf::Text m_HighScoreMenu;
    sf::Text m_Prompt;

    SettingsMainMenu m_SettingsMainMenu;
    bool m_ShowSettings;

    bool m_NeedRestart;
    float m_TransitionTimer;
    bool m_BossIntroShown;
    bool m_PortalActive;
    int m_PortalRoomIndex;
    sf::Vector2f m_PortalPos;
    sf::CircleShape m_PortalShape;

    bool m_MousePressedPrev;
    bool m_EnterPressedPrev;
    bool m_PausePressedPrev;
    bool m_InteractPressedPrev;

    void processEvents();
    void update(float dt);
    void render();

    void updatePlaying(float dt);
    void drawMenu();
    void drawPlaying();
    void drawOverlay(const std::string& title, const std::string& subtitle, sf::Color titleColor);

    void newGame();
    void loadFloor(int floorNum);
    void loadCurrentRoom();
    void spawnRoomContent();
    void onRoomCleared();

    void resolvePlayerWallCollision(sf::Vector2f desired);
    void checkRoomTransitions();
    void handlePlayerShoot();

    void loadHighScore();
    void saveHighScore();

    void setupMenuTexts();
    void refreshMenuHighlight();

    void showUpgradeUI();               // старый метод, можно оставить
    void checkAndShowUpgradeUI();       // новый метод
};