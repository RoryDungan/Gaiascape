#include "OgreWidget.h"
#include <QDebug>
#include <QFileInfo>
#include <QDir>

#define THIS OgreWidget

/**
 * @brief init the object
 * @author kito berg-taylor
 */
void THIS::init( std::string logFile,
         std::string renderSystem)
{
    // create the main ogre object
#ifdef _DEBUG
    mOgreRoot = new Ogre::Root( "plugins_d.cfg", "", logFile );
#else
    mOgreRoot = new Ogre::Root( "plugins.cfg", "", logFile );
#endif

    // setup a renderer
    Ogre::RenderSystemList::const_iterator renderers = mOgreRoot->getAvailableRenderers().begin();
    while(renderers != mOgreRoot->getAvailableRenderers().end())
    {
        Ogre::String rName = (*renderers)->getName();
        if (rName == renderSystem + " Rendering Subsystem")
            break;
        renderers++;
    }

    Ogre::RenderSystem *ogreRenderSystem = *renderers;
    mOgreRoot->setRenderSystem(ogreRenderSystem);
    QString dimensions = QString( "%1x%2" )
                    .arg(this->width())
                    .arg(this->height());

    ogreRenderSystem->setConfigOption( "Video Mode", dimensions.toStdString() );

    // initialize without creating window
    mOgreRoot->getRenderSystem()->setConfigOption( "Full Screen", "No" );
    mOgreRoot->saveConfig();
    mOgreRoot->initialise(false); // don't create a window

    mCameraMovement = Ogre::Vector3(0.0f, 0.0f, 0.0f);
    mHeightUpdateCountDown = 0.0f;
    mHeightUpdateRate = 0.05f; // 1/20, or 20 fps
    bCtrlPressed = false;
    mInteractionMode = IM_SELECT;
    mCurrentState = IS_IDLE;
    mBrushSize = 0.1f;
    mBrushWeight = 250;
    mLayerEdit = 1;

    // Set up resources
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("media", "FileSystem");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("media/models", "FileSystem");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("media/materials", "FileSystem");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("media/textures", "FileSystem");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("media/textures/nvidia", "FileSystem");
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("media/textures/irrskybox", "FileSystem");
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

/**
 * @brief setup the rendering context
 * @author Kito Berg-Taylor
 */
void THIS::initializeGL()
{
    //== Creating and Acquiring Ogre Window ==//

    // Get the parameters of the window QT created
    Ogre::String winHandle;
#ifdef WIN32
    // Windows code
    winHandle += Ogre::StringConverter::toString((unsigned long)(this->parentWidget()->winId()));
#elif MACOS
    // Mac code, tested on Mac OSX 10.6 using Qt 4.7.4 and Ogre 1.7.3
    Ogre::String winHandle  = Ogre::StringConverter::toString(winId());
#else
    // Unix code
    QX11Info info = x11Info();
    winHandle  = Ogre::StringConverter::toString((unsigned long)(info.display()));
    winHandle += ":";
    winHandle += Ogre::StringConverter::toString((unsigned int)(info.screen()));
    winHandle += ":";
    winHandle += Ogre::StringConverter::toString((unsigned long)(this->parentWidget()->winId()));
#endif


    Ogre::NameValuePairList params;
#ifndef MACOS
    // code for Windows and Linux
    params["parentWindowHandle"] = winHandle;
    mOgreWindow = mOgreRoot->createRenderWindow( "QOgreWidget_RenderWindow",
                           this->width(),
                           this->height(),
                           false,
                           &params );

    mOgreWindow->setActive(true);
    WId ogreWinId = 0x0;
    mOgreWindow->getCustomAttribute( "WINDOW", &ogreWinId );

    assert( ogreWinId );

    // bug fix, extract geometry
    QRect geo = this->frameGeometry ( );

    // create new window
    this->create( ogreWinId );

    // set geometrie infos to new window
    this->setGeometry (geo);

#else
    // code for Mac
    params["externalWindowHandle"] = winHandle;
    params["macAPI"] = "cocoa";
    params["macAPICocoaUseNSView"] = "true";
    mOgreWindow = mOgreRoot->createRenderWindow("QOgreWidget_RenderWindow",
          width(), height(), false, &params);
    mOgreWindow->setActive(true);
    makeCurrent();
#endif

    setAttribute( Qt::WA_PaintOnScreen, true );
    setAttribute( Qt::WA_NoBackground );

    //== Ogre Initialization ==//
    Ogre::SceneType scene_manager_type = Ogre::ST_EXTERIOR_CLOSE;

    mSceneMgr = mOgreRoot->createSceneManager( scene_manager_type );
    mSceneMgr->setAmbientLight( Ogre::ColourValue(1,1,1) );

    mCamera = mSceneMgr->createCamera( "QOgreWidget_Cam" );
    mCamera->setPosition( Ogre::Vector3(0,1,0) );
    mCamera->lookAt( Ogre::Vector3(0,0,0) );
    mCamera->setNearClipDistance( 0.1 );
    if(mOgreRoot->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE))
        mCamera->setFarClipDistance(0);
    else
        mCamera->setFarClipDistance(50000);

    mViewport = mOgreWindow->addViewport( mCamera );
    mViewport->setBackgroundColour( Ogre::ColourValue( 0.8,0.8,1 ) );

    mCamera->setAutoAspectRatio(true);
    //mCamera->setFOVy(mCamFOV);

    setupScene();
}

