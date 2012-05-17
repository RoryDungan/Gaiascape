#ifndef GLOBALTERRAINMASTER_H
#define GLOBALTERRAINMASTER_H

#include "terrainBlock.h"
#include <vector>
#include "include/algorithms/random.h"
class globalTerrainMaster
{
    public:
        globalTerrainMaster(unsigned int size);
        virtual ~globalTerrainMaster();

        terrainBlock* getByID(int ID);
        terrainBlock* getByLoc(unsigned int x, unsigned int y);
    protected:
        unsigned int iNumberOfBlocks;
        terrainBlock globalBlocks[];

        // Processes for generating terrain
        // Anything with the suffix "Value" is used for fine tuning the properties of the terrain (i.e. if the user selects oceanic, or very hot, or whatever.
        bool run(); // Main part of terrain generation
        bool genHeight(int heightValue); // Generate the height of each tile using midpoint displacement algorithm

        // Function which handles almost everything to do with quadrants.
        void genQuadrant(int xNW, int yNW, int xSE, int ySE, int iteration, int quadrant);

        bool genTemp(int temperValue);
        bool genRain(int rainValue);
        bool genDrain();
        bool genSalin();
        bool genBiomes();
        bool genMountains();
        bool runRivers();
        bool genRivers();
        bool genPlantlife();
    private:
        std::vector<terrainBlock*> terrainBlocks;

        bool displayMap();

        unsigned int iDimensions;
        unsigned int iQuadrants;

        float fStaggerValue;
};

#endif // GLOBALTERRAINMASTER_H
