#ifndef TERRAIN_H
#define TERRAIN_H

#include <OGRE/Ogre.h>
#include <OGRE/Terrain/OgreTerrain.h>
#include <OGRE/Terrain/OgreTerrainGroup.h>
#include <OGRE/OgreImage.h>

#include <sstream> // For intToStr. Move if the function is required somewhere else.

#include <algorithms/HeightMapGen.h>
#include "algorithms/random.h"
#include "floramanager.h"
#include "floraTree.h" // Probably just have this included in floraMain and include that instead

class Terrain
{
public:
    Terrain(Ogre::SceneManager* sceneManager, Ogre::Light *light);
    ~Terrain();
    Ogre::TerrainGroup* getTerrainGroup() { return mTerrainGroup; }

    void createFlatTerrain();
    void loadHeightmap();
    void generateTerrain();

    enum textureCatagory { TEX_GRASS, TEX_DIRT, TEX_ROCK, TEX_GENERIC };
    enum textureType { TT_NORMALMAP, TT_DIFFUSE };

    void setTexture(textureType texType, textureCatagory texCat, std::string filepath);
    void setTexturePlacementHeight(textureCatagory texCat, int newHeight);

    void clearTerrain();

private:
    Ogre::SceneManager* mSceneManager;
    Ogre::TerrainGlobalOptions* mTerrainGlobals;
    Ogre::TerrainGroup* mTerrainGroup;
    Ogre::Light* mSun;
    bool mTerrainsImported;

    std::string intToStr(int number);
    void defineTerrainFromFile(long x, long y);
    void initBlendMaps(Ogre::Terrain* terrain);
    void configureTerrainDefaults(Ogre::Light* light);
    void configureTextures(Ogre::Terrain::LayerInstanceList& layerList);

    // This should be defined before the terrain is generated. 4 is set as default for a small world.
    short unsigned int iTerrainSize;// = 4;
};

#endif // TERRAIN_H
