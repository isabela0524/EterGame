#pragma once

#include "AbstractMagic.h"

class Sparks : public AbstractMagic
{
public:
    Sparks();
    void getDescription() const override;
    bool usePower(Game& game) override;
};
