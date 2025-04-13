#include "MagicDistributor.h"

std::vector<std::shared_ptr<AbstractMagic>> MagicDistributor::generateMagicPowers(bool hasIllusions)
{
    std::vector<std::shared_ptr<AbstractMagic>> magicPowers;

    magicPowers.push_back(std::make_shared<ControlledExplosion>());
    magicPowers.push_back(std::make_shared<Destruction>());
    magicPowers.push_back(std::make_shared<Flames>());
    magicPowers.push_back(std::make_shared<Lava>());
    magicPowers.push_back(std::make_shared<Sparks>());
    magicPowers.push_back(std::make_shared<Blizzard>());
    magicPowers.push_back(std::make_shared<Storm>());
    magicPowers.push_back(std::make_shared<Hurricane>()); 
    magicPowers.push_back(std::make_shared<Gale>());
    magicPowers.push_back(std::make_shared<StormSurge>());
    magicPowers.push_back(std::make_shared<Tide>());
    magicPowers.push_back(std::make_shared<Wave>());    
    magicPowers.push_back(std::make_shared<Cascade>());
    magicPowers.push_back(std::make_shared<Support>());
    magicPowers.push_back(std::make_shared<Earthquake>());
    magicPowers.push_back(std::make_shared<Crack>());

    if (hasIllusions) 
    {
        magicPowers.push_back(std::make_shared<Mirage>());
        magicPowers.push_back(std::make_shared<Fog>());
        magicPowers.push_back(std::make_shared<Boulder>());
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(magicPowers.begin(), magicPowers.end(), g);

    return { magicPowers.begin(), magicPowers.begin() + 2 };
}
