#pragma once

#include "AllMagicPowers.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <random>


class MagicDistributor
{
public:
	static std::vector<std::shared_ptr<AbstractMagic>> generateMagicPowers(bool hasIllusions);
};

