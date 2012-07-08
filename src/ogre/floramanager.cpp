#include "floramanager.h"
#include "math.h"

floraManager* floraManager::sInstance = 0;

floraManager floraManager::getSingleton()
{
    if(!sInstance)
        sInstance = new floraManager;
    return *sInstance;
}

floraManager* floraManager::getSingletonPtr()
{
    if(!sInstance)
        sInstance = new floraManager;
    return sInstance;
}

bool floraManager::addFlora(floraMain &newFlora)
{
    for(unsigned int x = 0; x < vFlora.size(); x++)
    {
        if(vFlora[x]->getName() == newFlora.getName())
        {
            return false;
        }
    }
    // If everything is ok, add to vector
    vFlora.push_back(&newFlora);
    return true;
}

bool floraManager::removeFlora(floraMain *flora)
{
    for(unsigned int x = 0; x <= vFlora.size(); x++)
    {
        if(vFlora[x]->getName() == flora->getName())
        {
            vFlora.erase(vFlora.begin() + x);
            return true; // Success!
        }
    }
    return false; // if we were unable to remove the specified model
}

void floraManager::removeAllFlora()
{
    for(short unsigned i = 0; i < vFlora.size(); i++)
    {
        delete vFlora[i];
    }
    vFlora.clear();
}

float floraManager::getFloraClosestToPoint(Ogre::Vector3 point) // Two dimensional because why not?
{
    float fMinDistance = 100; // Default distance. Nothing should be affecting it at 100 units away.
    float fXDistance;
    float fZDistance;
    for(unsigned int x = 0; x < vFlora.size(); x++)
    {
        // Detect if ID is close enough to be attacked
        Ogre::Vector3 floraPos;
        floraPos = vFlora[x]->getPosition();
        // Calculating distance...
        long signed int mXDistance = floraPos.x - point.x;
        long signed int mZDistance = floraPos.z - point.z;

        float distance = sqrt( pow(mXDistance,2) + pow(mZDistance,2) ); // Using pythagoram's a^2 + b^2 = c^2
        if(distance < fMinDistance)
        {
            fMinDistance = distance;
        }
    }
    return fMinDistance;
}
