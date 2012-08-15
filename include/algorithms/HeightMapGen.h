#ifndef HEIGHTMAPGEN_H
#define HEIGHTMAPGEN_H

#include <vector>
#include "Random.h"
#include "HMBlock.h"

class HeightMapGen
{
    public:
        // Scale is really only required for terrain.cpp, but is put here because when we are generating vegetation we need the stored scale.
        // But really this is more appropriate for terrain.cpp
        HeightMapGen(unsigned int size, signed short x, signed short y, unsigned short erosionIterations, float staggerValue, float scale);

        short unsigned int retrieveDimensions();

        float* getHeightmap() { return pHMBlocks; }
        void outputSlopemap(float *slopemapArray);
        signed short getX() { return iX; }
        signed short getY() { return iY; }

        long unsigned int iFinalX;
        long unsigned int iFinalPoint;
        short unsigned int iDimensions;
        short unsigned int iQuadrants;
        float fScale;

    private:
        // An overload which allows a row to be already defined. Useful when we are generating a terrain next to one which already exists.
        // void generateHeightmap(float talos, float staggerValue, float *northArray, float *eastArray, float *southArray, float *westArray);
        void generateHeightmap(unsigned short erosionIterations, float staggerValue);  // Size for terrain should always be 8

        void genQuadrant(int xNW, int yNW, int xSE, int ySE, int iteration, int quadrant);
        void erodeBlock(float c, long unsigned int block);
        void transportMaterial(unsigned short material, long unsigned int block);
        float getHeightDifference(unsigned long inspectedBlock, unsigned long neighbourBlock);
        void writeMap();

        float* pHMBlocks;
        float heightMap[66049];

        float fStaggerValue;
        float fTalos; // Yet another point where size is taken for granted to be 8                           // Maximum angle a slope may have.

        signed short iX;
        signed short iY;
        unsigned short iErosionIterations;

};

#endif // HEIGHTMAPGEN_H
