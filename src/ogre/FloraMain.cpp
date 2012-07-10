#include "floramain.h"

floraMain::floraMain()
{
    // stub
}

Ogre::Vector3 floraMain::getPosition()
{
    return mNode->getPosition();
}

std::string floraMain::getName()
{
    return sName;
}
