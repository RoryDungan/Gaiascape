#ifndef OGREWIDGET_H
#define OGREWIDGET_H

#include <OGRE/Ogre.h>
#include <QGLWidget>
#ifndef WIN32
    #ifndef MACOS
        #include <QX11Info>
    #endif // MACOS
#endif // WIN32
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
        // init variables
        bCtrlPressed = false;

        // Fire up Ogre
        init( "plugins.cfg", "resources.cfg", "ogre.cfg", "ogre.log" );
    }

    virtual ~OgreWidget()
    {
        if(mTerrain) delete mTerrain;
        mOgreRoot->shutdown();
        delete mOgreRoot;
        destroy();
    }

    void setupScene();

    // Camera movement with the keyboard
    void setCameraMovementDirection(bool forward, bool back, bool left, bool right, bool up, bool down, bool sprint);
    Ogre::Vector3 mCameraMovement;

    // Ogre::Real is actually just a typedef of float so it's ok to assign it as one
    void setBrushSize(float size) { mBrushSize = size; }
    Terrain* getTerrain() { return mTerrain; }

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

    void setInteractionMode(interactionModes i);
    void setViewMode(Ogre::PolygonMode mode) { mCamera->setPolygonMode(mode); }
    void saveScreenshotToFile(QString filename);

    bool bCtrlPressed;
protected:
    virtual void initializeGL();
    virtual void resizeGL( int, int );
    virtual void paintGL();
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);

    void init( std::string, std::string, std::string, std::string );

    void modifyTerrain(Ogre::Terrain* terrain, const Ogre::Vector3& centerPos, Ogre::Real timeElapsed);

    virtual Ogre::RenderSystem* chooseRenderer( Ogre::RenderSystemList* );

    Ogre::Root* mOgreRoot;
    Ogre::RenderWindow* mOgreWindow;
    Ogre::Camera* mCamera;
    Ogre::Viewport* mViewport;
    Ogre::SceneManager* mSceneMgr;

private:
    Terrain* mTerrain;
    Ogre::Real mBrushSize;

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
};

#endif // OGREWIDGET_H
