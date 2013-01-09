#include "FloraTree.h"

FloraTree::FloraTree(std::string name, Ogre::SceneManager *sceneMgr, Ogre::Vector3 location)
{
    sName = name; // Required for Ogre as all entities an nodes need to be unique, this means that names must be unique as well
    mSceneMgr = sceneMgr;
    mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(sName + "_main");

    mEnt = mSceneMgr->createEntity(sName + "_entity", "ninja.mesh");
   // mEnt->setMaterialName("Shrub-Material");
    mNode->attachObject(mEnt);

    // Scale starts at a base number of 85 - 95, and each individual dimension may vary by 5.
    short unsigned int baseScale = Random::getSingleton().getRand(85, 95);
    mNode->setScale(0.9f, 0.9f, 0.9f);

    mNode->setPosition(location);
    // Also add a random rotation
    mNode->yaw(Ogre::Degree(Random::getSingleton().getRand(0, 359)));
}
