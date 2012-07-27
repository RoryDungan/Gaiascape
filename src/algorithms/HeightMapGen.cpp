#include <iostream>
#include "algorithms/HeightMapGen.h"
#include <cmath>
#include <fstream>
#include <QImage>
#include <QByteArray>

HeightMapGen::HeightMapGen(unsigned int size, signed short x, signed short y, float talos, float staggerValue)
{
    iX = x;
    iY = y;

    iNumberOfBlocks = pow(pow(4.0, size * 0.5), 2) + 1;
    // Rows and columns start at 0, which is unintuitive now but makes the code a hell of a lot easier to read later.
    // In this circumstance iRows and iColumns will always be the same. When this code is converted to its own
    // heightmap generator class, there may be some situation where the dimensions are different.
    iDimensions = pow(4.0, size * 0.5) + 1;

    // iQuadrants is the number of quadrants that are going to be generated
    // Or in other terms, the number of subdivisions.
    // iQuadrants = 1 will have 9 vertices, iQuadrants = 2 will have 25 vertices [I THINK]
    iQuadrants = size;

    // FinalX is the far right x point, in terms of a pointer.
    iFinalX = (iDimensions - 1)*iDimensions;
    iFinalPoint = iFinalX + iDimensions - 1;

    // Create a clear path for pHMBlocks
    float heightMap[iDimensions*iDimensions];
    for(long unsigned int i = 0; i < iDimensions*iDimensions; i++)
        heightMap[i] = 0;
    pHMBlocks = &heightMap[0];

    generateHeightmap(talos, staggerValue);
}

void HeightMapGen::genQuadrant(int xNW, int yNW, int xSE, int ySE, int iteration, int quadrant)
{
    // Generate a quadrant by giving each of the corners
    // Relative x and y are integers which give the x and y coordinates confined to that quadrant.
    // Therefore (iRelativeX/2) + xNW = Halfway along x
    // x values are already multiplied by iDimensions

    int iHalfRelativeX = (xSE - xNW)/2 + xNW; // This is never not used, but it is pausible that vanilla iRelativeX/2
    int iHalfRelativeY = (ySE - yNW)/2 + yNW; // Same with iHalfRelativeX

    // Calculating the midpoints, i.e.
    /*
      --+--
      |.|.|
      +-+-+
      |.|.|
      --+--
      where all + signs are calculated.
      */

    // How to access a two dimensional array pointer
    // To get array[8][7] where we have array[x][y], x would equal 8 and y would equal 7.
    // For a pointer, this is essentially element 87, or *(pArray + iDimensions*x + y)

    // Center
    *(pHMBlocks + iHalfRelativeX + iHalfRelativeY) = (*(pHMBlocks + xNW + yNW) + *(pHMBlocks + xSE + yNW) + *(pHMBlocks + xSE + ySE) + *(pHMBlocks + xNW + ySE))/4
                                    + Random::getSingleton().getRand(-fStaggerValue/iteration, fStaggerValue/iteration);

    // TODO: Check to make sure these haven't already been assigned, as there would be no point to assigning them again
    // N
    if(*(pHMBlocks + iHalfRelativeX + yNW) == 0)
        *(pHMBlocks + iHalfRelativeX + yNW) = (*(pHMBlocks + xNW + yNW) + *(pHMBlocks + xSE + yNW))/2
                                            + Random::getSingleton().getRand(-fStaggerValue/iteration, fStaggerValue/iteration);

    // E
    if(*(pHMBlocks + xSE + iHalfRelativeY) == 0)
        *(pHMBlocks + xSE + iHalfRelativeY) = (*(pHMBlocks + xSE + yNW) + *(pHMBlocks + xSE + ySE))/2
                                            + Random::getSingleton().getRand(-fStaggerValue/iteration, fStaggerValue/iteration);

    // S
    if(*(pHMBlocks + iHalfRelativeX + ySE) == 0)
        *(pHMBlocks + iHalfRelativeX + ySE) = (*(pHMBlocks + xNW + ySE) + *(pHMBlocks + xSE + ySE))/2
                                            + Random::getSingleton().getRand(-fStaggerValue/iteration, fStaggerValue/iteration);

    // W
    if(*(pHMBlocks + xNW + iHalfRelativeY) == 0)
        *(pHMBlocks + xNW + iHalfRelativeY) = (*(pHMBlocks + xNW + yNW) + *(pHMBlocks + xNW + ySE))/2
                                            + Random::getSingleton().getRand(-fStaggerValue/iteration, fStaggerValue/iteration);

    if (quadrant != 1)
    {
        // This then carries on the process of generating quadrants, providing there are still quadrants to generate.
        // Because quadrants count down, we know we do not need to continue to the loop if we are on the last
        // quadrant.
        // Essentially, you simply repeat the process found here in which you find the midpoints by using the midpoints
        // generated here as the corners in the next call of this function.
        // Increase iteration for the new set of quadrants
        iteration *= 2;

        // NW
        genQuadrant(xNW, yNW, iHalfRelativeX, iHalfRelativeY, iteration, quadrant - 1);

        // NE
        genQuadrant(iHalfRelativeX, yNW, xSE, iHalfRelativeY, iteration, quadrant - 1);

        // SE
        genQuadrant(iHalfRelativeX , iHalfRelativeY, xSE, ySE, iteration, quadrant - 1);

        // SW
        genQuadrant(xNW, iHalfRelativeY, iHalfRelativeX, ySE, iteration, quadrant - 1);
    }
}

