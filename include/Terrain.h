#ifndef TERRAIN_H
#define TERRAIN_H

#include <OGRE/Ogre.h>
#include <OGRE/Terrain/OgreTerrain.h>
#include <OGRE/Terrain/OgreTerrainGroup.h>
#include <OGRE/OgreImage.h>

#include <sstream> // For intToStr. Move if the function is required somewhere else.
#include <vector> // For the HMblocks vector

#include "algorithms/HeightMapGen.h"
#include "FloraManager.h"
#include "FloraTree.h" // Probably just have this included in FloraMain and include that instead

class Terrain
{
public:
    Terrain(Ogre::SceneManager* sceneManager, Ogre::Light *light, short unsigned int size, short unsigned int talos, short unsigned int staggerValue);
    ~Terrain();
    Ogre::TerrainGroup* getTerrainGroup() { return mTerrainGroup; }

    void createFlatTerrain();
    void loadHeightmap();
    void generateTerrain(short signed int x, short signed int y);

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
    HeightMapGen* getByLoc(signed short x, signed short y);
    void defineTerrainFromFile(long x, long y);
    void initBlendMaps(Ogre::Terrain* terrain);
    void configureTerrainDefaults(Ogre::Light* light);
    void configureTextures(Ogre::Terrain::LayerInstanceList& layerList);

    // List of heightmaps we have
    std::vector<HeightMapGen*> HMblocks;

    // These should be defined before the terrain is generated.
    short unsigned int iTerrainSize;
    short unsigned int iTalos;
    short unsigned int iBlocks;
    short unsigned int iStaggerValue;
};

#endif // TERRAIN_H
