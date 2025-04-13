#pragma once

#include "AbstractMagic.h"

class Blizzard : public AbstractMagic
{
public:
    Blizzard();
    void getDescription() const override;
    bool usePower(Game& game) override;
};
