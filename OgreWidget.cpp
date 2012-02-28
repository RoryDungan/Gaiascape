#include "OgreWidget.h"

#define THIS OgreWidget

/**
 * @brief init the object
 * @author kito berg-taylor
 */
void THIS::init( std::string plugins_file,
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
        if (rName == "OpenGL Rendering Subsystem")
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
    mCamera->setNearClipDistance( 1.0 );

    Ogre::Viewport *mViewport = mOgreWindow->addViewport( mCamera );
    mViewport->setBackgroundColour( Ogre::ColourValue( 0.8,0.8,1 ) );
}

/**
 * @brief render a frame
 * @author Kito Berg-Taylor
 */
void THIS::paintGL()
{
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
