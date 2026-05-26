#pragma once
#include <vector>
#include "Upgrade.h"

struct PlayerStats
{
    float maxHealthPercent = 100.0f;
    float damagePercent = 100.0f;
    float speedPercent = 100.0f;
    float fireRatePercent = 100.0f;

    bool hasRegeneration = false;
    bool hasPiercingShots = false;
    bool hasExplosiveShots = false;
    bool hasLifesteal = false;
    bool hasDoubleShot = false;
    bool hasShieldCharge = false;
    bool hasDashReset = false;
    bool hasGreed = false;
    bool hasCriticalHit = false;

    float regenRate = 0.5f;
    float lifestealPercent = 0.1f;
    float criticalChance = 0.15f;
    float criticalMultiplier = 2.0f;
    float greedMultiplier = 1.5f;
    int extraProjectiles = 0;
};

class ExperienceSystem
{
public:
    ExperienceSystem();

    void addExperience(int amount);
    void addExperienceFromKill(int scoreValue);

    int getCurrentLevel() const { return m_Level; }
    int getCurrentExp() const { return m_CurrentExp; }
    int getExpForNextLevel() const { return getExpRequired(m_Level); }
    float getLevelProgress() const;

    // Новые методы для отложенных улучшений
    bool hasPendingUpgrade() const { return m_PendingUpgradeCount > 0; }
    int getPendingUpgradeCount() const { return m_PendingUpgradeCount; }
    void clearOnePendingUpgrade() { if (m_PendingUpgradeCount > 0) m_PendingUpgradeCount--; }
    void clearAllPendingUpgrades() { m_PendingUpgradeCount = 0; }

    const std::vector<UpgradeType>& getAcquiredUpgrades() const { return m_AcquiredUpgrades; }

    void applyUpgrade(UpgradeType type, PlayerStats& stats);
    std::vector<UpgradeType> getUpgradeChoices(int count = 3) const;

    void reset();
    void onLevelUp();

private:
    int getExpRequired(int level) const;

    int m_Level;
    int m_CurrentExp;
    int m_PendingUpgradeCount;          // счётчик накопленных улучшений
    std::vector<UpgradeType> m_AcquiredUpgrades;

    static constexpr int BASE_EXP = 100;
    static constexpr float EXP_SCALING = 1.2f;
};