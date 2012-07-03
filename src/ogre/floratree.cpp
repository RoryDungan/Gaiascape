#include "floratree.h"

floraTree::floraTree(std::string name, Ogre::SceneManager *sceneMgr, Ogre::Vector3 location)
{
    sName = name;
    mSceneMgr = sceneMgr;
    mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(sName + "_main");

    mEnt = mSceneMgr->createEntity(sName + "_entity", "ninja.mesh");
    mNode->attachObject(mEnt);
    mNode->setScale(0.05, 0.05, 0.05);
    mNode->setPosition(location);
}
