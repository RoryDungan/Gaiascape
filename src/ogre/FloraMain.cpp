#include "floramain.h"

floraMain::floraMain()
{
    // stub
}

Ogre::Vector3 floraMain::getPosition()
{
    //return mNode->getPosition(); // This caused a crash at one point, and I'm unconvinced I actually fixed the problem.
    return Ogre::Vector3(0,0,0); // Because it turns out the above WAS crashing.
}

std::string floraMain::getName()
{
    return sName;
}
