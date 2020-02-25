#include "AssetManager.h"

void AssetManager::addTexture(const std::string& path, const std::string& alias)
{
	getInstance().m_textures.try_emplace(alias);
	getInstance().m_textures[alias].loadFromFile(path);
}

sf::Texture& AssetManager::getTexture(const std::string& alias)
{
	return getInstance().m_textures[alias];
}