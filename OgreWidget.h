#ifndef OGREWIDGET_H
#define OGREWIDGET_H

#include <OGRE/Ogre.h>
#include <QGLWidget>
#include <QX11Info>
#include "Terrain.h"

class OgreWidget : public QGLWidget
{
//Q_OBJECT;
public:
    OgreWidget( QWidget *parent=0 ):
    QGLWidget( parent ),
    mOgreWindow(NULL)
    {
        init( "plugins.cfg", "resources.cfg", "ogre.cfg", "ogre.log" );
    }

    virtual ~OgreWidget()
    {
        mOgreRoot->shutdown();
        delete mOgreRoot;
        delete mTerrain;
        destroy();
    }

    void setupScene();

    Terrain* getTerrain() { return mTerrain; }

protected:
    virtual void initializeGL();
    virtual void resizeGL( int, int );
    virtual void paintGL();

    void init( std::string, std::string, std::string, std::string );

    virtual Ogre::RenderSystem* chooseRenderer( Ogre::RenderSystemList* );

    Ogre::Root *mOgreRoot;
    Ogre::RenderWindow *mOgreWindow;
    Ogre::Camera *mCamera;
    Ogre::Viewport *mViewport;
    Ogre::SceneManager *mSceneMgr;
    Terrain* mTerrain;
};

#endif // OGREWIDGET_H
