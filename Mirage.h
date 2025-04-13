#pragma once

#include "AbstractMagic.h"

class Mirage : public AbstractMagic
{
public:
    Mirage();
    void getDescription() const override;
    bool usePower(Game& game) override;
};
