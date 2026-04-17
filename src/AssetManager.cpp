#include "AssetManager.h"

AssetManager& AssetManager::instance()
{
    static AssetManager inst;
    return inst;
}

AssetManager::AssetManager()
    : m_FontLoaded(false)
{
    loadFont();
}

void AssetManager::loadFont()
{
    // Пробуем загрузить шрифт из нескольких стандартных путей
    const char* paths[] = {
        "assets/font.ttf",
        "../assets/font.ttf",
        // Windows
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/segoeui.ttf",
        "C:/Windows/Fonts/consola.ttf",
        // macOS
        "/System/Library/Fonts/Helvetica.ttc",
        "/System/Library/Fonts/SFNSMono.ttf",
        "/Library/Fonts/Arial Unicode.ttf",
        // Linux
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        "/usr/share/fonts/TTF/DejaVuSans.ttf"
    };

    for (const auto& path : paths)
    {
        if (m_Font.loadFromFile(path))
        {
            m_FontLoaded = true;
            return;
        }
    }
    m_FontLoaded = false;
}

const sf::Font& AssetManager::getFont() const
{
    return m_Font;
}

bool AssetManager::isFontLoaded() const
{
    return m_FontLoaded;
}
