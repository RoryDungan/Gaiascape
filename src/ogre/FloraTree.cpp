#include "floratree.h"

floraTree::floraTree(std::string name, Ogre::SceneManager *sceneMgr, Ogre::Vector3 location)
{
    sName = name; // Required for Ogre as all entities an nodes need to be unique, this means that names must be unique as well
    mSceneMgr = sceneMgr;
    mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(sName + "_main");

    mEnt = mSceneMgr->createEntity(sName + "_entity", "shrub.mesh");
    mNode->attachObject(mEnt);

    // Scale starts at a base number of 85 - 95, and each individual dimension may vary by 5.
    short unsigned int baseScale = Random::getSingleton().getRand(85, 95);
    mNode->setScale(baseScale + Random::getSingleton().getRand(-5, 5), baseScale + Random::getSingleton().getRand(-5, 5), baseScale + Random::getSingleton().getRand(-5, 5));

    mNode->setPosition(location);
    // Also add a random rotation
    mNode->yaw(Ogre::Degree(Random::getSingleton().getRand(0, 359)));
}
