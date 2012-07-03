#ifndef FLORAMAIN_H
#define FLORAMAIN_H

#include <OgreRoot.h>
#include <OgreEntity.h>

class floraMain
{
    public:
        floraMain();

        Ogre::Vector3 getPosition();
        std::string getName();

        Ogre::Entity* mEnt; // Holds the class' OWN entity here.
        Ogre::SceneNode* mNode; // Holds the class' OWN node here.
    protected:
        Ogre::SceneManager* mSceneMgr;
        std::string sName;
};

#endif // FLORAMAIN_H
