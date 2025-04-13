#include "Boulder.h"
#include "Game.h"

Boulder::Boulder()
{
    m_description = "Boulder: Cover any illusion with a card from your hand without turning the illusion face up.";
}

void Boulder::getDescription() const
{
    std::cout << m_description << std::endl;
}

bool Boulder::usePower(Game& game) 
{
    std::cout << "Boulder power activated: Cover any illusion with a card from your hand without turning the illusion face up." << std::endl;
    size_t xpos, ypos;
    std::cout << "\nEnter position of the illusion: ";
    std::cin >> xpos >> ypos;

    if (!game.accessBoard().validateInsertPosition({ xpos,ypos }))
    {
        return false;
    }

    if (!game.accessGameboardAPI()[xpos][ypos].has_value())
    {
        return false;
    }


    if (!game.accessGameboardAPI()[xpos][ypos]->top().isIllusion())
    {
        return false;
    }

    int valueInput;
    std::cout << "\nEnter value: ";
    std::cin >> valueInput;
    uint8_t value = static_cast<uint8_t>(valueInput);


    if (!game.accessBoard().validateValue(value))
    {
        return false;
    }

    if (!game.getCurrentPlayer().removeCard(value))
    {
        std::cout << "value available wrong!\n"; /*if card exists also delete it*/
        return false; /*restart loop*/
    }


    game.accessBoard().insertCard({ value,game.getCurrentPlayerEnum() }, { xpos,ypos });


    markUsed();
    game.getCurrentPlayer().markMagicUsed();
    return true;
}
