#include "FloraManager.h"
#include <cmath>

FloraManager* FloraManager::sInstance = 0;

FloraManager FloraManager::getSingleton()
{
    if(!sInstance)
        sInstance = new FloraManager;
    return *sInstance;
}

FloraManager* FloraManager::getSingletonPtr()
{
    if(!sInstance)
        sInstance = new FloraManager;
    return sInstance;
}

bool FloraManager::addFlora(FloraMain &newFlora)
{
    for(unsigned int x = 0; x < vFlora.size(); x++)
    {
        if(vFlora[x]->getName() == newFlora.getName())
        {
            return false;
        }
    }
    // If everything is ok, add to vector
    vFlora.push_back(&newFlora);
    return true;
}

bool FloraManager::removeFlora(FloraMain *flora)
{
    for(unsigned int x = 0; x <= vFlora.size(); x++)
    {
        if(vFlora[x]->getName() == flora->getName())
        {
            Ogre::Entity* entity = vFlora[x]->mEnt;
            Ogre::SceneNode* node = vFlora[x]->mNode;

            node->detachObject(entity->getName());
            vFlora[x]->getSceneMgr()->destroyEntity(entity->getName());
            vFlora[x]->getSceneMgr()->destroySceneNode(node->getName());

            vFlora.erase(vFlora.begin() + x); // Erase vector data

            delete entity;
            delete node;

            return true; // Success!
        }
    }
    return false; // if we were unable to remove the specified model
}

void FloraManager::removeAllFlora()
{
    Ogre::Entity* entity;
    Ogre::SceneNode* node;

    for(short unsigned i = 0; i < vFlora.size(); i++)
    {
        entity = vFlora[i]->mEnt;
        node = vFlora[i]->mNode;

        node->detachObject(entity->getName());
        vFlora[i]->getSceneMgr()->destroyEntity(entity->getName());
        vFlora[i]->getSceneMgr()->destroySceneNode(node->getName());
    }
    vFlora.clear(); // Clear vector data
    //delete entity;
    //delete node;
}

float FloraManager::getFloraClosestToPoint(Ogre::Vector3 point) // Two dimensional because why not?
{
    float fMinDistance = 100; // Default distance. Nothing should be affecting it at 100 units away.
    float fXDistance;
    float fZDistance;
    for(unsigned int x = 0; x < vFlora.size(); x++)
    {
        // Detect if ID is close enough to be attacked
        Ogre::Vector3 floraPos;
        floraPos = vFlora[x]->getPosition();
        // Calculating distance...
        float mXDistance = floraPos.x - point.x;
        float mZDistance = floraPos.z - point.z;

        float distance = sqrt( pow(mXDistance, 2) + pow(mZDistance,2) ); // Using pythagoram's a^2 + b^2 = c^2
        if(distance < fMinDistance)
        {
            fMinDistance = distance;
        }
    }
    return fMinDistance;
}

void FloraManager::spawnGrass(Ogre::SceneManager *mSceneMgr)
{
    Ogre::Entity *grass = mSceneMgr->createEntity("grass", "GrassBladesMesh");
    Ogre::StaticGeometry *sg = mSceneMgr->createStaticGeometry("GrassArea");

    const float size = 375;
    const float amount = 20;

    sg->setRegionDimensions(Ogre::Vector3(size, size, size));
    sg->setOrigin(Ogre::Vector3(-size/2, 0, -size/2));

    for (int x = -size/2; x < size/2; x += (size/amount))
    {
            for (int z = -size/2; z < size/2; z += (size/amount))
            {
                    Ogre::Real r = size / amount / 2;
                    Ogre::Vector3 pos(x + Ogre::Math::RangeRandom(-r, r), 0, z + Ogre::Math::RangeRandom(-r, r));
                    Ogre::Vector3 scale(1, Ogre::Math::RangeRandom(0.9, 1.1), 1);
                    Ogre::Quaternion orientation;
                    orientation.FromAngleAxis(Ogre::Degree(Ogre::Math::RangeRandom(0, 359)), Ogre::Vector3::UNIT_Y);

                    sg->addEntity(grass, pos, orientation, scale);
            }
    }

    sg->build();
}

void FloraManager::createGrassMesh()
{
    const float width = 25;
    const float height = 30;
    Ogre::ManualObject mo("GrassObject");

    Ogre::Vector3 vec(width/2, 0, 0);
    Ogre::Quaternion rot;
    rot.FromAngleAxis(Ogre::Degree(60), Ogre::Vector3::UNIT_Y);

    mo.begin("Examples/GrassBlades", Ogre::RenderOperation::OT_TRIANGLE_LIST);
    for (int i = 0; i < 3; ++i)
    {
        mo.position(-vec.x, height, -vec.z);
        mo.textureCoord(0, 0);

        mo.position(vec.x, height, vec.z);
        mo.textureCoord(1, 0);

        mo.position(-vec.x, 0, -vec.z);
        mo.textureCoord(0, 1);

        mo.position(vec.x, 0, vec.z);
        mo.textureCoord(1, 1);

        int offset = i * 4;
        mo.triangle(offset, offset+3, offset+1);
        mo.triangle(offset, offset+2, offset+3);

        vec = rot * vec;
    }
    mo.end();

    mo.convertToMesh("GrassBladeMesh");
}
