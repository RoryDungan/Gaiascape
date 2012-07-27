#ifndef FLORATREE_H
#define FLORATREE_H

#include "FloraMain.h"

class FloraTree : public FloraMain
{
public:
    FloraTree(std::string name, Ogre::SceneManager* sceneMgr, Ogre::Vector3 location);
    ~FloraTree() { /*stub*/ }
};

#endif // FLORATREE_H
