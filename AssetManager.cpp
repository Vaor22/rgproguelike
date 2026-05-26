#include "AssetManager.h"
#include <iostream>
#include <fstream>
#include <random>
#include <cstring>

// Для вывода сообщений в консоль (даже в Windows GUI-приложении)
#ifdef _WIN32
#include <windows.h>
#define DEBUG_OUT(msg) OutputDebugStringA(msg)
#else
#define DEBUG_OUT(msg) printf("%s", msg)
#endif

// Вспомогательная функция для генерации простой текстуры-заглушки
static void createPlaceholderTexture(sf::Texture& tex, int w, int h, sf::Color color, const sf::Color& secondColor = sf::Color::Transparent) {
    sf::Image img;
    img.create(w, h, color);
    if (secondColor != sf::Color::Transparent) {
        for (int y = 0; y < h; y += 8)
            for (int x = 0; x < w; x += 8)
                for (int dy = 0; dy < 4 && y + dy < h; ++dy)
                    for (int dx = 0; dx < 4 && x + dx < w; ++dx)
                        img.setPixel(x + dx, y + dy, secondColor);
    }
    tex.loadFromImage(img);
}

AssetManager& AssetManager::instance() {
    static AssetManager inst;
    return inst;
}

AssetManager::AssetManager()
    : m_FontLoaded(false) {
    loadFont();
    loadGameTextures();
}

void AssetManager::loadFont() {
    const char* paths[] = {
        "assets/font.ttf",
        "../assets/font.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/segoeui.ttf",
        "/System/Library/Fonts/Helvetica.ttc",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"
    };
    for (const auto& path : paths) {
        if (m_Font.loadFromFile(path)) {
            m_FontLoaded = true;
            DEBUG_OUT(("Font loaded: " + std::string(path) + "\n").c_str());
            return;
        }
    }
    m_FontLoaded = false;
    DEBUG_OUT("WARNING: No font loaded! Text will not appear.\n");
}

const sf::Font& AssetManager::getFont() const { return m_Font; }
bool AssetManager::isFontLoaded() const { return m_FontLoaded; }

bool AssetManager::loadTexture(const std::string& name, const std::string& path) {
    sf::Texture tex;
    if (tex.loadFromFile(path)) {
        tex.setRepeated(true);
        m_Textures[name] = std::move(tex);
        DEBUG_OUT(("OK: Loaded " + name + " from " + path + "\n").c_str());
        return true;
    }
    else {
        DEBUG_OUT(("FAIL: Could not load " + name + " from " + path + " – creating placeholder\n").c_str());

        // Создаём placeholder в зависимости от имени текстуры
        if (name.find("wall") != std::string::npos) {
            // Кирпичный узор
            int w = 80, h = 80;
            sf::Image img;
            img.create(w, h, sf::Color(120, 80, 50));
            for (int y = 0; y < h; y += 20)
                for (int x = 0; x < w; x += 40)
                    for (int dy = 0; dy < 18 && y + dy < h; ++dy)
                        for (int dx = 0; dx < 38 && x + dx < w; ++dx)
                            img.setPixel(x + dx, y + dy, sf::Color(180, 100, 60));
            tex.loadFromImage(img);
        }
        else if (name == "floor") {
            createPlaceholderTexture(tex, 80, 80, sf::Color(130, 90, 50), sf::Color(90, 60, 30));
        }
        else if (name == "money") {
            sf::Image img;
            img.create(32, 32, sf::Color::Yellow);
            // Нарисуем символ $
            for (int y = 8; y < 24; ++y) {
                img.setPixel(16, y, sf::Color::Black);
                img.setPixel(15, y, sf::Color::Black);
            }
            tex.loadFromImage(img);
        }
        else if (name.find("btn_") != std::string::npos) {
            sf::Color color = sf::Color(100, 100, 100);
            if (name == "btn_play") color = sf::Color(70, 170, 70);
            else if (name == "btn_settings") color = sf::Color(70, 70, 170);
            else if (name == "btn_load") color = sf::Color(170, 130, 70);
            else if (name == "btn_exit") color = sf::Color(170, 70, 70);
            createPlaceholderTexture(tex, 200, 60, color, sf::Color(255, 255, 255, 100));
        }
        else {
            createPlaceholderTexture(tex, 64, 64, sf::Color(128, 128, 128));
        }
        tex.setRepeated(true);
        m_Textures[name] = std::move(tex);
        return false;
    }
}

const sf::Texture* AssetManager::getTexture(const std::string& name) const {
    auto it = m_Textures.find(name);
    if (it == m_Textures.end()) {
        DEBUG_OUT(("WARNING: Texture '" + name + "' not found in manager!\n").c_str());
        return nullptr;
    }
    return &it->second;
}

bool AssetManager::hasTexture(const std::string& name) const {
    return m_Textures.count(name) > 0;
}

void AssetManager::loadGameTextures() {
    // Стены
    loadTexture("wall_1", "assets/Brick_Wall_1.png");
    loadTexture("wall_2", "assets/Brick_Wall_2.png");
    loadTexture("wall_3", "assets/Brick_Wall_3.png");
    loadTexture("wall_4", "assets/Brick_Wall_4.png");
    loadTexture("wall_5", "assets/Brick_Wall_5.png");
    loadTexture("wall_6", "assets/Brick_Wall_6.png");
    loadTexture("wall_7", "assets/Brick_Wall_7.png");
    loadTexture("wall_8", "assets/Brick_Wall_8.png");
    loadTexture("wall_9", "assets/Brick_Wall_9.png");
    loadTexture("wall_10", "assets/Brick_Wall_10.png");
    loadTexture("wall_11", "assets/Brick_Wall_11.png");
    loadTexture("wall_12", "assets/Brick_Wall_12.png");
    loadTexture("wall_13", "assets/Brick_Wall_13.png");
    loadTexture("wall_14", "assets/Brick_Wall_14.png");

    // Пол
    loadTexture("floor", "assets/Wooden_Floor.png");
    // Монетка
    loadTexture("money", "assets/money.png");
    // Кнопки
    loadTexture("btn_play", "assets/Button_Play.png");
    loadTexture("btn_settings", "assets/Button_Settings.png");
    loadTexture("btn_load", "assets/Button_Load.png");
    loadTexture("btn_exit", "assets/Button_Exit.png");

    DEBUG_OUT("=== Texture loading finished ===\n");
}