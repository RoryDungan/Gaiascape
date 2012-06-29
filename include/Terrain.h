#ifndef TERRAIN_H
#define TERRAIN_H

#include <OGRE/Ogre.h>
#include <OGRE/Terrain/OgreTerrain.h>
#include <OGRE/Terrain/OgreTerrainGroup.h>
#include <OGRE/OgreImage.h>

#include <algorithms/HeightMapGen.h>

class Terrain
{
public:
    Terrain(Ogre::SceneManager* sceneManager, Ogre::Light *light);
    ~Terrain();
    Ogre::TerrainGroup* getTerrainGroup() { return mTerrainGroup; }

    void createFlatTerrain();
    void loadHeightmap();
    void generateTerrain();

    void clearTerrain();

private:
    Ogre::SceneManager* mSceneManager;
    Ogre::TerrainGlobalOptions* mTerrainGlobals;
    Ogre::TerrainGroup* mTerrainGroup;
    Ogre::Light* mSun;
    bool mTerrainsImported;

    void defineTerrainFromFile(long x, long y);
    void initBlendMaps(Ogre::Terrain* terrain);
    void configureTerrainDefaults(Ogre::Light* light);
    void configureTextures(Ogre::Terrain::LayerInstanceList& layerList);

    // This should be defined before the terrain is generated. 4 is set as default for a small world.
    short unsigned int iTerrainSize;// = 4;
};

#endif // TERRAIN_H
