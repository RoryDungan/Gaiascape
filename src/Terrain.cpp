#include "Terrain.h"

Terrain::Terrain(Ogre::SceneManager* sceneManager, Ogre::Light* light)
{
    // Workaround by Rory - this needs to be set outside of the header file
    iTerrainSize = 8;

    mSceneManager = sceneManager;
    mSun = light;
    mTerrainGlobals = new Ogre::TerrainGlobalOptions();

    createFlatTerrain();
}

Terrain::~Terrain()
{
    delete mTerrainGroup;
    delete mTerrainGlobals;
}

void Terrain::createFlatTerrain()
{
    // construct terrain group
    mTerrainGroup = new Ogre::TerrainGroup(mSceneManager, Ogre::Terrain::ALIGN_X_Z, 513, 12000.0f);
    mTerrainGroup->setFilenameConvention(Ogre::String("Terrain"), Ogre::String("dat"));
    mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

    configureTerrainDefaults(mSun);

    // define our terrains and instruct the TerrainGroup to load them all
    for(long x = 0; x <= 0; ++x)
        for(long y = 0; y <= 0; ++y)
            mTerrainGroup->defineTerrain(x, y, 0.0f);

    // sync load since we want everything in place when we start
    mTerrainGroup->loadAllTerrains(true);

    // Calculate blendmaps
    Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
    while(ti.hasMoreElements())
    {
        Ogre::Terrain* t = ti.getNext()->instance;
        initBlendMaps(t);
    }

    // Now, all there is left to do is clean up after the initial terrain creation:
    mTerrainGroup->freeTemporaryResources();
}

void Terrain::loadHeightmap()
{
    // construct terrain group
    mTerrainGroup = new Ogre::TerrainGroup(mSceneManager, Ogre::Terrain::ALIGN_X_Z, 513, 12000.0f);
    mTerrainGroup->setFilenameConvention(Ogre::String("Terrain"), Ogre::String("dat"));
    mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

    configureTerrainDefaults(mSun);

    // define our terrains and instruct the TerrainGroup to load them all
    for(long x = 0; x <= 0; ++x)
        for(long y = 0; y <= 0; ++y)
            defineTerrainFromFile(x, y);

    // sync load since we want everything in place when we start
    mTerrainGroup->loadAllTerrains(true);

    // Now, if we just imported our terrains, we would like our blendmaps to be calculated:
    /*if(mTerrainsImported)
    {
        Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
        while(ti.hasMoreElements())
        {
            Ogre::Terrain* t = ti.getNext()->instance;
            initBlendMaps(t);
        }
    }*/

    // Now, all there is left to do is clean up after the initial terrain creation:
    mTerrainGroup->freeTemporaryResources();
}

