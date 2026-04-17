#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <array>

// Типы комнат
enum class RoomType
{
    START,
    NORMAL,
    ITEM,
    SHOP,
    BOSS
};

// Направление двери
enum class Direction { NORTH = 0, SOUTH = 1, WEST = 2, EAST = 3 };

// Комната — тайловая сетка с дверями
class Room
{
public:
    static constexpr int GRID_WIDTH = 16;
    static constexpr int GRID_HEIGHT = 9;
    static constexpr int TILE_SIZE = 80;

    Room();

    // Сгенерировать комнату заданного типа с указанными дверями
    // doors[i] — есть ли дверь в направлении Direction(i)
    void generate(RoomType type, const std::array<bool, 4>& doors, int seed);

    // Обновить визуальное состояние дверей (открыты/закрыты)
    void setDoorsLocked(bool locked);

    void draw(sf::RenderWindow& window);

    // Проверка препятствия в координатах мира
    bool isSolid(float x, float y, bool ignoreDoors) const;

    // Проверка, находится ли прямоугольник внутри дверного проёма в указанном направлении
    bool isAtDoor(const sf::FloatRect& bounds, Direction dir) const;

    // Позиция появления игрока при входе с указанной стороны
    sf::Vector2f getEntryPosition(Direction fromDir) const;

    // Центральная точка комнаты
    sf::Vector2f getCenter() const;

    // Случайные точки для спавна врагов/предметов (не в стене)
    std::vector<sf::Vector2f> getSpawnPoints(int count, int seed) const;

    RoomType getType() const;
    bool hasDoor(Direction d) const;
    bool isDoorsLocked() const;

    static int getGridWidth()  { return GRID_WIDTH; }
    static int getGridHeight() { return GRID_HEIGHT; }
    static int getTileSize()   { return TILE_SIZE; }

private:
    // 0 = пол, 1 = стена, 2 = дверь
    int m_Grid[GRID_HEIGHT][GRID_WIDTH];
    std::array<bool, 4> m_Doors;
    RoomType m_Type;
    bool m_Locked;

    // Визуальные компоненты (перестраиваются при generate и setDoorsLocked)
    std::vector<sf::RectangleShape> m_Tiles;
    std::vector<sf::RectangleShape> m_Decorations;

    void buildVisuals();
    void placeDoors();
    void placeObstacles(int seed);
};
