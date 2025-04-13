#pragma once

#include "AbstractMage.h"

class EarthMage : public AbstractMage
{
public:
    EarthMage();
    void getDescription() const override;
    bool usePower(Game& game) const override;
};
