#pragma once

#include "AbstractMage.h"

class AirMage : public AbstractMage
{
public:
    AirMage();
    void getDescription() const override;
    bool usePower(Game& game) const override;
};
