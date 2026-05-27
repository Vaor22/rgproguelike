#include "Room.h"
#include "AssetManager.h"
#include <random>
#include <cmath>

Room::Room() : m_Type(RoomType::NORMAL), m_Locked(false) {
    std::array<bool, 4> d = { true, true, true, true };
    generate(RoomType::START, d, 0);
}

void Room::generate(RoomType type, const std::array<bool, 4>& doors, int seed) {
    m_Type = type;
    m_Doors = doors;
    m_Locked = false;

    // Очистка сетки
    for (int y = 0; y < GRID_HEIGHT; ++y)
        for (int x = 0; x < GRID_WIDTH; ++x)
            m_Grid[y][x] = 0;

    // Внешние стены
    for (int x = 0; x < GRID_WIDTH; ++x) {
        m_Grid[0][x] = 1;
        m_Grid[GRID_HEIGHT - 1][x] = 1;
    }
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        m_Grid[y][0] = 1;
        m_Grid[y][GRID_WIDTH - 1] = 1;
    }

    placeDoors();
    if (type == RoomType::NORMAL || type == RoomType::BOSS)
        placeObstacles(seed);
    buildVisuals();
}

void Room::placeDoors() {
    if (m_Doors[(int)Direction::NORTH]) m_Grid[0][GRID_WIDTH / 2] = 2;
    if (m_Doors[(int)Direction::SOUTH]) m_Grid[GRID_HEIGHT - 1][GRID_WIDTH / 2] = 2;
    if (m_Doors[(int)Direction::WEST])  m_Grid[GRID_HEIGHT / 2][0] = 2;
    if (m_Doors[(int)Direction::EAST])  m_Grid[GRID_HEIGHT / 2][GRID_WIDTH - 1] = 2;
}

void Room::placeObstacles(int seed) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> countDist(2, 5);
    std::uniform_int_distribution<int> xDist(3, GRID_WIDTH - 4);
    std::uniform_int_distribution<int> yDist(2, GRID_HEIGHT - 3);
    int count = countDist(rng);
    for (int i = 0; i < count; ++i) {
        int ox = xDist(rng), oy = yDist(rng);
        if (ox == GRID_WIDTH / 2 && oy == GRID_HEIGHT / 2) continue;
        m_Grid[oy][ox] = 1;
    }
}

void Room::buildVisuals() {
    m_TileSprites.clear();

    // Получаем текстуры
    const sf::Texture* texWall = AssetManager::instance().getTexture("wall_1");
    const sf::Texture* texHoriz = AssetManager::instance().getTexture("horizont");
    const sf::Texture* texVert = AssetManager::instance().getTexture("vert");
    const sf::Texture* texLeftUp = AssetManager::instance().getTexture("leftUp");
    const sf::Texture* texRightUp = AssetManager::instance().getTexture("rightUp");
    const sf::Texture* texLeftDown = AssetManager::instance().getTexture("leftDown");
    const sf::Texture* texRightDown = AssetManager::instance().getTexture("rightDown");
    const sf::Texture* texFloor = AssetManager::instance().getTexture("floor");

    // Функция масштабирования текстуры под TILE_SIZE (80x80)
    auto makeSprite = [](const sf::Texture* tex, float x, float y) -> sf::Sprite {
        sf::Sprite spr;
        if (!tex) return spr;
        spr.setTexture(*tex);
        sf::Vector2u sz = tex->getSize();
        if (sz.x != TILE_SIZE || sz.y != TILE_SIZE) {
            float sx = static_cast<float>(TILE_SIZE) / sz.x;
            float sy = static_cast<float>(TILE_SIZE) / sz.y;
            spr.setScale(sx, sy);
        }
        spr.setPosition(x, y);
        return spr;
        };

    // Проходим по всем клеткам
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            int cell = m_Grid[y][x];
            float posX = static_cast<float>(x * TILE_SIZE);
            float posY = static_cast<float>(y * TILE_SIZE);

            if (cell == 0) { // Пол
                if (texFloor)
                    m_TileSprites.push_back(makeSprite(texFloor, posX, posY));
            }
            else if (cell == 1) { // Стена
                const sf::Texture* tex = nullptr;
                bool isTop = (y == 0);
                bool isBottom = (y == GRID_HEIGHT - 1);
                bool isLeft = (x == 0);
                bool isRight = (x == GRID_WIDTH - 1);

                if (isTop && isLeft) tex = texLeftUp;
                else if (isTop && isRight) tex = texRightUp;
                else if (isBottom && isLeft) tex = texLeftDown;
                else if (isBottom && isRight) tex = texRightDown;
                else if (isTop || isBottom) tex = texHoriz;
                else if (isLeft || isRight) tex = texVert;
                else tex = texWall;

                if (tex)
                    m_TileSprites.push_back(makeSprite(tex, posX, posY));
            }
            // cell == 2 (дверь) – не рисуем ничего
        }
    }
}

