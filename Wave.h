#pragma once

#include "AbstractMagic.h"

class Wave : public AbstractMagic
{
public:
    Wave();
    void getDescription() const override;
    bool usePower(Game& game) override;
};
