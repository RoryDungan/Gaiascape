#ifndef TERRAIN_H
#define TERRAIN_H

#include <OGRE/Ogre.h>
#include <OGRE/Terrain/OgreTerrain.h>
#include <OGRE/Terrain/OgreTerrainGroup.h>

class Terrain
{
public:
    Terrain(Ogre::SceneManager* sceneManager, Ogre::Light *light);
    Ogre::TerrainGroup* getTerrainGroup() { return mTerrainGroup; }

//private:
    Ogre::SceneManager* mSceneManager;
    Ogre::TerrainGlobalOptions* mTerrainGlobals;
    Ogre::TerrainGroup* mTerrainGroup;
    bool mTerrainsImported;

    void defineTerrain(long x, long y);
    void initBlendMaps(Ogre::Terrain* terrain);
    void configureTerrainDefaults(Ogre::Light* light);
};

#endif // TERRAIN_H
