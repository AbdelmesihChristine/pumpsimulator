#include "UserProfileManager.h"

UserProfileManager::UserProfileManager(QObject* parent)
    : QObject(parent)
{
    // Optionally, add a default profile
    UserProfile defaultP("Default", 1.0, 10.0, 2.0, 6.0);
    profiles.push_back(defaultP);
    activeProfile = defaultP;
}

void UserProfileManager::loadProfile(const UserProfile &profile)
{
    activeProfile = profile;
}

UserProfile UserProfileManager::getActiveProfile() const
{
    return activeProfile;
}

const std::vector<UserProfile>& UserProfileManager::getProfiles() const
{
    return profiles;
}

void UserProfileManager::addProfile(const UserProfile &profile)
{
    profiles.push_back(profile);
}

void UserProfileManager::updateProfile(int index, const UserProfile &profile)
{
    if (index >= 0 && index < (int)profiles.size()) {
        profiles[index] = profile;
    }
}

void UserProfileManager::deleteProfile(int index)
{
    if (index >= 0 && index < (int)profiles.size()) {
        profiles.erase(profiles.begin() + index);
    }
}
