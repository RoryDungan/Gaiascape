#ifndef HEIGHTMAPGEN_H
#define HEIGHTMAPGEN_H

#include <vector>
#include "algorithms/random.h"
#include "HMBlock.h"

class HeightMapGen
{
    public:
        HeightMapGen(unsigned int size);
        std::vector<HMBlock*> retrieveBlocks();
    private:
        void genQuadrant(int xNW, int yNW, int xSE, int ySE, int iteration, int quadrant);
        HMBlock* getByID(short unsigned int ID);
        HMBlock* getByLoc(short unsigned int x, short unsigned int y);

        std::vector<HMBlock*> HMBlocks;

        short unsigned int iNumberOfBlocks;
        short unsigned int iDimensions;
        short unsigned int iQuadrants;

        float fStaggerValue;

};

#endif // HEIGHTMAPGEN_H
