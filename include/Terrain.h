#ifndef TERRAIN_H
#define TERRAIN_H

#ifndef Q_MOC_RUN
#include <OGRE/Ogre.h>
#include <OGRE/Terrain/OgreTerrain.h>
#include <OGRE/Terrain/OgreTerrainGroup.h>
#include <OGRE/OgreImage.h>
#endif // Q_MOC_RUN

#include <sstream> // For intToStr. Move if the function is required somewhere else.
#include <vector> // For the HMblocks vector

#include "algorithms/HeightMapGen.h"
#include "FloraManager.h"
#include "FloraTree.h" // Probably just have this included in FloraMain and include that instead
#include <QImage>

class Terrain
{
public:
    Terrain(Ogre::SceneManager* sceneManager, Ogre::Light *light);
    ~Terrain();
    Ogre::TerrainGroup* getTerrainGroup() { return mTerrainGroup; }

    QImage getQImage(long xIndex, long yIndex);

    void createFlatTerrain();
    void loadHeightmap(std::string imageFile);
    void generateTerrain(unsigned int seed, unsigned short size, unsigned short scale, unsigned short erosionIterations, unsigned short staggerValue, unsigned short treeDensity);

    void generateVegetation(unsigned int treesToGenerate, signed int x, signed int y);

    void replaceTexture(unsigned char index, float worldSize, std::string diffuseSpecular, std::string normalHeight);

    void clearTerrain();

private:
    Ogre::SceneManager* mSceneManager;
    Ogre::TerrainGlobalOptions* mTerrainGlobals;
    Ogre::TerrainGroup* mTerrainGroup;
    Ogre::Light* mSun;

    std::string intToStr(int number);
    HeightMapGen* getByLoc(signed short x, signed short y);
    void defineTerrainFromFile(long x, long y, Ogre::Image &img);
    void initBlendMaps(Ogre::Terrain* terrain);
    void configureTerrainDefaults(Ogre::Light* light);
    void configureTextures(Ogre::Terrain::LayerInstanceList& layerList);

    // List of heightmaps we have
    //std::vector<HeightMapGen*> HMblocks; - DISABLED SINCE ITS USELESS AND CAUSING PROBLEMS
    HeightMapGen* HMHMgen;
    // These should be defined before the terrain is generated.
    short unsigned int iTerrainSize;
    float fTalos;
    short unsigned int iErosionIterations;
    float fStaggerValue;
    short unsigned int iBlocks;
};

#endif // TERRAIN_H
