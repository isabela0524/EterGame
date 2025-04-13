#pragma once

#include "AbstractMage.h"

class WaterMage : public AbstractMage
{
public:
    WaterMage();
    void getDescription() const override;
    bool usePower(Game& game) const override;
};
