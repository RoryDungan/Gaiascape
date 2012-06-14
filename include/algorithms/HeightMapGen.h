#ifndef HEIGHTMAPGEN_H
#define HEIGHTMAPGEN_H

#include <vector>
#include "algorithms/random.h"
#include "HMBlock.h"

class HeightMapGen
{
    public:
        HeightMapGen(float talos, unsigned int size);
        std::vector<HMBlock*> retrieveBlocks();

        HMBlock* getByID(short unsigned int ID);
        HMBlock* getByLoc(short unsigned int x, short unsigned int y);

        short unsigned int retrieveDimensions();
    private:
        void genQuadrant(int xNW, int yNW, int xSE, int ySE, int iteration, int quadrant);
        void erode(short int c);

        std::vector<HMBlock*> HMBlocks;

        short unsigned int iNumberOfBlocks;
        short unsigned int iDimensions;
        short unsigned int iQuadrants;

        float fStaggerValue;
        float fTalos; // Maximum angle a slope may have
};

#endif // HEIGHTMAPGEN_H
