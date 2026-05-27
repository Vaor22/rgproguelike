#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

class AssetManager
{
public:
    static AssetManager& instance();

    const sf::Font& getFont() const;
    bool isFontLoaded() const;

    // Новые методы для текстур
    bool loadTexture(const std::string& name, const std::string& path);
    const sf::Texture* getTexture(const std::string& name) const;
    bool hasTexture(const std::string& name) const;

    void loadGameTextures(); // загрузка всех игровых текстур

private:
    AssetManager();
    AssetManager(const AssetManager&) = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    void loadFont();

    sf::Font m_Font;
    bool m_FontLoaded;

    std::unordered_map<std::string, sf::Texture> m_Textures;
};