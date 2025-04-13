#include "ControlledExplosion.h"
#include "Game.h"

ControlledExplosion::ControlledExplosion()
{
    m_description = "ControlledExplosion: The board explodes!";
}

void ControlledExplosion::getDescription() const
{
    std::cout << m_description << std::endl;
}

bool ControlledExplosion::usePower(Game& game)
{
    std::cout << "ControlledExplosion power activated: The board explodes!" << std::endl;
    game.useExplosion();
    markUsed();
    game.getCurrentPlayer().markMagicUsed();
    return true;

}
