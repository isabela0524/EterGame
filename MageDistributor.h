#pragma once

#include "AllMages.h"
#include <vector>
#include <memory>
#include <algorithm>
#include <random>

class MageDistributor
{
public:
    static std::vector<std::shared_ptr<AbstractMage>> generateMages();
};
