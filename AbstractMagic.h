#pragma once

#include <string>
#include <iostream>
class Game;

class AbstractMagic
{
protected:
    std::string m_description;
    bool        m_hasBeenUsed{false};
    void        markUsed();
public:
    AbstractMagic() = default;
    virtual ~AbstractMagic() = default;
    virtual void getDescription() const = 0;
    virtual bool usePower(Game& game) = 0;
    bool hasBeenUsed() const;
    void resetUsage();
};
