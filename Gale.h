#pragma once

#include "AbstractMagic.h"

class Gale : public AbstractMagic
{
public:
    Gale();
    void getDescription() const override;
    bool usePower(Game& game) override;
};
