#ifndef WORLDOPTIONS_H
#define WORLDOPTIONS_H

#include <QString>

// Struct containing all the options for generating a terrain
// This is what we get from loading a saved file or from running the New File wizard

struct WorldOptions
{
    int terrainSeed;
    int terrainRandomFactor;
    int terrainScale;
    int erosionPasses;
    int terrainSize;
    double worldSize;
    struct textureLayer
    {
        int textureScale;
        double minHeight;
        double fadeDist;
        QString diffuseSpecular;
        QString normalHeight;
    } layers[3];
    QString skyDome;
    double skyCurvature;
    int skyTiling;
    int foliageDensisty;

    QString writeToXml();
    bool loadAttributesFromXml(QString xml);
};

#endif // WORLDOPTIONS_H
