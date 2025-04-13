#pragma once

#include "AbstractMagic.h"

class ControlledExplosion : public AbstractMagic
{
public:
    ControlledExplosion();
    void getDescription() const override;
    bool usePower(Game& game) override;
};
