#pragma once

#include "AbstractMagic.h"

class Lava : public AbstractMagic
{
public:
    Lava();
    void getDescription() const override;
    bool usePower(Game& game) override;
};