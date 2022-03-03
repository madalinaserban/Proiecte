#pragma once
#include <random>
#include <type_traits>
#include "SingleTon.h"
class UtilityFunctions final
{
public:
	template <typename T, typename =
		std::enable_if_t<std::is_floating_point<T>::value>>
		static T GenerateRandomNumber(const T& min, const T& max); //only float types allowed
	static void Tokenizing(std::string& line, std::vector<std::string>& receiver);

private:
	UtilityFunctions();
	template <class T>
	friend class SingleTon;
};

template<typename T, typename>
inline T UtilityFunctions::GenerateRandomNumber(const T& min, const T& max)
{
	std::random_device seeder;
	std::mt19937 engine(seeder());
	std::uniform_real_distribution<T> dist(min, max);
	return dist(engine);
}