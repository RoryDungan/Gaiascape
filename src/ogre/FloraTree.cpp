#include "floratree.h"

floraTree::floraTree(std::string name, Ogre::SceneManager *sceneMgr, Ogre::Vector3 location)
{
    sName = name; // Required for Ogre as all entities an nodes need to be unique, this means that names must be unique as well
    mSceneMgr = sceneMgr;
    mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(sName + "_main");

    mEnt = mSceneMgr->createEntity(sName + "_entity", "ninja.mesh");
    mNode->attachObject(mEnt);
    mNode->setScale(0.9, 0.9, 0.9); // Should be randomized
    mNode->setPosition(location);
    // Also add a random rotation
    mNode->yaw(Ogre::Degree(Random::getSingleton().getRand(0, 359)));
}
