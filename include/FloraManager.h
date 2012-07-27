#ifndef FLORAMANAGER_H
#define FLORAMANAGER_H

#include <vector>

#include "FloraMain.h" // Includes OgreRoot

class FloraManager
{
    public:
        static FloraManager getSingleton();
        static FloraManager* getSingletonPtr();

        bool addFlora(FloraMain& newFlora);
        bool removeFlora(FloraMain* flora);
        void removeAllFlora(); // To be done every time we generate new terrain

        float getFloraClosestToPoint(Ogre::Vector3 point);
    private:
        std::vector<FloraMain*> vFlora;
        static FloraManager* sInstance;
};

#endif // FLORAMANAGER_H