void Room::setDoorsLocked(bool locked) {
    if (m_Locked != locked) {
        m_Locked = locked;
        // Визуально ничего не меняем, т.к. двери не рисуются
    }
}

void Room::draw(sf::RenderWindow& window) {
    for (auto& sprite : m_TileSprites)
        window.draw(sprite);
}

bool Room::isSolid(float x, float y, bool ignoreDoors) const {
    if (x < 0 || y < 0) return true;
    int gx = static_cast<int>(x) / TILE_SIZE;
    int gy = static_cast<int>(y) / TILE_SIZE;
    if (gx < 0 || gy < 0 || gx >= GRID_WIDTH || gy >= GRID_HEIGHT) return true;
    int cell = m_Grid[gy][gx];
    if (cell == 1) return true;
    if (cell == 2) return ignoreDoors ? false : m_Locked;
    return false;
}

bool Room::isRectSolid(const sf::FloatRect& rect) const {
    int left = std::max(0, static_cast<int>(rect.left / TILE_SIZE));
    int top = std::max(0, static_cast<int>(rect.top / TILE_SIZE));
    int right = std::min(GRID_WIDTH - 1, static_cast<int>((rect.left + rect.width) / TILE_SIZE));
    int bottom = std::min(GRID_HEIGHT - 1, static_cast<int>((rect.top + rect.height) / TILE_SIZE));
    for (int y = top; y <= bottom; ++y)
        for (int x = left; x <= right; ++x) {
            int cell = m_Grid[y][x];
            if (cell == 1) return true;
            if (cell == 2 && m_Locked) return true;
        }
    return false;
}

bool Room::isAtDoor(const sf::FloatRect& bounds, Direction dir) const {
    if (!m_Doors[(int)dir]) return false;
    float cx = bounds.left + bounds.width / 2.f;
    float cy = bounds.top + bounds.height / 2.f;
    int doorX = GRID_WIDTH / 2, doorY = GRID_HEIGHT / 2;
    const float edge = static_cast<float>(TILE_SIZE) * 0.4f;
    switch (dir) {
    case Direction::NORTH:
        return cy < edge && std::abs(cx - (doorX + 0.5f) * TILE_SIZE) < TILE_SIZE * 0.55f;
    case Direction::SOUTH:
        return cy > GRID_HEIGHT * TILE_SIZE - edge && std::abs(cx - (doorX + 0.5f) * TILE_SIZE) < TILE_SIZE * 0.55f;
    case Direction::WEST:
        return cx < edge && std::abs(cy - (doorY + 0.5f) * TILE_SIZE) < TILE_SIZE * 0.55f;
    case Direction::EAST:
        return cx > GRID_WIDTH * TILE_SIZE - edge && std::abs(cy - (doorY + 0.5f) * TILE_SIZE) < TILE_SIZE * 0.55f;
    }
    return false;
}

