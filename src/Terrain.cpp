#include "Terrain.h"
#include <math.h> // Needed only temporarily so that dimensions can be calculated without going through the heightmap generator

Terrain::Terrain(Ogre::SceneManager* sceneManager, Ogre::Light* light)
{
    // Workaround by Rory - this needs to be set outside of the header file
    iTerrainSize = 8;

    mSceneManager = sceneManager;
    mSun = light;
    //mTerrainGlobals = new Ogre::TerrainGlobalOptions();

    createFlatTerrain();
}

Terrain::~Terrain()
{
    if(mTerrainGroup)   delete mTerrainGroup;
    if(mTerrainGlobals) delete mTerrainGlobals;
}

void Terrain::createFlatTerrain()
{
    mTerrainGlobals = new Ogre::TerrainGlobalOptions();

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
    mTerrainGlobals = new Ogre::TerrainGlobalOptions();

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

void Terrain::generateTerrain()
{
    mTerrainGlobals = new Ogre::TerrainGlobalOptions();

    // Firstly, generate the terrain data we're going to be working with
    // The reason why this looks weird is that all HMgen classes must start with what they are calculating,
    // in this case, a HM.
    HeightMapGen HMHMgen(0.0f, iTerrainSize);
    // Set the array to be filled with zeroes, and be flat.
    // Set up the dimensions of the array
    short unsigned int iDimensions = pow(4.0, iTerrainSize * 0.5) + 1;
    float heightMap[iDimensions*iDimensions];
    for(long unsigned int i = 0; i < iDimensions*iDimensions; i++)
        heightMap[i] = 0;

    HMHMgen.retrieveHeightmap(1, iTerrainSize, &heightMap[0]); // Talos is set to something random for a defualt until erosion is added

    Ogre::uchar stream[iDimensions*iDimensions];
    long unsigned int iFinalX = (iDimensions - 1)*iDimensions;
    float* pHeightMap = &heightMap[0];
    for(long unsigned int i = 0; i < iFinalX + iDimensions - 1; ++i)
    {
        stream[i] = (Ogre::uchar)(*(pHeightMap + i)*255); // Probably just put in the above if statement if it works
    }
    Ogre::uchar* pStream = &stream[0];

    Ogre::Image img;
    img.loadDynamicImage(pStream, iDimensions, iDimensions, Ogre::PF_L8); // PF_L8 = 8-pit pixel format, all luminance
    img.save("map.bmp");

    // How generating the terrain will work
    //  Because we cannot conveniently return an array, instead the array is created here.
    //  We then pass a pointer of that array to the function which generates the terrain

    // construct terrain group
    mTerrainGroup = new Ogre::TerrainGroup(mSceneManager, Ogre::Terrain::ALIGN_X_Z, iDimensions, 12000.0f);
    //mTerrainGroup->setFilenameConvention(Ogre::String("Terrain"), Ogre::String("dat"));
    mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

    configureTerrainDefaults(mSun);

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
    defaultimp.terrainSize = iDimensions;
    defaultimp.worldSize = 12000.0f;
    defaultimp.inputScale = 1800;
    defaultimp.minBatchSize = 129;
    defaultimp.maxBatchSize = 129;
    // defaultimp.inputFloat = &array[0];

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

    // define our terrains and instruct the TerrainGroup to load them all
    for(long x = 0; x <= 0; ++x)
        for(long y = 0; y <= 0; ++y)
        {
            mTerrainGroup->defineTerrain(x, y, &img);
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

    // -----------------
    // Create Vegetation
    // -----------------
    // This could be put before here depending on how texturing based on the slopemap will work.
    // Generate a slopemap based on heightMap
    // For each point in heightMap
    //  Create a probability of a tree spawning there based on:
    //      Altitude
    //      Proximity to other trees
    //      Slope
    float slopeMap[iDimensions*iDimensions];
    HMHMgen.retrieveSlopemap(&slopeMap[0], &heightMap[0], iDimensions);
    short unsigned int iProbability = 0; // Probability a tree will spawn. If random returns equal to or below this number, it spawns.

    Ogre::Vector3 enterPos; // The point the flora will be entering.
    short unsigned int iRandomNumber; // A randomly generated number from 0 to 10. Should be changed to 0 to 100 for more depth.
    short unsigned int treesToGenerate = 1000; // A constant that says how many iterations of the for loop we want to go through
    long unsigned int iRandomBlock; // A randomly selected vertex from the terrain used to spawn a tree
    floraTree* addedTree;

    std::cout << "1%iDimensions = " << 1%iDimensions << "\n1/iDimensions = " << 1/iDimensions << "\n";
    mTerrainGroup->getTerrain(0, 0)->getPoint(1%iDimensions, 1/iDimensions, &enterPos);


    for(long unsigned int i = 0; i < treesToGenerate; ++i)
    {
        // Treat 128, 0, 128 as the center of terrain
        // and 129 = terrainCenter or tC for short
        // Therefore at any point, the Vector3 position is tC + ((i%256) - 128), 0, tC + (floor(i/256) - 128)
        // @ i = 0, the Vector3 position would be 128 - 128, 0, 128 - 128
        // Then at i = 257, the Vector3 position would be 128 - 127, 0, 128 - 127

        // Set up variables
        iProbability = 0;
        iRandomBlock = Random::getSingleton().getRand(0,iFinalX + iDimensions - 1);
        mTerrainGroup->getTerrain(0, 0)->getPoint(iRandomBlock%iDimensions, iRandomBlock/iDimensions, &enterPos);

        // --------------------------
        // Height probability changes
        // --------------------------
        // 0-10% = 50% prob
        // 11-30% = 40% prob
        // 31-50% = 30% prob
        // 51-90% = 10% prob
        if(heightMap[iRandomBlock] <= 0.9) // Put an else iProbability = -1 if you want nothing to spawn above 90%
            iProbability += 1;
        if(heightMap[iRandomBlock] <= 0.5f)
            iProbability += 2;
        if(heightMap[iRandomBlock] <= 0.3f)
            iProbability += 1;
        if(heightMap[iRandomBlock] <= 0.1f)
            iProbability += 1; // DISABLE if water spawns at a certain level, since we don't want vegetation on sand

        // Proximity to trees - Disabled because the for loop isn't functioning correctly
        // < 3 = 0% prob
        // 3.1-10 = +30% prob
        if(iProbability != -1 && floraManager::getSingletonPtr()->getFloraClosestToPoint(enterPos) <= 3) // 3 is arbitrary and needs to be adjusted to the scale of the model!
        {
            std::cout << "Too close!\n";
            iProbability = -1;
        }
        if(iProbability != -1 && floraManager::getSingletonPtr()->getFloraClosestToPoint(enterPos) <= 10) // Plants tend to gather together due to factors we don't calculate, so simullate this
        {
            std::cout << "Pretty close\n";
            iProbability += 3;
        }

        // Slope
        if(slopeMap[iRandomBlock] >= 0.1f)
        {
            iProbability = -1;
        }

        // Run the probability
        iRandomNumber = Random::getSingleton().getRand(1, 10);
        if(iRandomNumber <= iProbability)
        {
            // Create a tree at this point!
            addedTree = new floraTree("tree" + intToStr(iRandomBlock), mSceneManager, enterPos);
            floraManager::getSingletonPtr()->addFlora(*addedTree);
        }
    }

    // Generate an image of slopeMap for diagnostic purposes
    Ogre::uchar stream2[iDimensions*iDimensions];
    float* pSlopeMap = &slopeMap[0];
    for(long unsigned int i = 0; i < iFinalX + iDimensions - 1; ++i)
    {
        stream2[i] = (Ogre::uchar)(*(pSlopeMap + i)*255); // Probably just put in the above if statement if it works
    }
    Ogre::uchar* pStream2 = &stream2[0];

    Ogre::Image img2;
    img2.loadDynamicImage(pStream2, iDimensions, iDimensions, Ogre::PF_L8); // PF_L8 = 8-pit pixel format, all luminance
    img2.save("slopeMap.bmp");
}

std::string Terrain::intToStr(int number)
{
    std::stringstream ss;
    ss << number; // Put the integer into the magical stringstream box
    return ss.str(); // Return the string
}

void Terrain::clearTerrain()
{
    // delete all terrain data
    //mTerrainGroup->removeAllTerrains();
    delete mTerrainGroup;
    delete mTerrainGlobals;
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
        img.load("terrain1.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
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
    //defaultimp.layerList[2].textureNames.push_back("grass-01_diffusespecular.png");
}

void Terrain::configureTextures(Ogre::Terrain::LayerInstanceList& layerList)
{

}

void Terrain::setTexture(textureType newTexture, textureCatagory texCat, std::string filepath)
{
    // Change one of the textures
}

void Terrain::setTexturePlacementHeight(textureCatagory texCat, int newHeight)
{
    // Recalculate blendmaps, changing the placement height for one texture
}
