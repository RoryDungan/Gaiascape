#include <iostream>
#include "algorithms\HeightMapGen.h"
#include <math.h>
#include <fstream>
#include <QImage>
#include <QByteArray>

HeightMapGen::HeightMapGen(float talos, unsigned int size)
{
    // Moved to retrieveHeightmap()
    // Everything remaining has been commented out in case we go back, so this is sort of a recycling bin.

    // The active ID that all blocks are assigned. Incremented later in the for loop.
    /*int iBlockID = 0;

    for(short unsigned int y = 0; y < (iDimensions + 1); y++)
    {
        for (short unsigned int x = 0; x < (iDimensions + 1); x++)
        {
            HMBlock* tempPointer = new HMBlock(x,y,iBlockID);
            HMBlocks.push_back(tempPointer); // Program each column along the row
            iBlockID++;
        }
    }*/
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

    // N
    *(pHMBlocks + iHalfRelativeX + yNW) = (*(pHMBlocks + xNW + yNW) + *(pHMBlocks + xSE + yNW) + *(pHMBlocks + iHalfRelativeX + iHalfRelativeY))/3
                                    + Random::getSingleton().getRand(-fStaggerValue/iteration, fStaggerValue/iteration);

    // E
    *(pHMBlocks + xSE + iHalfRelativeY) = (*(pHMBlocks + xSE + yNW) + *(pHMBlocks + xSE + ySE) + *(pHMBlocks + iHalfRelativeX + iHalfRelativeY))/3
                                    + Random::getSingleton().getRand(-fStaggerValue/iteration, fStaggerValue/iteration);

    // S
    *(pHMBlocks + iHalfRelativeX + ySE) = (*(pHMBlocks + xNW + ySE) + *(pHMBlocks + iHalfRelativeX + iHalfRelativeY) + *(pHMBlocks + xSE + ySE))/3
                                    + Random::getSingleton().getRand(-fStaggerValue/iteration, fStaggerValue/iteration);

    // W
    *(pHMBlocks + xNW + iHalfRelativeY) = (*(pHMBlocks + xNW + yNW) + *(pHMBlocks + xNW + ySE) + *(pHMBlocks + iHalfRelativeX + iHalfRelativeY))/3
                                    + Random::getSingleton().getRand(-fStaggerValue/iteration, fStaggerValue/iteration);

    if (quadrant != 1)
    {
        // This then carries on the process of generating quadrants, providing there are still quadrants to generate.
        // Because quadrants count down, we know we do not need to continue to the loop if we are on the last
        // quadrant.
        // Essentially, you simply repeat the process found here in which you find the midpoints by using the midpoints
        // generated here as the corners in the next call of this function.

        // NW
        genQuadrant(xNW, yNW, iHalfRelativeX, iHalfRelativeY, iteration + 1, quadrant - 1);

        // NE
        genQuadrant(iHalfRelativeX, yNW, xSE, iHalfRelativeY, iteration + 1, quadrant - 1);

        // SE
        genQuadrant(iHalfRelativeX , iHalfRelativeY, xSE, ySE, iteration + 1, quadrant - 1);

        // SW
        genQuadrant(xNW, iHalfRelativeY, iHalfRelativeX, ySE, iteration + 1, quadrant - 1);
    }
}

void erode(short int c)
{
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

    // Grab the first terrain block in the array
    // If the neighbour at NW exists
    //
}

short unsigned int HeightMapGen::retrieveDimensions()
{
    return iDimensions;
}

void HeightMapGen::retrieveHeightmap(float talos, unsigned int size, float* heightmapArray)
{
    iNumberOfBlocks = pow(pow(4.0, size * 0.5), 2) + 1;
    fTalos = talos; // A good default is 4/iNumberOfBlocks, however this will change depending on how eroded the terrain should be.
    // Rows and columns start at 0, which is unintuitive now but makes the code a hell of a lot easier to read later.
    // In this circumstance iRows and iColumns will always be the same. When this code is converted to its own
    // heightmap generator class, there may be some situation where the dimensions are different.
    iDimensions = pow(4.0, size * 0.5) + 1;

    // iQuadrants is the number of quadrants that are going to be generated
    // Or in other terms, the number of subdivisions.
    // iQuadrants = 1 will have 9 vertices, iQuadrants = 2 will have 25 vertices [I THINK]
    iQuadrants = size;
    fStaggerValue = iNumberOfBlocks / 20;

    pHMBlocks = heightmapArray;

    // FinalX is the far right x point, in terms of a pointer.
    long unsigned int iFinalX = (iDimensions - 1)*iDimensions;

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
        *(pHMBlocks + i) = ((*(pHMBlocks + i) - iMinHeight)/iRelativeMaxHeight)*1;
    }

    QByteArray data((const char*)(iFinalX + iDimensions - 1));

    for(unsigned int i = 0; i < iFinalX + iDimensions - 1; ++i)
    {
        data[i] = (unsigned char)(*(pHMBlocks + i)); // Probably just put in the above if statement if it works
    }

    QVector<QRgb> grayscale;

    for(long unsigned int i = 0; i < 256; ++i)
        grayscale.append(qRgb(i, i, i));

    QImage image((uchar*)(data.constData()), iDimensions, iDimensions, QImage::Format_Indexed8);
    image.setColorTable(grayscale);


    QString filename = "map.bmp";
    image.save(filename, 0, -1);

    writeMap();
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
