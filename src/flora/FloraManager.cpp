#include "FloraManager.h"
#include <cmath>

FloraManager* FloraManager::sInstance = 0;

FloraManager FloraManager::getSingleton()
{
    if(!sInstance)
        sInstance = new FloraManager;
    return *sInstance;
}

FloraManager* FloraManager::getSingletonPtr()
{
    if(!sInstance)
        sInstance = new FloraManager;
    return sInstance;
}

bool FloraManager::addFlora(FloraMain &newFlora)
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

bool FloraManager::removeFlora(FloraMain *flora)
{
    for(unsigned int x = 0; x <= vFlora.size(); x++)
    {
        if(vFlora[x]->getName() == flora->getName())
        {
            Ogre::Entity* entity = vFlora[x]->mEnt;
            Ogre::SceneNode* node = vFlora[x]->mNode;

            node->detachObject(entity->getName());
            vFlora[x]->getSceneMgr()->destroyEntity(entity->getName());
            vFlora[x]->getSceneMgr()->destroySceneNode(node->getName());

            vFlora.erase(vFlora.begin() + x); // Erase vector data

            delete entity;
            delete node;

            return true; // Success!
        }
    }
    return false; // if we were unable to remove the specified model
}

void FloraManager::removeAllFlora()
{
    Ogre::Entity* entity;
    Ogre::SceneNode* node;

    for(short unsigned i = 0; i < vFlora.size(); i++)
    {
        entity = vFlora[i]->mEnt;
        node = vFlora[i]->mNode;

        node->detachObject(entity->getName());
        vFlora[i]->getSceneMgr()->destroyEntity(entity->getName());
        vFlora[i]->getSceneMgr()->destroySceneNode(node->getName());
    }
    vFlora.clear(); // Clear vector data
    //delete entity;
    //delete node;
}

float FloraManager::getFloraClosestToPoint(Ogre::Vector3 point) // Two dimensional because why not?
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