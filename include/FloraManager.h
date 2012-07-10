#ifndef FLORAMANAGER_H
#define FLORAMANAGER_H

#include <vector>

#include "floramain.h" // Includes OgreRoot

class floraManager
{
    public:
        static floraManager getSingleton();
        static floraManager* getSingletonPtr();

        bool addFlora(floraMain& newFlora);
        bool removeFlora(floraMain* flora);
        void removeAllFlora(); // To be done every time we generate new terrain

        float getFloraClosestToPoint(Ogre::Vector3 point);
    private:
        std::vector<floraMain*> vFlora;
        static floraManager* sInstance;
};

#endif // FLORAMANAGER_H
