#pragma once

#include "AbstractMagic.h"

class Support : public AbstractMagic
{
public:
    Support();
    void getDescription() const override;
    bool usePower(Game& game) override;
};
