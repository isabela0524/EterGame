#pragma once

#include "AbstractMagic.h"

class Hurricane : public AbstractMagic
{
public:
    Hurricane();
    void getDescription() const override;
    bool usePower(Game& game) override;
};