// Checks to see if the neigbhourBlock exists, and then calculates the difference between the two blocks.
float HeightMapGen::getHeightDifference(long unsigned int inspectedBlock, long unsigned int neighbourBlock)
{
    if(neighbourBlock >= 0 && neighbourBlock <= iFinalPoint)
    {
        float fReturnValue = *(pHMBlocks + inspectedBlock) - *(pHMBlocks + neighbourBlock);
        return fReturnValue;
    }
    else
        return 0.0f; // For transportMaterial, since if we get to the edge material goes into the void.
}

void HeightMapGen::erodeBlock(float c, long unsigned int block)
{
    // Erode an individual block, and if this passes material down, then calculate erosion for that as well.
    // dNW, dNE etc. = difference in heights at point NW, NE etc. Set as 0 so if the points don't exist they don't cause
    // errors later.
    float dNW = 0;
    float dNE = 0;
    float dSW = 0;
    float dSE = 0;
    // dTotal = The total of all the d's that are above Talus.
    float dTotal = 0;
    // dMax = The highest d value
    float dMax = 0;
    // tNW, tNE etc. = the transported material at point NW, NE etc.
    float tNW = 0;
    float tNE = 0;
    float tSE = 0;
    float tSW = 0;
    // Define height values
    // Check if the point exists by seeing if is above the max or below the min
    // NW
    dNW = getHeightDifference(block, block - iDimensions - 1);
    if(dNW > fTalos)
    {
        // Skipping the calculations because we know this if we are at this point dTotal and dMax haven't been set
        dTotal = dNW;
        dMax = dNW;
    }
    else
        dNW = 0; // Which later means we will not change this block

    // NE
    dNE = getHeightDifference(block, block + iDimensions - 1);
    if(dNE > fTalos)
    {
        dTotal += dNE;
        if(dNE > dMax)
            dMax = dNE;
    }
    else
        dNE = 0; // Which later means we will not change this block

    // SE
    dSE = getHeightDifference(block, block + iDimensions + 1);
    if(dSE > fTalos)
    {
        dTotal += dSE;
        if(dSE > dMax)
            dMax = dSE;
    }
    else
        dSE = 0; // Which later means we will not change this block

    // SW
    dSW = getHeightDifference(block, block - iDimensions + 1);
    if(dSW > fTalos)
    {
        dTotal += dSW;
        if(dSW > dMax)
            dMax = dSW;
    }
    else
        dSW = 0; // Which later means we will not change this block

    // Go to each of the corners and apply the equation
    // hi += c(dmax - talos) * di/dtotal
    // NW
    if(dNW != 0)
    {
        tNW = c * (dMax - fTalos) * (dNW/dTotal);
        *(pHMBlocks + block - iDimensions - 1) += tNW;
    }
    // NE
    if(dNE != 0)
    {
        tNE = c * (dMax - fTalos) * (dNE/dTotal);
        *(pHMBlocks + block + iDimensions - 1) += tNE;
    }
    // SE
    if(dSE != 0)
    {
        tSE = c * (dMax - fTalos) * (dSE/dTotal);
        *(pHMBlocks + block + iDimensions + 1) += tSE;
    }
    // SW
    if(dSW != 0)
    {
        tSW = c * (dMax - fTalos) * (dSW/dTotal);
        *(pHMBlocks + block - iDimensions + 1) += tSW;
    }
    // ToDo; remove material transported?

    // Move the transported material to lower pastures
    if(tNW != 0)
        transportMaterial(tNW, block - iDimensions - 1);
    if(tNE != 0)
        transportMaterial(tNE, block + iDimensions - 1);
    if(tSE != 0)
        transportMaterial(tSE, block + iDimensions + 1);
    if(tSW != 0)
        transportMaterial(tSW, block - iDimensions + 1);
}