/**
 * @brief Set the vertical field of view
 * @author Rory Dungan
 */
void THIS::setFOVy(float fov)
{
    if(mOgreWindow->isActive()) mCamera->setFOVy(Ogre::Degree(fov));
    else mCamFOV = Ogre::Degree(10.0f);
}

/**
 * @brief Handle mouse input
 * @author Rory Dungan
 */
void THIS::mousePressEvent(QMouseEvent * event)
{
    mLastCursorPos = event->pos();

    switch(event->button())
    {
    case Qt::LeftButton:
        if(mCurrentState == IS_IDLE)
            switch(mInteractionMode)
            {
            case IM_EXTRUDE:
                mPaintTimer.start();
                mCurrentState = IS_EDITING_HEIGHT;
                break;
            case IM_INTRUDE:
                mCurrentState = IS_EDITING_HEIGHT;
                break;
            case IM_PAINT:
                mCurrentState = IS_PAINTING;
                break;
            case IM_PLACEOBJ:
                mCurrentState = IS_PLACING_OBJECTS;
                break;
            default: break;
            }
        break;
    case Qt::RightButton:
        if(!bCtrlPressed)
            mCurrentState = IS_ROTATING_CAMERA;
        else
            mCurrentState = IS_MOVING_CAMERA;
        break;

    case Qt::MiddleButton:
        mCurrentState = IS_MOVING_CAMERA;
        break;

    default:
        break;
    }
}

/**
 * @brief Handle mouse input
 * @author Rory Dungan
 */
void THIS::mouseReleaseEvent(QMouseEvent * event)
{
    switch(mInteractionMode)
    {
    case IS_EDITING_HEIGHT:

        break;
    }

    mCurrentState = IS_IDLE;
    mEditMarker->setVisible(false);
}

/**
 * @brief Handle mouse input
 * @author Rory Dungan
 */
void THIS::mouseMoveEvent(QMouseEvent * event)
{
    // Get amount of movement instead of absolute position
    int dx = event->x() - mLastCursorPos.x();
    int dy = event->y() - mLastCursorPos.y();

    switch(mCurrentState)
    {
    case IS_ROTATING_CAMERA:
        mCamera->pitch(Ogre::Degree(bCameraControlsInverted ? -dy : dy));
        mCamera->yaw(Ogre::Degree(bCameraControlsInverted ? -dx : dx));
        updateGL();
        break;
    case IS_MOVING_CAMERA:
        mCamera->moveRelative(Ogre::Vector3(-dx, dy, 0)); // dx movement is inverted
        updateGL();
        break;
    default:
        break;
    }

    mLastCursorPos = event->pos();
}

