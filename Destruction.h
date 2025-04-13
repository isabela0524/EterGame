#pragma once

#include "AbstractMagic.h"

class Destruction : public AbstractMagic
{
public:
    Destruction();
    void getDescription() const override;
    bool usePower(Game& game) override;
};
