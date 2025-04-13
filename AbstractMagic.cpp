#include "AbstractMagic.h"

void AbstractMagic::markUsed()
{
    m_hasBeenUsed = true;
}

bool AbstractMagic::hasBeenUsed() const
{
    return m_hasBeenUsed;
}

void AbstractMagic::resetUsage()
{
    m_hasBeenUsed = false;
}