/**
 * @brief Set the mode for interacting with the mouse
 * @author Rory Dungan
 */
void THIS::setInteractionMode(interactionModes i)
{
    mInteractionMode = i;
}

/**
 * @brief set up the scene with a terrain
 * @author Rory Dungan
 */
void THIS::setupScene()
{
    // Set up camera
    mCamera->setPosition(Ogre::Vector3(-4000, 1500, 4000));
    mCamera->lookAt(Ogre::Vector3(2000, 50, -3000));

    // Set up skybox
    //mSceneMgr->setSkyBox(true, "irrSky");
    setSkyDome("clouds.jpg");

    // Set up light
    Ogre::Vector3 lightdir(0.55, -0.3, 0.75);
    lightdir.normalise();

    mSun = mSceneMgr->createLight("tstLight");
    mSun->setType(Ogre::Light::LT_DIRECTIONAL);
    mSun->setDirection(lightdir);
    mSun->setDiffuseColour(Ogre::ColourValue::White);
    mSun->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));

    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));

    // Set up terrain
    // @Wraith - I don't actually remember what talos was set to before, and can't find out until I commit.
    // REGARDING RORY - You should be able to use this function to integrate the sliders with modifying the terrain.
    // More functionality will be added, but probably to a different function, from what I can think of at the moment.
    // In order, the variables are terrainSize, talos, and staggerValue (the unevenness of the terrain)
    mTerrain = new Terrain(mSceneMgr, mSun);

    /*/ Set up indicator for terrain
    mDecalFrustum = new Ogre::Frustum();
    mProjectorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("DecalProjectorNode");
    mProjectorNode->attachObject(mDecalFrustum);
    mProjectorNode->setPosition(0, 100, 0);*/
    mEditMarker = mSceneMgr->createEntity("EditMarker", "sphere.mesh");
    mEditNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    mEditNode->attachObject(mEditMarker);
    mEditNode->setScale(0.05, 0.05, 0.05);

    /*/ Set up material for indicator
    Ogre::Pass* pass = mTerrain->getTerrainGroup()->getTerrain(0,0)->getMaterial()->getTechnique(0)->createPass();
    pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
    pass->setDepthBias(1);
    pass->setLightingEnabled(false);

    Ogre::TextureUnitState* texState = pass->createTextureUnitState("terrainmarker.png");
    texState->setProjectiveTexturing(true, mDecalFrustum);
    texState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
    texState->setTextureFiltering(Ogre::FO_POINT, Ogre::FO_LINEAR, Ogre::FO_NONE);*/
}

/**
 * @brief render a frame
 * @author Kito Berg-Taylor
 */
