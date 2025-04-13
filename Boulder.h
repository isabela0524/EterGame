#pragma once

#include "AbstractMagic.h"

class Boulder : public AbstractMagic
{
public:
    Boulder();
    void getDescription() const override;
    bool usePower(Game& game) override;
};
