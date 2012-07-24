#ifndef HEIGHTMAPGEN_H
#define HEIGHTMAPGEN_H

#include <vector>
#include "random.h"
#include "HMBlock.h"

class HeightMapGen
{
    public:
        HeightMapGen(float talos, unsigned int size);

        short unsigned int retrieveDimensions();
        void retrieveHeightmap(float talos, unsigned int size, float *heightmapArray, float staggerValue);  // Size for terrain should always be 8
        void retrieveSlopemap(float *slopemapArray, float *heightmapArray, short unsigned int dimensions);

    private:
        void genQuadrant(int xNW, int yNW, int xSE, int ySE, int iteration, int quadrant);
        void erodeBlock(float c, long unsigned int block);
        void transportMaterial(float material, long unsigned int block);
        float getHeightDifference(unsigned long inspectedBlock, unsigned long neighbourBlock);
        void writeMap();

        float* pHMBlocks;

        short unsigned int iNumberOfBlocks;
        long unsigned int iFinalX;
        long unsigned int iFinalPoint;
        short unsigned int iDimensions;
        short unsigned int iQuadrants;

        float fStaggerValue;
        float fTalos; // Maximum angle a slope may have

};

#endif // HEIGHTMAPGEN_H