void HeightMapGen::transportMaterial(float material, unsigned long int block)
{
    // Get the difference in heights of all neighbours
    // Send the material to the lowest neighbour
    // If no neighbours are below this one (i.e. they are all positive) then add the material to the block.
    // dNW, dNE etc. = difference in heights at point NW, NE etc. Set as 0 so if the points don't exist they don't cause
    // errors later.
    float dNW = 0;
    float dNE = 0;
    float dSW = 0;
    float dSE = 0;
    // dMax = The largest d value
    float dMax = -1; // Otherwise anything negative would never overtake this, apart from NW
    // Define height values
    // Check if the point exists by seeing if is above the max or below the min
    // NW
    dNW = getHeightDifference(block, block - iDimensions - 1);
    dMax = dNW;
    // NE
    dNE = getHeightDifference(block, block + iDimensions - 1);
    if(dNE > dMax)
        dMax = dNE;
    // SE
    dSE = getHeightDifference(block, block + iDimensions + 1);
    if(dSE > dMax)
        dMax = dSE;
    // SW
    dSW = getHeightDifference(block, block - iDimensions + 1);
    if(dSW > dMax)
        dMax = dSW;

    // If the biggest d value we have is negative, we are the lowest of our neighbours, so add material to our height.
    if(dMax < 0)
        *(pHMBlocks + block) += material;
    else
    {
        // Otherwise send the material further down, to the lowest point nearby.
        if(dNW == dMax)
            transportMaterial(material, block - iDimensions - 1);
        else if(dNE == dMax)
            transportMaterial(material, block + iDimensions - 1);
        else if(dSE == dMax)
            transportMaterial(material, block + iDimensions + 1);
        else if (dSW == dMax)
            transportMaterial(material, block - iDimensions + 1);
        else
        {
            // We shouldn't be here. Why are we here?
            // Something has gone horribly wrong.
            std::cout << "Uh... something has gone horribly wrong inside transportMaterial....\n";
        }
    }
}

// Define a row of the terrain which is supplied in one of the arrays. Then run normal generation. Don't do this for NULL arrays.
// This may cause weird terrain if a weird amount of floats is supplied. It should be obvious if that happens.

void HeightMapGen::generateHeightmap(float talos, float staggerValue, float *northArray, float *eastArray, float *southArray, float *westArray)
{
    short unsigned int i;
    if(northArray != NULL)
    {
        for (i = 0; i < iDimensions; i++)
        {
            *(pHMBlocks + i) = *(northArray + i);
        }
    }

    if(eastArray != NULL)
    {
        for (i = 0; i < iDimensions; i++)
        {
            *(pHMBlocks + iDimensions*(i + 1) - 1) = *(eastArray + i);
        }
    }

    if(southArray != NULL)
    {
        for (i = 0; i < iDimensions; i++)
        {
            *(pHMBlocks + iDimensions*(iDimensions - 1) + i) = *(southArray + i);
        }
    }

    if(westArray != NULL)
    {
        for (i = 0; i < iDimensions; i++)
        {
            *(pHMBlocks + iDimensions*i) = *(westArray + i);
        }
    }

    generateHeightmap(talos, staggerValue);
}

// retrieveHeightmap
//  talos = the angle at which a slope must be for erosion to take place
//  heightmapArray = The array we will put the heighmap into.
//  staggerValue = The degree to which the stagger value will be increased/decreased. 1 = 100% (normal)