void THIS::paintGL()
{
    // If the current state is a state in which the user is interacting with the terrain via the mouse
    if(mCurrentState == IS_EDITING_HEIGHT || mCurrentState == IS_PAINTING || mCurrentState == IS_PLACING_OBJECTS)
    {
        // Get the 3d point that the cursor is over
        float relPosX = (float)mLastCursorPos.x() / (float)mCamera->getViewport()->getActualWidth();
        float relPosY = (float)mLastCursorPos.y() / (float)mCamera->getViewport()->getActualHeight();
        // fire ray
        Ogre::Ray ray = mCamera->getCameraToViewportRay(relPosX, relPosY);

        Ogre::TerrainGroup::RayResult rayResult = mTerrain->getTerrainGroup()->rayIntersects(ray);
        if(rayResult.hit) // Ray hit the terrain
        {
            //qDebug() << "rayResult hit point " << rayResult.position.x << " " << rayResult.position.y << " " << rayResult.position.z;
            mEditMarker->setVisible(true);
            mEditNode->setPosition(rayResult.position);

            // Figue out which parts of the terrain this affects
            Ogre::TerrainGroup::TerrainList terrainList;
            Ogre::Real brushSizeWorldSpace = mTerrain->getTerrainGroup()->getTerrainWorldSize() * mBrushSize;
            Ogre::Sphere sphere(rayResult.position, brushSizeWorldSpace);
            mTerrain->getTerrainGroup()->sphereIntersects(sphere, &terrainList);

            switch(mCurrentState)
            {
            case IS_EDITING_HEIGHT:
                for(Ogre::TerrainGroup::TerrainList::iterator ti = terrainList.begin(); ti != terrainList.end(); ++ti)
                    modifyTerrain(*ti, rayResult.position, mBrushSize, mBrushWeight * 0.017f, mInteractionMode == IM_EXTRUDE);
                break;
            case IS_PAINTING:
                for(Ogre::TerrainGroup::TerrainList::iterator ti = terrainList.begin(); ti != terrainList.end(); ++ti)
                    modifyBlendMaps(*ti, rayResult.position, 0.017f);
                break;
            case IS_PLACING_OBJECTS:

            default:
                break;
            }
        }
        else
            mEditMarker->setVisible(false);
    }

    // Update the terrain
    if (mHeightUpdateCountDown > 0)
    {
        mHeightUpdateCountDown -= 0.017f;//evt.timeSinceLastFrame; - always updates at ~60fps so this can be a constant
        if (mHeightUpdateCountDown <= 0)
        {
            mTerrain->getTerrainGroup()->update();
            mHeightUpdateCountDown = 0;
        }
    }

    // Notify of texture update
    if (mTerrain->getTerrainGroup()->isDerivedDataUpdateInProgress())
    {
        if(bUpdatingTextures == false)
            emit textureUpdateInProgress();
        bUpdatingTextures = true;
    }
    else
    {
        if(bUpdatingTextures == true)
            emit textureUpdateFinished();
        bUpdatingTextures = false;
    }

    // Update camera
    mCamera->moveRelative(mCameraMovement);

    assert( mOgreWindow );
    mOgreRoot->renderOneFrame();
}

/**
 * @brief resize the GL window
 * @author Kito Berg-Taylor
 */
void THIS::resizeGL( int width, int height )
{
    assert( mOgreWindow );
    mOgreWindow->windowMovedOrResized();
}

/**
 * @brief choose the right renderer
 * @author Kito Berg-Taylor
 */
Ogre::RenderSystem* THIS::chooseRenderer( Ogre::RenderSystemList *renderers )
{
    // It would probably be wise to do something more friendly
    // that just use the first available renderer
    return *renderers->begin();
}

/**
 * @brief Modify the terrain with one of the terrain modification tools
 * @author Rory Dungan
 */
void THIS::modifyTerrain(Ogre::Terrain* terrain, const Ogre::Vector3 &centerPos, float brushSize, float brushWeight, bool extrude)
{
    Ogre::Vector3 tsPos;
    terrain->getTerrainPosition(centerPos, &tsPos);

    // We need point coords
    Ogre::Real terrainSize = (terrain->getSize() -1);
    long startx = (tsPos.x - brushSize) * terrainSize;
    long starty = (tsPos.y - brushSize) * terrainSize;
    long endx = (tsPos.x + brushSize) * terrainSize;
    long endy = (tsPos.y + brushSize) * terrainSize;
    startx = std::max(startx, 0L);
    starty = std::max(starty, 0L);
    endx = std::min(endx, (long)terrainSize);
    endy = std::min(endy, (long)terrainSize);
    for(long y = starty; y <= endy; ++y)
    {
        for(long x = startx; x <= endx; ++x)
        {
            Ogre::Real tsXdist = (x / terrainSize) - tsPos.x;
            Ogre::Real tsYdist = (y / terrainSize) - tsPos.y;

            Ogre::Real weight = std::min((Ogre::Real)1.0f, Ogre::Math::Sqrt(tsYdist * tsYdist + tsXdist * tsXdist) / Ogre::Real(0.5 * mBrushSize));
            weight = 1.0f - (weight * weight);

            float addedHeight = weight * brushWeight;
            float newHeight = extrude
                    ? terrain->getHeightAtPoint(x, y) + addedHeight
                    : terrain->getHeightAtPoint(x, y) - addedHeight;
            terrain->setHeightAtPoint(x, y, newHeight);
        }
    }
    if(mHeightUpdateCountDown == 0)
        mHeightUpdateCountDown = mHeightUpdateRate;
}

