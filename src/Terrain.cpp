#include "Terrain.h"
#include <cmath> // Needed only temporarily so that dimensions can be calculated without going through the heightmap generator
#include <QDesktopServices> // Also needed temporarily, so that the heightmap image can be outputted to a directory where ImageViewer can easily find it
#include <QDir> // Same as QDesktopServices

Terrain::Terrain(Ogre::SceneManager* sceneManager, Ogre::Light *light, short unsigned int size, short unsigned int talos, short unsigned int staggerValue)
{
    // Terrain Size must be constant if tiles are to make any sense interacting with each other.
    iTerrainSize = size;
    // These do not need to be constant, but for large, consistent terrains, it doesn't make sense otherwise.
    iTalos = talos;               // Minimum angle where thermal erosion takes place
    iStaggerValue = staggerValue; // The unevenness of the terrain

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


// x = The x position of the terrain we are generating
// y = The y position of the terrain we are generating

void Terrain::generateTerrain(signed short x, signed short y)
{
    // First, check to make sure the terrain doesn't already exist.
    if(getByLoc(x, y) != NULL)
    {
        std::cout << "Tried to generate terrain at x = " << x << " and y = " << y << " but terrain already exists there!\n";
        return;
    }
    mTerrainGlobals = new Ogre::TerrainGlobalOptions();

    // Firstly, generate the terrain data we're going to be working with
    // The reason why this looks weird is that all HMgen classes must start with what they are calculating,
    // in this case, a HM.
    HeightMapGen* HMHMgen = new HeightMapGen(iTerrainSize, x, y, iTalos, iStaggerValue);

    // Convert that to an image
    Ogre::uchar stream[HMHMgen->iDimensions*HMHMgen->iDimensions];
    float* pHeightMap = HMHMgen->getHeightmap();
    for(long unsigned int i = 0; i < HMHMgen->iFinalX + HMHMgen->iDimensions - 1; ++i)
    {
        stream[i] = (Ogre::uchar)(*(pHeightMap + i)*255); // Probably just put in the above if statement if it works
    }
    Ogre::uchar* pStream = &stream[0];

    Ogre::Image img;
    img.loadDynamicImage(pStream, HMHMgen->iDimensions, HMHMgen->iDimensions, Ogre::PF_L8); // PF_L8 = 8-pit pixel format, all luminance
    img.save(std::string(QDesktopServices::storageLocation(QDesktopServices::TempLocation).toAscii() + QDir::separator().toAscii() + "gaiascape-heightmap.bmp"));

    // construct terrain group
    mTerrainGroup = new Ogre::TerrainGroup(mSceneManager, Ogre::Terrain::ALIGN_X_Z, HMHMgen->iDimensions, 12000.0f);
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
    defaultimp.terrainSize = HMHMgen->iDimensions;
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
    mTerrainGroup->defineTerrain(x, y, &img);
    mTerrainsImported = true;

    // sync load since we want everything in place when we start - Do after all terrains have finished?
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
    float slopeMap[HMHMgen->iDimensions*HMHMgen->iDimensions];
    HMHMgen->outputSlopemap(&slopeMap[0]);
    short signed int iProbability = 0; // Probability a tree will spawn. If random returns equal to or below this number, it spawns.

    Ogre::Vector3 enterPos; // The point the flora will be entering.
    short unsigned int iRandomNumber; // A randomly generated number from 0 to 10. Should be changed to 0 to 100 for more depth.
    short unsigned int treesToGenerate = 1000; // A constant that says how many iterations of the for loop we want to go through
    long unsigned int iRandomBlock; // A randomly selected vertex from the terrain used to spawn a tree
    FloraTree* addedTree;

    mTerrainGroup->getTerrain(0, 0)->getPoint(1%HMHMgen->iDimensions, 1/HMHMgen->iDimensions, &enterPos);

    for(long unsigned int i = 0; i < treesToGenerate; ++i)
    {
        try
        {
            // Treat 128, 0, 128 as the center of terrain
            // and 129 = terrainCenter or tC for short
            // Therefore at any point, the Vector3 position is tC + ((i%256) - 128), 0, tC + (floor(i/256) - 128)

            // Set up variables
            iProbability = 0;
            iRandomBlock = Random::getSingleton().getRand(0, HMHMgen->iFinalX + HMHMgen->iDimensions - 1);
            mTerrainGroup->getTerrain(0, 0)->getPoint(iRandomBlock%HMHMgen->iDimensions, (iRandomBlock*2)/HMHMgen->iDimensions, &enterPos); // Puts the vector3 position of what's specified into enterPos

            // --------------------------
            // Height probability changes
            // --------------------------
            // Change this to Vector3 positions since some points will be higher/lower!
            // 0-10% = 50% prob
            // 11-30% = 40% prob
            // 31-50% = 30% prob
            // 51-90% = 10% prob
            if(*(pHeightMap + iRandomBlock) <= 0.9) // Put an else iProbability = -1 if you want nothing to spawn above 90%
                iProbability += 1;
            if(*(pHeightMap + iRandomBlock) <= 0.5f)
                iProbability += 2;
            if(*(pHeightMap + iRandomBlock) <= 0.3f)
                iProbability += 1;
            if(*(pHeightMap + iRandomBlock) <= 0.1f)
                iProbability += 1; // DISABLE if water spawns at a certain level, since we don't want vegetation on sand

            // Proximity to trees
            // < 90 = 0% prob - We don't want trees spawning too close to each other.
            // 91-450 = +30% prob
            if(iProbability != -1 && FloraManager::getSingletonPtr()->getFloraClosestToPoint(enterPos) <= 90) // 3 is arbitrary and needs to be adjusted to the scale of the model!
            {
                // Needs to be bigger
                iProbability = -1;
                std::cout << "Too close!\n";
            }
            if(iProbability != -1 && FloraManager::getSingletonPtr()->getFloraClosestToPoint(enterPos) <= 450) // Plants tend to gather together due to factors we don't calculate, so simullate this
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
                addedTree = new FloraTree("tree" + intToStr(iRandomBlock), mSceneManager, enterPos);
                FloraManager::getSingletonPtr()->addFlora(*addedTree);
            }
        }
        catch(std::exception &e)
        {
            std::cout << "Weird error occured\n"; // This hasn't been called for a while and may not be necessary any more
        }
    }

    // Store the heightmap generator for later use. As of this point, this can actually be done fairly early on, but
    // it might be changed later on in this function.

}

std::string Terrain::intToStr(int number)
{
    std::stringstream ss;
    ss << number; // Put the integer into the magical stringstream box
    return ss.str(); // Return the string
}

HeightMapGen* Terrain::getByLoc(signed short x, signed short y)
{
    for(unsigned short i = 0; i < HMblocks.size(); i++)
    {
        if(HMblocks[i]->getX() == x && HMblocks[i]->getY() == y)
            return HMblocks[i];
    }
    return NULL;
}

void Terrain::clearTerrain()
{
    // delete all terrain data
    //mTerrainGroup->removeAllTerrains();
    FloraManager::getSingletonPtr()->removeAllFlora();
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
