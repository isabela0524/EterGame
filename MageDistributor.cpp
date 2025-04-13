#include "MageDistributor.h"

std::vector<std::shared_ptr<AbstractMage>> MageDistributor::generateMages()
{
    std::vector<std::shared_ptr<AbstractMage>> mages;
    mages.push_back(std::make_shared<IceMage>());
    mages.push_back(std::make_shared<EarthMage>());
    mages.push_back(std::make_shared <FireMage>());
    mages.push_back(std::make_shared<WaterMage>());
    mages.push_back(std::make_shared<AirMage>());
    mages.push_back(std::make_shared<DarkMage>());
    mages.push_back(std::make_shared<LightMage>());

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(mages.begin(), mages.end(), g);

    return { mages.begin(), mages.begin() + 2 };
}
