#pragma once

#include "AbstractMagic.h"

class Flames : public AbstractMagic
{
public:
    Flames();
    void getDescription() const override;
    bool usePower(Game& game) override;
};
