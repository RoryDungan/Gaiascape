#ifndef RANDOM_H
#define RANDOM_H
#include <cstdlib>
#include <ctime>

class Random
{
    public:
        static Random& getSingleton()
        {
            static Random mRandom;
            return mRandom;
        }
        int getRand(int min, int max)
        {
            return rand() % (max-min+1) + min; // difference between max and min + 1 + min
        }
    protected:
    private:
        Random() // Private constructor
        {
            srand(time(NULL));
        }
        Random(const Random&);              // Prevent copy-construction
        Random& operator=(const Random&);   // Prevent assignment
};

#endif // RANDOM_H
