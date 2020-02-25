#pragma once

#include <SFML/Config.hpp>

enum class ClientMessageType : sf::Int8
{
	Invalid = -1,
	Login,
	Fleet,
	Attack,
	PlayAgain,
	Count
};

enum class ServerMessageType : sf::Int8
{
	Invalid = -1,
	LoginResult,
	FirstMove,
	AttackResult,
	EnemyAttack,
	GameOver,
	Count
};
