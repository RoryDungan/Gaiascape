#ifndef OGREWIDGET_H
#define OGREWIDGET_H

#include <OGRE/Ogre.h>
#include <QGLWidget>
#ifndef WIN32 && MACOS
#include <QX11Info>
#endif
#include <QMouseEvent>
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
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);

    void init( std::string, std::string, std::string, std::string );

    virtual Ogre::RenderSystem* chooseRenderer( Ogre::RenderSystemList* );

    Ogre::Root* mOgreRoot;
    Ogre::RenderWindow* mOgreWindow;
    Ogre::Camera* mCamera;
    Ogre::Viewport* mViewport;
    Ogre::SceneManager* mSceneMgr;

private:
    Terrain* mTerrain;

    // Is used to tell what action clicking on the widget has
    // Corrosponds to what tool the user has selected
    enum interactionModes
    {
        IM_SELECT,
        IM_MOVE_CAMERA,
        IM_EXTRUDE,
        IM_INTRUDE,
        IM_PAINT,
        IM_PLACEOBJ
    } interactionMode;

    // Corrosponds to what the user is doing right now
    enum interactionStates
    {
        IS_IDLE, // Not doing anything
        IS_BUSY, // This state is activated while something else is happening (e.g. the terrain is being genarated)
                 // in order to prevent the user from doing something else while this is going on
        IS_MOVING_CAMERA,
        IS_ROTATING_CAMERA,
        IS_EXTRUDING,
        IS_INTRUDING,
        IS_PAINTING,
        IS_PLACING_OBJECTS
    } currentState;

    // Stores the last position of the cursor
    QPoint mLastCursorPos;

public:
    void setInteractionMode(interactionModes i);
};

#endif // OGREWIDGET_H