/**
 * @brief Modify terrain textures/blendmap with paint tool
 * @author Rory Dungan
 */
void THIS::modifyBlendMaps(Ogre::Terrain *terrain, const Ogre::Vector3 &centerPos, Ogre::Real timeElapsed)
{
    Ogre::Vector3 tsPos;
    terrain->getTerrainPosition(centerPos, &tsPos);

    Ogre::TerrainLayerBlendMap* layer = terrain->getLayerBlendMap(mLayerEdit);
    // we need image coords
    Ogre::Real imgSize = terrain->getLayerBlendMapSize();
    long startx = (tsPos.x - mBrushSize) * imgSize;
    long starty = (tsPos.y - mBrushSize) * imgSize;
    long endx = (tsPos.x + mBrushSize) * imgSize;
    long endy= (tsPos.y + mBrushSize) * imgSize;
    startx = std::max(startx, 0L);
    starty = std::max(starty, 0L);
    endx = std::min(endx, (long)imgSize);
    endy = std::min(endy, (long)imgSize);
    for (long y = starty; y <= endy; ++y)
    {
        for (long x = startx; x <= endx; ++x)
        {
            Ogre::Real tsXdist = (x / imgSize) - tsPos.x;
            Ogre::Real tsYdist = (y / imgSize)  - tsPos.y;

            Ogre::Real weight = std::min((Ogre::Real)1.0f,
                Ogre::Math::Sqrt(tsYdist * tsYdist + tsXdist * tsXdist) / Ogre::Real(0.5 * mBrushSize));
            weight = 1.0 - (weight * weight);

            float paint = weight * timeElapsed;
            size_t imgY = imgSize - y;
            float val;
            val = layer->getBlendValue(x, imgY) + paint;
            val = Ogre::Math::Clamp(val, 0.0f, 1.0f);
            layer->setBlendValue(x, imgY, val);

        }
    }

    layer->update();
}

/**
 * @brief Save a screenshot of the current viewport to a file
 * @author Rory Dungan
 */
void THIS::saveScreenshotToFile(QString filename)
{
    // Cancel if there is no filename to write to
    if(filename == 0)
        return;

    // Render to texture.
    // Note that we can't just use mOgreWindow->writeContentsToFile() since that just takes whatever is onscreen
    // at the time on the area Ogre is rendering to, so if part of the render window is obscured by another window
    // it will also show up on the file. Rendering to a texture first prevents this.
    Ogre::TexturePtr rtt_texture = Ogre::TextureManager::getSingleton().createManual("RttTex", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, mOgreWindow->getWidth(), mOgreWindow->getHeight(), 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);
    Ogre::RenderTexture *renderTexture = rtt_texture->getBuffer()->getRenderTarget();

    // Set to camera viewport
    renderTexture->addViewport(mCamera);
    renderTexture->getViewport(0)->setOverlaysEnabled(false);

    // Update the texture once
    renderTexture->update();

    // Now save the contents
    renderTexture->writeContentsToFile(filename.toStdString());
}

/**
 * @brief Camera movement with the keyboard
 * @author Rory Dungan
 */
