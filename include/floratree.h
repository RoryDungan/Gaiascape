#ifndef FLORATREE_H
#define FLORATREE_H

#include "floramain.h"

class floraTree : public floraMain
{
public:
    floraTree(std::string name, Ogre::SceneManager* sceneMgr, Ogre::Vector3 location);
};

#endif // FLORATREE_H
