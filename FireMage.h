#pragma once

#include "AbstractMage.h"

class FireMage : public AbstractMage
{
public:
    FireMage();
    void getDescription() const override;
    bool usePower(Game& game) const override;
};
