#include "FloraMain.h"

FloraMain::FloraMain()
{
    // stub
}

Ogre::Vector3 FloraMain::getPosition()
{
    //return mNode->getPosition(); // This caused a crash at one point, and I'm unconvinced I actually fixed the problem.
    return Ogre::Vector3(0,0,0); // Because it turns out the above WAS crashing.
}

std::string FloraMain::getName()
{
    return sName;
}
