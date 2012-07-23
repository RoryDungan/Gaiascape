#ifndef FLORAMAIN_H
#define FLORAMAIN_H

#include <OgreRoot.h>
#include <OgreEntity.h>

#include "algorithms/random.h" // Also used by Terrain.h

class floraMain
{
    public:
        floraMain();
        ~floraMain()
        {
            // Deconstructor
            /*mNode->detachObject(mEnt);
            //mNode->removeAndDestroyAllChildren();

            mSceneMgr->destroyEntity(mEnt->getName());
            mSceneMgr->destroySceneNode(mNode->getName());*/

            //delete mEnt; // The data from mEnt has already been deleted?
            //delete mNode;
        }

        Ogre::Vector3 getPosition();
        std::string getName();
        Ogre::SceneManager* getSceneMgr(){ return mSceneMgr;}

        Ogre::Entity* mEnt; // Holds the class' OWN entity here.
        Ogre::SceneNode* mNode; // Holds the class' OWN node here.
    protected:
        Ogre::SceneManager* mSceneMgr;
        std::string sName;
};

#endif // FLORAMAIN_H
