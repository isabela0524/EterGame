#pragma once

#include "AbstractMage.h"

class LightMage : public AbstractMage
{
public:
    LightMage();
    void getDescription() const override;
    bool usePower(Game& game) const override;
};