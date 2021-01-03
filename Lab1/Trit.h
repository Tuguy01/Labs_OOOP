#pragma once
#include <iostream>

enum class trit: char{
	False = 0,
	Unknown = 1,
	True = 3
};

trit operator!(trit x);
trit operator|(trit x, trit y);
trit operator&(trit x, trit y);
std::ostream& operator<<(std::ostream& os, trit x);