void HeightMapGen::generateHeightmap(float talos, float staggerValue)
{
    fTalos = talos; // A good default is 4/iNumberOfBlocks, however this will change depending on how eroded the terrain should be.

    fStaggerValue = iNumberOfBlocks*255*staggerValue;

    // NW corner
    *pHMBlocks = Random::getSingleton().getRand(5, 10);
    std::cout << "NW corner = " << *pHMBlocks << "\n";

    // NE corner
    *(pHMBlocks + iFinalX) = Random::getSingleton().getRand(5, 10);
    std::cout << "NE corner = " << *(pHMBlocks + iFinalX) << "\n";

    // SE corner
    *(pHMBlocks + iFinalX + iDimensions - 1) = Random::getSingleton().getRand(5, 10);
    std::cout << "SE corner = " << *(pHMBlocks + iFinalX + iDimensions - 1) << "\n";

    // SW corner
    *(pHMBlocks + iDimensions - 1) = Random::getSingleton().getRand(5, 10);
    std::cout << "SW corner = " << *(pHMBlocks + iDimensions - 1) << "\n";

    genQuadrant(0, 0, iFinalX, iDimensions - 1, 1, iQuadrants);

    // -----------------------
    // Set up the return value
    // -----------------------

    // A heightmap requires values ranging from 0 and 1. For now, everything is rated in proportion to the largest height
    // in the vector, so the highest point will be 1 and the lowest point will be 0
    // Step 1 = Get the largest and smallest height
    long signed int iMaxHeight;
    long signed int iMinHeight;

    // Setting a default iMaxHeight and iMinHeight to the northwest corner
    iMaxHeight = *pHMBlocks;
    iMinHeight = *pHMBlocks;

    // Go through the blocks and find out what is the highest and what is the lowest block
    for(long unsigned int i = 0; i <= iFinalX + iDimensions - 1; i++)
    {
        if(*(pHMBlocks + i) < iMinHeight)
            iMinHeight = *(pHMBlocks + i);
        if(*(pHMBlocks + i) > iMaxHeight)
            iMaxHeight = *(pHMBlocks + i);

    }

    // Advised for CPU optimisation
    short signed int iRelativeMaxHeight = iMaxHeight - iMinHeight;

    // Go through the blocks and scale them relative to the maximum or minimum
    // The percentage range can be found by first subtracting the miminum from the number
    // And then dividing that by iRelativeMaxHeight
    for(long unsigned int i = 0; i <= iFinalX + iDimensions - 1; i++)
    {
        // Scale it up to the point we want (by making it between 0 and 1 and then multiplying it by the range we want
        // Scaling disabled temporarily
        *(pHMBlocks + i) = ((*(pHMBlocks + i) - iMinHeight)/iRelativeMaxHeight);
    }

    // -------
    // Erosion
    // -------
    // Talos = The maximum angle allowed for slopes to be before thermal erosion begins taking place.
    // In most cases, we want this to be 4/N, where N = the number of blocks in the terrain set.
    // c = A variable I don't fully understand.

    // This uses Thermal Erosion
    // In the future, should probably modify it so that we can also have Rainfall erosion.
    // First we travel through all of the different HMBlocks and calculate the difference in height to
    // all of their neighbours, if they exist.
    // d1 = h - h1
    // where h = the height of the inspected block, h1 = the height of the neigbouring block at position 1, and d1
    // is the difference in height between inspected block 1 and the inspected block.
    // If the tile is higher than its neighbour, it will be positive, otherwise negative.

    // Use a for statement to call erodeBlock on every block in the array in order
    for(long unsigned int i = 0; i <= iFinalPoint; i++)
        erodeBlock(-0.5, i);

    writeMap();
}

void HeightMapGen::getSlopemap(float *slopemapArray)
{
    // FinalX is the far right x point, in terms of a pointer.
    iFinalX = (iDimensions - 1)*iDimensions;
    iFinalPoint = iFinalX + iDimensions - 1;

    // -------------------
    // Generate a slopemap
    // -------------------
    // This is necessary for vegetation, structures and textures, as trees and grass won't grow on cliffs, etc.
    for(long unsigned int i = 0; i <= iFinalPoint; i++)
    {
        // Add the total of all the differences and put it into the slopemapArray
        // which is NW, NE, SE, and SW
        *(slopemapArray + i) = getHeightDifference(i, i - iDimensions - 1)
                             + getHeightDifference(i, i + iDimensions - 1)
                             + getHeightDifference(i, i + iDimensions + 1)
                             + getHeightDifference(i, i - iDimensions + 1);
    }
}

void HeightMapGen::writeMap()
{
    std::ofstream myfile;
    myfile.open("map.txt");
    short int coutValue = 0;
    for(int x = 0; x < iDimensions; x++)
    {
        for(int y = 0; y < iDimensions; y++)
        {
            coutValue = *(pHMBlocks + x*iDimensions + y) * 100;
            // cout << coutValue << " ";
            myfile << coutValue << " ";
        }
        myfile << "\n";
    }
    myfile.close();
}
