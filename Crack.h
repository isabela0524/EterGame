#pragma once

#include "AbstractMagic.h"

class Crack : public AbstractMagic
{
public:
    Crack();
    void getDescription() const override;
    bool usePower(Game& game) override;
};
