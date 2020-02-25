#pragma once

#include <memory>

template <typename T>
class Singleton
{
	public:
	virtual ~Singleton() {}

	static T& getInstance()
	{
		static std::unique_ptr<T> instance = std::make_unique<T>();

		return *instance;
	}

	protected:
	Singleton() {};
};