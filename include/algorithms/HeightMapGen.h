#ifndef HEIGHTMAPGEN_H
#define HEIGHTMAPGEN_H

#include <vector>
#include "Random.h"
#include "HMBlock.h"

class HeightMapGen
{
    public:
        HeightMapGen(unsigned int size, signed short x, signed short y, float talos, float staggerValue);

        short unsigned int retrieveDimensions();

        void outputSlopemap(float *slopemapArray);
        float* getHeightmap() { return pHMBlocks; }
        signed short getX() { return iX; }
        signed short getY() { return iY; }

        short unsigned int iNumberOfBlocks;
        long unsigned int iFinalX;
        long unsigned int iFinalPoint;
        short unsigned int iDimensions;
        short unsigned int iQuadrants;

    private:
        // An overload which allows a row to be already defined. Useful when we are generating a terrain next to one which already exists.
        void generateHeightmap(float talos, float staggerValue, float *northArray, float *eastArray, float *southArray, float *westArray);
        void generateHeightmap(float talos, float staggerValue);  // Size for terrain should always be 8

        void genQuadrant(int xNW, int yNW, int xSE, int ySE, int iteration, int quadrant);
        void erodeBlock(float c, long unsigned int block);
        void transportMaterial(float material, long unsigned int block);
        float getHeightDifference(unsigned long inspectedBlock, unsigned long neighbourBlock);
        void writeMap();

        float* pHMBlocks;

        float fStaggerValue;
        float fTalos;                           // Maximum angle a slope may have

        signed short iX;
        signed short iY;

};

#endif // HEIGHTMAPGEN_H