void THIS::setCameraMovementDirection(bool forward, bool back, bool left, bool right, bool up, bool down, bool sprint)
{
    float movementSpeed = sprint ? -30.0f : -15.0f;

    mCameraMovement = Ogre::Vector3((left ? movementSpeed : 0.0f) - (right ? movementSpeed : 0.0f),
                                    (down ? movementSpeed : 0.0f) - (up ? movementSpeed : 0.0f),
                                    (forward ? movementSpeed : 0.0f) - (back ? movementSpeed : 0.0f));
}

/**
 * @brief Create or change the skybox
 * @author Rory Dungan
 */
void THIS::setSkyDome(QString filepath, float curvature, float tiling)
{
    // If necessary, add resource group in directory
    QDir directory = QFileInfo(filepath).dir();
    if(!Ogre::ResourceGroupManager::getSingleton().resourceLocationExists(directory.path().toStdString()))
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(directory.path().toStdString(), "FileSystem");

    // In order for this to be user-configurable we must manually create the material
    std::string name = QFileInfo(filepath).fileName().toStdString();
    Ogre::MaterialPtr skydome = Ogre::MaterialManager::getSingleton().create("sky_" + name, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    Ogre::Pass* pass = skydome->getTechnique(0)->getPass(0);
    pass->setLightingEnabled(false);
    pass->setDepthWriteEnabled(false);
    pass->createTextureUnitState(name);
    mSceneMgr->setSkyDome(true, "sky_" + name, curvature, tiling); // Finished setting up material and loading data, now create the skydome
}

/**
 * @brief Change the fog parameters
 * @author Rory Dungan
 */
void THIS::setFog(int fogType, QColor colour, float density, float start, float end)
{
    switch(fogType)
    {
    case 0: // No fog
        mSceneMgr->setFog(Ogre::FOG_NONE);
        break;
    case 1: // Linear fog
        mSceneMgr->setFog(Ogre::FOG_LINEAR, Ogre::ColourValue(colour.redF(), colour.greenF(), colour.blueF()), density, start, end);
        break;
    case 2: // Exponential fog
        mSceneMgr->setFog(Ogre::FOG_EXP, Ogre::ColourValue(colour.redF(), colour.greenF(), colour.blueF()), density, start, end);
        break;
    case 3: // Exponential^2
        mSceneMgr->setFog(Ogre::FOG_EXP2, Ogre::ColourValue(colour.redF(), colour.greenF(), colour.blueF()), density, start, end);
        break;
    default: break;
    }
}

/**
 * @brief
 * @author Rory Dungan
 */
void THIS::setSunPosition(double altitude, double angle)
{
    mSunAngle = angle;
    mSunAltitude = altitude;
    // Convert degrees to radians
    double angleRads = mSunAngle * (M_PI/180);
    double altitudeRads = mSunAltitude * (M_PI/180);
    //
    double realDist = cos(altitudeRads);
    mSun->setDirection(Ogre::Vector3::ZERO - Ogre::Vector3(sin(angleRads) * realDist, sin(altitudeRads), cos(angleRads) * realDist));
    // Update terrain
    Ogre::TerrainGroup::TerrainIterator ti = mTerrain->getTerrainGroup()->getTerrainIterator();
    while(ti.hasMoreElements())
    {
        Ogre::Terrain* t = ti.getNext()->instance;
        t->dirty();
        t->update();
    }
}

void THIS::loadModel(QString filepath)
{
    // If necessary, add resource group in directory
    QDir directory = QFileInfo(filepath).dir();
    if(!Ogre::ResourceGroupManager::getSingleton().resourceLocationExists(directory.path().toStdString()))
        Ogre::ResourceGroupManager::getSingleton().addResourceLocation(directory.path().toStdString(), "FileSystem");

    Ogre::SceneNode* mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(filepath.toStdString() + "_main");

    Ogre::Entity* mEnt = mSceneMgr->createEntity(filepath.toStdString() + "_entity", QFileInfo(filepath).fileName().toStdString());
    mEnt->setMaterialName("Shrub-Material");
    mNode->attachObject(mEnt);

    mNode->setPosition(mEditNode->getPosition());
}
