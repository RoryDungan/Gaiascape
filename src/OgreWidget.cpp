#include "OgreWidget.h"
#include <QDebug>

#define THIS OgreWidget

/**
 * @brief init the object
 * @author kito berg-taylor
 */
void THIS::init( std::string plugins_file,
         std::string resources_file,
         std::string ogre_cfg_file,
         std::string ogre_log )
{
    // create the main ogre object
    mOgreRoot = new Ogre::Root( plugins_file, ogre_cfg_file, ogre_log );

    // setup a renderer
    Ogre::RenderSystemList::const_iterator renderers = mOgreRoot->getAvailableRenderers().begin();
    while(renderers != mOgreRoot->getAvailableRenderers().end())
    {
        Ogre::String rName = (*renderers)->getName();
#ifdef _WIN32
        if (rName == "Direct3D9 Rendering Subsystem")
#else
        if (rName == "OpenGL Rendering Subsystem")
#endif
            break;
        renderers++;
    }

    Ogre::RenderSystem *renderSystem = *renderers;
    mOgreRoot->setRenderSystem( renderSystem );
    QString dimensions = QString( "%1x%2" )
                    .arg(this->width())
                    .arg(this->height());

    renderSystem->setConfigOption( "Video Mode", dimensions.toStdString() );

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
    mBrushSize = 0.1;

    // Set up resources
    Ogre::ConfigFile cf;
    cf.load(resources_file);    

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
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

    // This should really be called after initializeGL() because it takes a fair amount of time to load the resources
    // and initializeGL() is called before the main window of the program is created. In future I would like to have
    // it create the window immediatly, set the cursor to busy/wait and then call setupScene(), preferably in a
    // background thread - Rory
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
                mCurrentState = IS_EXTRUDING;
                break;
            case IM_INTRUDE:
                mCurrentState = IS_EXTRUDING;
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
    mCurrentState = IS_IDLE;
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
    mCamera->setPosition(Ogre::Vector3(1683, 50, 2116));
    mCamera->lookAt(Ogre::Vector3(1963, 50, 1660));

    // Set up skybox
    mSceneMgr->setSkyBox(true, "irrSky"); //For some reason this crashes, I have no idea why. - Rory

    // Set up light
    Ogre::Vector3 lightdir(0.55, -0.3, 0.75);
    lightdir.normalise();

    Ogre::Light* light = mSceneMgr->createLight("tstLight");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    light->setDirection(lightdir);
    light->setDiffuseColour(Ogre::ColourValue::White);
    light->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));

    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));

    // Set up terrain
    mTerrain = new Terrain(mSceneMgr, light);

    mEditMarker = mSceneMgr->createEntity("EditMarker", "sphere.mesh");
    mEditNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    mEditNode->attachObject(mEditMarker);
    mEditNode->setScale(0.05, 0.05, 0.05);
}

/**
 * @brief render a frame
 * @author Kito Berg-Taylor
 */
void THIS::paintGL()
{
    // If the current state is a state in which the user is interacting with the terrain via the mouse
    if(mCurrentState == IS_EXTRUDING || mCurrentState == IS_INTRUDING || mCurrentState == IS_PAINTING || mCurrentState == IS_PLACING_OBJECTS)
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
            Ogre::Real brushSizeWorldSpace = /* TERRAIN_WORLD_SIZE * mBrushSize */ 240;
            Ogre::Sphere sphere(rayResult.position, brushSizeWorldSpace);
            mTerrain->getTerrainGroup()->sphereIntersects(sphere, &terrainList);

            switch(mCurrentState)
            {
            case IS_EXTRUDING:
                for(Ogre::TerrainGroup::TerrainList::iterator ti = terrainList.begin(); ti != terrainList.end(); ++ti)
                    modifyTerrain(*ti, rayResult.position, 0.017f);
                break;
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
            qDebug() << "Updating terrain";
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
void THIS::modifyTerrain(Ogre::Terrain* terrain, const Ogre::Vector3 &centerPos, Ogre::Real timeElapsed)
{
    Ogre::Vector3 tsPos;
    terrain->getTerrainPosition(centerPos, &tsPos);

    // We need point coords
    Ogre::Real terrainSize = (terrain->getSize() -1);
    long startx = (tsPos.x - mBrushSize) * terrainSize;
    long starty = (tsPos.y - mBrushSize) * terrainSize;
    long endx = (tsPos.x + mBrushSize) * terrainSize;
    long endy = (tsPos.y + mBrushSize) * terrainSize;
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

            float addedHeight = weight * 250.0f * timeElapsed;
            float newHeight = mInteractionMode == IM_EXTRUDE ? terrain->getHeightAtPoint(x, y) + addedHeight : terrain->getHeightAtPoint(x, y) - addedHeight;
            terrain->setHeightAtPoint(x, y, newHeight);
        }
    }
    if(mHeightUpdateCountDown == 0)
        mHeightUpdateCountDown = mHeightUpdateRate;
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
                                    (up ? movementSpeed : 0.0f) - (down ? movementSpeed : 0.0f),
                                    (forward ? movementSpeed : 0.0f) - (back ? movementSpeed : 0.0f));
}
