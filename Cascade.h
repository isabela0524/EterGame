#pragma once

#include "AbstractMagic.h"

class Cascade : public AbstractMagic
{
public:
    Cascade();
    void getDescription() const override;
    bool usePower(Game& game) override;
};
