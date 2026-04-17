#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// Singleton: загрузка и хранение общих ресурсов (шрифты)
class AssetManager
{
public:
    static AssetManager& instance();

    const sf::Font& getFont() const;
    bool isFontLoaded() const;

private:
    AssetManager();
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    void loadFont();

    sf::Font m_Font;
    bool m_FontLoaded;
};
