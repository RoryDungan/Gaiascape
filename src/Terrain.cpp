#include "Terrain.h"
#include <vector>
#include <cmath> // Needed only temporarily so that dimensions can be calculated without going through the heightmap generator
#include <QDesktopServices> // Also needed temporarily, so that the heightmap image can be outputted to a directory where ImageViewer can easily find it
#include <QDir> // Same as QDesktopServices
#include <QElapsedTimer> // Used for profiling
#include <QDebug>

Terrain::Terrain(Ogre::SceneManager* sceneManager, Ogre::Light *light)
{
    mSceneManager = sceneManager;
    mSun = light;

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

void Terrain::loadHeightmap(std::string imageFile)
{
    // If necessary, add resource group in directory
    QDir directory = QFileInfo(imageFile.c_str()).dir();
    if(!Ogre::ResourceGroupManager::getSingleton().resourceLocationExists(directory.path().toStdString()))
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(directory.path().toStdString(), "FileSystem");

    // Load image
    Ogre::Image img;
    img.load(QFileInfo(imageFile.c_str()).fileName().toStdString(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    mTerrainGlobals = new Ogre::TerrainGlobalOptions();

    // construct terrain group
    mTerrainGroup = new Ogre::TerrainGroup(mSceneManager, Ogre::Terrain::ALIGN_X_Z, img.getWidth(), 12000.0f);
    mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);

    configureTerrainDefaults(mSun);

    // define our terrains and instruct the TerrainGroup to load them all
    for(long x = 0; x <= 0; ++x)
        for(long y = 0; y <= 0; ++y)
            defineTerrainFromFile(x, y, img);

    // sync load since we want everything in place when we start
    mTerrainGroup->loadAllTerrains(true);

    // Calculate blendmaps for all terrains:
    Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
    while(ti.hasMoreElements())
    {
        Ogre::Terrain* t = ti.getNext()->instance;
        initBlendMaps(t);
    }

    // Now, all there is left to do is clean up after the initial terrain creation:
    mTerrainGroup->freeTemporaryResources();
}

// Size, talos and staggerValue are only used for generating terrains so they can e moved into this function instead of the Terrain constructor
void Terrain::generateTerrain(unsigned int seed, unsigned short size, unsigned short scale, unsigned short erosionIterations, unsigned short staggerValue, unsigned short treeDensity)
{
    // Timer for profiling
    QElapsedTimer timer;
    timer.start();

    // Terrain Size must be constant if tiles are to make any sense interacting with each other.
    iTerrainSize = size;
    // These do not need to be constant, but for large, consistent terrains, it doesn't make sense otherwise.
    iErosionIterations = erosionIterations;               // Number of times we run the erosion algorithm
    fStaggerValue = staggerValue; // The unevenness of the terrain

    // Dylan: I think it's better if this function generates all segments, given a number of segments to generate
    // x = The x position of the terrain we are generating
    // y = The y position of the terrain we are generating
    signed short x = 0, y = 0;

    // First, check to make sure the terrain doesn't already exist.
    if(getByLoc(x, y) != NULL)
    {
        std::cout << "Tried to generate terrain at x = " << x << " and y = " << y << " but terrain already exists there!\n";
        return;
    }
    mTerrainGlobals = new Ogre::TerrainGlobalOptions();

    qDebug() << "About to generate terrain -" << timer.elapsed() << "ms";

    //===============================
    // Generate a random heightmap
    //===============================
    // Firstly, generate the terrain data we're going to be working with
    // The reason why this looks weird is that all HMgen classes must start with what they are calculating,
    // in this case, a HM.

    Random::getSingleton().seed(seed); // First set the seed we'll be using for our random numbers

    delete HMHMgen;
    HMHMgen = new HeightMapGen(iTerrainSize, x, y, iErosionIterations, fStaggerValue, scale);

    // Convert that to an image
    Ogre::uchar* pStream = new Ogre::uchar[HMHMgen->iDimensions * HMHMgen->iDimensions];
    //Ogre::uchar stream[HMHMgen->iDimensions*HMHMgen->iDimensions];
    float* pHeightMap = HMHMgen->getHeightmap();
    std::cout << HMHMgen->iFinalPoint << std::endl;
    for(long unsigned int i = 0; i < HMHMgen->iFinalPoint + 1; ++i)
    {
        pStream[i] = (Ogre::uchar)(*(pHeightMap + i)*255); // Probably just put in the above if statement if it works
    }

    qDebug() << "Terrain created -" << timer.elapsed() << "ms";

    Ogre::Image img;
    img.loadDynamicImage(pStream, HMHMgen->iDimensions, HMHMgen->iDimensions, Ogre::PF_L8); // PF_L8 = 8-pit pixel format, all luminance

    qDebug() << "Converted to Ogre::Image -" << timer.elapsed() <<  "ms";

    img.save(std::string(QDesktopServices::storageLocation(QDesktopServices::TempLocation).toAscii() + QDir::separator().toAscii() + "gaiascape-heightmap.bmp"));

    qDebug() << "Image saved -" << timer.elapsed() << "ms";

    //===============================
    // Construct terrain group
    //===============================
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

    qDebug() << "Constructed terrain group -" << timer.elapsed() << "ms";

    //====================================================================
    // Configure default import settings for if we use imported image
    //====================================================================
    Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
    defaultimp.terrainSize = HMHMgen->iDimensions;
    defaultimp.worldSize = size;
    defaultimp.inputScale = scale;
    defaultimp.minBatchSize = 129;
    defaultimp.maxBatchSize = 129;

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

    qDebug() << "Configured default import settings -" << timer.elapsed() << "ms";

    //===========================
    // Construct Ogre terrain
    //===========================
    // define our terrains and instruct the TerrainGroup to load them all
    mTerrainGroup->defineTerrain(x, y, &img);

    // sync load since we want everything in place when we start - Do after all terrains have finished?
    mTerrainGroup->loadAllTerrains(true);

    // Calculate blendmaps for all terrains:
    Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
    while(ti.hasMoreElements())
    {
        Ogre::Terrain* t = ti.getNext()->instance;
        initBlendMaps(t);
    }

    qDebug() << "Loaded terrain -" << timer.elapsed() <<  "ms";

    // Now, all there is left to do is clean up after the initial terrain creation:
    mTerrainGroup->freeTemporaryResources();

    // Store the heightmap generator for later use. As of this point, this can actually be done fairly early on, but
    // it might be changed later on in this function.

    delete[] pStream; // clean up

    qDebug() << "Finished cleaning up -" << timer.elapsed() << "ms";
}

void Terrain::generateVegetation(unsigned int treesToGenerate, signed int x, signed int y)
{
    //=======================
    // Create Vegetation
    //=======================
    // This could be put before here depending on how texturing based on the slopemap will work.
    // Generate a slopemap based on heightMap
    // For each point in heightMap
    //  Create a probability of a tree spawning there based on:
    //      Altitude
    //      Proximity to other trees
    //      Slope

    float* pHeightMap = HMHMgen->getHeightmap();

    float* slopeMap = new float[HMHMgen->iDimensions * HMHMgen->iDimensions];
    HMHMgen->outputSlopemap(slopeMap);

    short signed int probability = 0;   // Probability a tree will spawn. If random returns equal to or below this number, it spawns.
    Ogre::Vector3 enterPos;             // The point the flora will be entering.
    short unsigned int randomNumber;    // A randomly generated number from 0 to 10. Should be changed to 0 to 100 for more depth.
    long unsigned int randomBlock;      // A randomly selected vertex from the terrain used to spawn a tree
    FloraTree* addedTree = NULL;

    mTerrainGroup->getTerrain(x, y)->getPoint(1%HMHMgen->iDimensions, 1/HMHMgen->iDimensions, &enterPos);

    for(long unsigned int i = 0; i < treesToGenerate; ++i)
    {
        try
        {
            // Treat 128, 0, 128 as the center of terrain
            // and 129 = terrainCenter or tC for short
            // Therefore at any point, the Vector3 position is tC + ((i%256) - 128), 0, tC + (floor(i/256) - 128)

            // Set up variables
            probability = 0;
            randomBlock = Random::getSingleton().getRand(0, HMHMgen->iFinalX + HMHMgen->iDimensions - 1);
            mTerrainGroup->getTerrain(x, y)->getPoint(randomBlock%HMHMgen->iDimensions, (randomBlock*2)/HMHMgen->iDimensions, &enterPos); // Puts the vector3 position of what's specified into enterPos

            // --------------------------
            // Height probability changes
            // --------------------------
            // Change this to Vector3 positions since some points will be higher/lower!
            // 0-10% = 50% prob
            // 11-30% = 40% prob
            // 31-50% = 30% prob
            // 51-90% = 10% prob
            if(*(pHeightMap + randomBlock) <= 0.9f/(HMHMgen->fScale/1800)) // Put an else probability = -1 if you want nothing to spawn above 90%
                probability += 1;
            if(*(pHeightMap + randomBlock) <= 0.5f/(HMHMgen->fScale/1800))
                probability += 2;
            if(*(pHeightMap + randomBlock) <= 0.3f/(HMHMgen->fScale/1800))
                probability += 1;
            if(*(pHeightMap + randomBlock) <= 0.1f/(HMHMgen->fScale/1800))
                probability += 1; // DISABLE if water spawns at a certain level, since we don't want vegetation on sand

            // Proximity to trees
            // < 90 = 0% prob - We don't want trees spawning too close to each other.
            // 91-450 = +30% prob
            if(probability != -1 && FloraManager::getSingletonPtr()->getFloraClosestToPoint(enterPos) <= 90) // 3 is arbitrary and needs to be adjusted to the scale of the model!
            {
                // Needs to be bigger
                probability = -1;
                std::cout << "Too close!\n";
            }
            if(probability != -1 && FloraManager::getSingletonPtr()->getFloraClosestToPoint(enterPos) <= 450) // Plants tend to gather together due to factors we don't calculate, so simullate this
            {
                std::cout << "Pretty close\n";
                probability += 3;
            }

            // Slope
            if(slopeMap[randomBlock] >= 0.1f/(HMHMgen->fScale/1800))
            {
                probability = -1;
            }

            // Run the probability
            randomNumber = Random::getSingleton().getRand(1, 10);
            if(randomNumber <= probability)
            {
                // Create a tree at this point!
                addedTree = new FloraTree("tree" + intToStr(randomBlock), mSceneManager, enterPos);
                FloraManager::getSingletonPtr()->addFlora(*addedTree);
            }
        }
        catch(std::exception &e)
        {
            std::cout << "Weird error occured\n";
        }
    }

    FloraManager::getSingletonPtr()->spawnGrass(mSceneManager);
    delete[] slopeMap; // clean up
}

std::string Terrain::intToStr(int number)
{
    std::stringstream ss;
    ss << number; // Put the integer into the magical stringstream box
    return ss.str(); // Return the string
}

HeightMapGen* Terrain::getByLoc(signed short x, signed short y)
{
    /*for(unsigned short i = 0; i < HMblocks.size(); i++)
    {
        if(HMblocks[i]->getX() == x && HMblocks[i]->getY() == y)
            return HMblocks[i];
    }*/
    return NULL;
    // STUB because this vector isn't in use yet
}

void Terrain::clearTerrain()
{
    // delete all terrain data
    FloraManager::getSingletonPtr()->removeAllFlora();
    delete mTerrainGroup;
    delete mTerrainGlobals;
}

void Terrain::defineTerrainFromFile(long x, long y, Ogre::Image& img)
{
    // get terrain image
    if(x % 2 != 0)
        img.flipAroundY();
    if(y % 2 != 0)
        img.flipAroundX();

    // Define Terrain here:
    mTerrainGroup->defineTerrain(x, y, &img);
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
    mTerrainGroup->getDefaultImportSettings().layerList = layerList;

}

void Terrain::replaceTexture(unsigned char index, float worldSize, std::string diffuseSpecular, std::string normalHeight)
{
    // If necessary, add resource group in directories
    QDir directory1 = QFileInfo(diffuseSpecular.c_str()).dir();
    if(!Ogre::ResourceGroupManager::getSingleton().resourceLocationExists(directory1.path().toStdString()))
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(directory1.path().toStdString(), "FileSystem");
    QDir directory2 = QFileInfo(diffuseSpecular.c_str()).dir();
    if(!Ogre::ResourceGroupManager::getSingleton().resourceLocationExists(directory2.path().toStdString()))
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(directory2.path().toStdString(), "FileSystem");

    Ogre::StringVector textureNames;
    textureNames.push_back(QFileInfo(diffuseSpecular.c_str()).fileName().toStdString());
    textureNames.push_back(QFileInfo(normalHeight.c_str()).fileName().toStdString());

    // Cycle through the the TerrainGroup and change the image on all terrains
    Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
    while(ti.hasMoreElements())
    {
        Ogre::Terrain* t = ti.getNext()->instance;
        t->replaceLayer(index, true, worldSize, &textureNames);
        t->dirty();
        t->update();
    }
}

QImage Terrain::getQImage(long xIndex, long yIndex)
{
    // Get the terrain we will be exporting
    Ogre::Terrain* terrain = mTerrainGroup->getTerrain(xIndex, yIndex);

    // Convert the array of floats to an array of unsigned chars (values between 0 and 255)
    float minHeight = terrain->getMinHeight(); // this will be 0
    float scale = 255.f / terrain->getMaxHeight(); // highest point in the terrain will be 255

    // Transfer the data
    uchar* image = new uchar[terrain->getSize() * terrain->getSize() * 3 + 1]; // 3 channels
    int pixelIndex = 0;
    for(int x = 0; x < terrain->getSize() * terrain->getSize(); x++)
    {
        float data = terrain->getHeightData()[x];// *(terrain->getHeightData() + x);
        uchar converted = (data - minHeight) * scale;
        // 3 times, once for the Red channel, one for Green and one for Blue
        image[pixelIndex] = converted;  pixelIndex++;
        std::cout << (int)converted << '\n';
        //image[pixelIndex] = converted;  pixelIndex++;
        //image[pixelIndex] = converted;  pixelIndex++;
    }

    QImage final(image, terrain->getSize(), terrain->getSize(), QImage::Format_Indexed8);

    delete[] image; // clean up

    return final;
}

