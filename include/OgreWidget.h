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
#include <QColor>
#include "Terrain.h"

class OgreWidget : public QGLWidget
{
    Q_OBJECT

public:
    OgreWidget( QWidget *parent=0, QString logFile = "", QString renderSystem = "OpenGL Rendering Subsystem" )
        : QGLWidget( parent )
        , mOgreWindow(NULL)
    {
        // Fire up Ogre
        init(logFile.toStdString(), renderSystem.toStdString());
    }

    virtual ~OgreWidget()
    {
        if(mTerrain) delete mTerrain;
        mOgreRoot->shutdown();
        delete mOgreRoot;
        destroy();
    }

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
    } mInteractionMode;

    void setupScene();
    void setInteractionMode(interactionModes i);
    void setViewMode(Ogre::PolygonMode mode) { mCamera->setPolygonMode(mode); }
    void saveScreenshotToFile(QString filename);
    void setCameraInverted(bool inverted) { bCameraControlsInverted = inverted; }
    void setFOVy(float fov);
    // Brush size is the area which will be affected by painting on the terrain
    // Ogre::Real is actually just a typedef of float so it's ok to assign it as one. We use standard floats here so that the class can be used without directly referencing any of Ogre3d
    void setBrushSize(float size) { mBrushSize = size; }
    float getBrushSize() { return mBrushSize; }
    // Brush weight is how much of an effect clicking on the terrain will have
    void setBrushWeight(float weight) { mBrushWeight = weight; }
    float getBrushWeight() { return mBrushWeight; }
    // Terrain texture layer which will be affected by painting on the terrain
    void setLayerEdit(unsigned char layer) { mLayerEdit = layer; }
    unsigned char getLayerEdit() { return mLayerEdit; }
    // Give outside classes direct access to the terrain
    Terrain* getTerrain() { return mTerrain; }
    // Each string corrosponds to the path the the image
    //void setSkyBox(QString up, QString dn, QString lf, QString rt, QString fr, QString bk);
    void setSkyDome(QString filepath, float curvature = 10.0f, float tiling = 8.0f);
    void setSunPosition(double altitude, double angle);
    double getSunAlitude() { return mSunAltitude; }
    double getSunAngle() { return mSunAngle; }
    void setFog(int fogType, QColor colour, float density = 0.001, float start = 50, float end = 500);
    // Camera movement with the keyboard
    void setCameraMovementDirection(bool forward, bool back, bool left, bool right, bool up, bool down, bool sprint);

    Ogre::Vector3 mCameraMovement;
    bool bCtrlPressed;

signals:
    void textureUpdateInProgress();
    void textureUpdateFinished();

    void heightUpdated();

protected:
    virtual void initializeGL();
    virtual void resizeGL( int, int );
    virtual void paintGL();
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);

    void init( std::string, std::string);

    void modifyTerrain(Ogre::Terrain* terrain, const Ogre::Vector3& centerPos, Ogre::Real timeElapsed);
    void modifyBlendMaps(Ogre::Terrain* terrain, const Ogre::Vector3& centerPos, Ogre::Real timeElapsed);

    virtual Ogre::RenderSystem* chooseRenderer( Ogre::RenderSystemList* );

    Ogre::Root* mOgreRoot;
    Ogre::RenderWindow* mOgreWindow;
    Ogre::Camera* mCamera;
    Ogre::Viewport* mViewport;
    Ogre::SceneManager* mSceneMgr;

private:
    Terrain* mTerrain;
    Ogre::Real mBrushSize;
    Ogre::Real mBrushWeight;
    Ogre::Entity* mEditMarker;
    Ogre::SceneNode* mEditNode;
    Ogre::Real mHeightUpdateCountDown;
    Ogre::Real mHeightUpdateRate;
    Ogre::Vector3 mTerrainPos;
    Ogre::uint8 mLayerEdit;
    Ogre::Light* mSun;
    double mSunAngle;
    double mSunAltitude;

    // Corrosponds to what the user is doing right now
    enum interactionStates
    {
        IS_IDLE, // Not doing anything
        IS_BUSY, // This state is activated while something else is happening (e.g. the terrain is being genarated)
                 // in order to prevent the user from doing something else while this is going on
        IS_MOVING_CAMERA,
        IS_ROTATING_CAMERA,
        IS_EDITING_HEIGHT,
        IS_PAINTING,
        IS_PLACING_OBJECTS
    } mCurrentState;

    // Stores the last position of the cursor
    QPoint mLastCursorPos;
    bool bCameraControlsInverted;
    Ogre::Degree mCamFOV;

    bool bUpdatingTextures;
};

#endif // OGREWIDGET_H
