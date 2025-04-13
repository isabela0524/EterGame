#pragma once

#include "AbstractMagic.h"

class Storm : public AbstractMagic
{
public:
    Storm();
    void getDescription() const override;
    bool usePower(Game& game) override;
};
