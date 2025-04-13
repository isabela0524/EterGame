#pragma once

#include "AbstractMagic.h"

class Fog : public AbstractMagic
{
public:
    Fog();
    void getDescription() const override;
    bool usePower(Game& game) override;
};