sf::Vector2f Room::getEntryPosition(Direction fromDir) const {
    float cx = (GRID_WIDTH / 2 + 0.5f) * TILE_SIZE;
    float cy = (GRID_HEIGHT / 2 + 0.5f) * TILE_SIZE;
    switch (fromDir) {
    case Direction::NORTH: return { cx, static_cast<float>(TILE_SIZE) * 1.5f };
    case Direction::SOUTH: return { cx, static_cast<float>((GRID_HEIGHT - 2) * TILE_SIZE) + TILE_SIZE / 2.f };
    case Direction::WEST:  return { static_cast<float>(TILE_SIZE) * 1.5f, cy };
    case Direction::EAST:  return { static_cast<float>((GRID_WIDTH - 2) * TILE_SIZE) + TILE_SIZE / 2.f, cy };
    }
    return { cx, cy };
}

sf::Vector2f Room::getCenter() const {
    return { (GRID_WIDTH / 2 + 0.5f) * TILE_SIZE, (GRID_HEIGHT / 2 + 0.5f) * TILE_SIZE };
}

bool Room::isAreaFree(sf::Vector2f pos, float radius) const {
    sf::FloatRect area(pos.x - radius, pos.y - radius, radius * 2, radius * 2);
    return !isRectSolid(area);
}

sf::Vector2f Room::getSafeSpawnPosition(float entityRadius, int maxAttempts) const {
    sf::Vector2f center = getCenter();
    if (isAreaFree(center, entityRadius)) {
        return center;
    }

    const float offsets[] = { -60.f, -40.f, -20.f, 0.f, 20.f, 40.f, 60.f };
    for (int attempt = 0; attempt < maxAttempts; ++attempt) {
        for (float dx : offsets) {
            for (float dy : offsets) {
                sf::Vector2f pos(center.x + dx, center.y + dy);
                if (isAreaFree(pos, entityRadius)) {
                    return pos;
                }
            }
        }
        for (float r = 80.f; r <= 220.f; r += 40.f) {
            for (float angle = 0; angle < 360; angle += 45) {
                float rad = angle * 3.14159f / 180.f;
                sf::Vector2f pos(center.x + r * cos(rad), center.y + r * sin(rad));
                if (isAreaFree(pos, entityRadius)) {
                    return pos;
                }
            }
        }
    }
    return center; // fallback
}

std::vector<sf::Vector2f> Room::getSpawnPoints(int count, int seed, float entityRadius) const {
    if (count <= 0) return {};

    std::vector<sf::Vector2f> result;
    result.reserve(count);

    std::mt19937 rng(seed ^ 0x5A5A);
    std::uniform_int_distribution<int> xDist(1, GRID_WIDTH - 2);
    std::uniform_int_distribution<int> yDist(1, GRID_HEIGHT - 2);

    const float minDistSq = (entityRadius * 2.5f) * (entityRadius * 2.5f);
    int attempts = 0;
    const int MAX_ATTEMPTS = 800;

    while ((int)result.size() < count && attempts < MAX_ATTEMPTS) {
        int gx = xDist(rng), gy = yDist(rng);
        sf::Vector2f p((gx + 0.5f) * TILE_SIZE, (gy + 0.5f) * TILE_SIZE);
        if (isAreaFree(p, entityRadius)) {
            bool tooClose = false;
            for (auto& existing : result) {
                float dx = existing.x - p.x, dy = existing.y - p.y;
                if (dx * dx + dy * dy < minDistSq) {
                    tooClose = true;
                    break;
                }
            }
            if (!tooClose) {
                result.push_back(p);
                continue;
            }
        }
        attempts++;
    }

    if (result.empty() && count > 0) {
        result.push_back(getSafeSpawnPosition(entityRadius));
    }
    return result;
}

RoomType Room::getType() const { return m_Type; }
bool Room::hasDoor(Direction d) const { return m_Doors[(int)d]; }
bool Room::isDoorsLocked() const { return m_Locked; }