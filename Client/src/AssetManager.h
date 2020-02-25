#pragma once

#include "Singleton.h"

#include <SFML/Graphics/Texture.hpp>
#include <unordered_map>

class AssetManager : public Singleton<AssetManager>
{
	public:
	static void addTexture(const std::string& path, const std::string& alias);
	static sf::Texture& getTexture(const std::string& alias);

	private:
	std::unordered_map<std::string, sf::Texture> m_textures;
};