void Terrain::generateTerrain()
{
    // Firstly, generate the terrain data we're going to be working with
    // The reason why this looks weird is that all HMgen classes must start with what they are calculating,
    // in this case, a HM.
    HeightMapGen HMHMgen(0.0f, iTerrainSize);

    std::cout << "HMHMgen.retrieveDimensions(): " << HMHMgen.retrieveDimensions() << std::endl;

    // Convert the array of TerrainBlocks to an array of floats
    float array[HMHMgen.retrieveDimensions()][HMHMgen.retrieveDimensions()];
    for(short unsigned int x; x <= HMHMgen.retrieveDimensions(); x++)
        for(short unsigned int y; y <= HMHMgen.retrieveDimensions(); y++)
            array[x][y] = HMHMgen.getByLoc(x, y)->getHeight();

    // construct terrain group
    mTerrainGroup = new Ogre::TerrainGroup(mSceneManager, Ogre::Terrain::ALIGN_X_Z, HMHMgen.retrieveDimensions(), 12000.0f);
    mTerrainGroup->setFilenameConvention(Ogre::String("Terrain"), Ogre::String("dat"));
    mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

    //configureTerrainDefaults(mSun);


    // Configure global
    mTerrainGlobals->setMaxPixelError(8);
    // testing composite map
    mTerrainGlobals->setCompositeMapDistance(3000);

    // Important to set these so that the terrain knows what to use for derived (non-realtime) data
    mTerrainGlobals->setLightMapDirection(mSun->getDerivedDirection());
    mTerrainGlobals->setCompositeMapAmbient(mSceneManager->getAmbientLight());
    mTerrainGlobals->setCompositeMapDiffuse(mSun->getDiffuseColour());

    // Configure default import settings for if we use imported image
    Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
    defaultimp.terrainSize = HMHMgen.retrieveDimensions();
    defaultimp.worldSize = 12000.0f;
    defaultimp.inputScale = 600; // due terrain.png is 8bpp
    defaultimp.minBatchSize = 33;
    defaultimp.maxBatchSize = 65;
    //defaultimp.inputFloat = &array[0][0];

    // Add textures
    defaultimp.layerList.resize(3);
    defaultimp.layerList[0].worldSize = 100;
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
    defaultimp.layerList[1].worldSize = 30;
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
    defaultimp.layerList[2].worldSize = 200;
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");

    float* ptr = &array[0][0];

    // define our terrains and instruct the TerrainGroup to load them all
    for(long x = 0; x <= 0; ++x)
        for(long y = 0; y <= 0; ++y)
        {
            mTerrainGroup->defineTerrain(x, y, ptr);
            mTerrainsImported = true;
        }

    // sync load since we want everything in place when we start
    mTerrainGroup->loadAllTerrains(true);

    // Now, if we just imported our terrains, we would like our blendmaps to be calculated:
    if(mTerrainsImported)
    {
        Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
        while(ti.hasMoreElements())
        {
            Ogre::Terrain* t = ti.getNext()->instance;
            initBlendMaps(t);
        }
    }

    // Now, all there is left to do is clean up after the initial terrain creation:
    mTerrainGroup->freeTemporaryResources();
}

void Terrain::clearTerrain()
{
    // delete all terrain data
    //mTerrainGroup->removeAllTerrains();
    delete mTerrainGroup;
}

void Terrain::defineTerrainFromFile(long x, long y)
{
    Ogre::String filename = mTerrainGroup->generateFilename(x, y);
    if(Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename))
        // Define terrain here:
        mTerrainGroup->defineTerrain(x, y);
    else
    {
        // get terrain image
        Ogre::Image img;
        img.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        if(x % 2 != 0)
            img.flipAroundY();
        if(y % 2 != 0)
            img.flipAroundX();

        // Define Terrain here:
        mTerrainGroup->defineTerrain(x, y, &img);
        mTerrainsImported = true;
    }
}

void Terrain::initBlendMaps(Ogre::Terrain *terrain)
{
    Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
    Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
    Ogre::Real minHeight0 = 70;
    Ogre::Real fadeDist0 = 40;
    Ogre::Real minHeight1 = 70;
    Ogre::Real fadeDist1 = 15;
    float* pBlend0 = blendMap0->getBlendPointer();
    float* pBlend1 = blendMap1->getBlendPointer();
    for(Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
    {
        for(Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
        {
            Ogre::Real tx, ty;

            blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
            Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
            Ogre::Real val = (height - minHeight0) / fadeDist0;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend0++ = val;

            val = (height - minHeight1) / fadeDist1;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend1++ = val;
        }
    }
    blendMap0->dirty();
    blendMap1->dirty();
    blendMap0->update();
    blendMap1->update();
}

void Terrain::configureTerrainDefaults(Ogre::Light *light)
{
    // Configure global
    mTerrainGlobals->setMaxPixelError(8);
    // testing composite map
    mTerrainGlobals->setCompositeMapDistance(3000);

    // Important to set these so that the terrain knows what to use for derived (non-realtime) data
    mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
    mTerrainGlobals->setCompositeMapAmbient(mSceneManager->getAmbientLight());
    mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());

    // Configure default import settings for if we use imported image
    Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
    defaultimp.terrainSize = 513;
    defaultimp.worldSize = 12000.0f;
    defaultimp.inputScale = 600; // due terrain.png is 8bpp
    defaultimp.minBatchSize = 33;
    defaultimp.maxBatchSize = 65;

    // Add textures
    defaultimp.layerList.resize(3);
    defaultimp.layerList[0].worldSize = 100;
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
    defaultimp.layerList[1].worldSize = 30;
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
    defaultimp.layerList[2].worldSize = 200;
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
}

void Terrain::configureTextures(Ogre::Terrain::LayerInstanceList& layerList)
{

}
