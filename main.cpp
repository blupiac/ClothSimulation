//-----------------------------------------------------------------------------
// Author: Bernard Lupiac
// Created: 15/11/2018
//-----------------------------------------------------------------------------

#include <iostream>
#include <time.h>

#include "ClothSimulationSystem.hpp"
#include "Vec3.hpp"

void printVector(std::vector<Vec3f> vec)
{
    for(int i = 0; i < vec.size(); i++)
    {
        printf("Point %d: %lf, %lf, %lf \n", i, vec[i][0], vec[i][1], vec[i][2]);
    }
    printf("\n");
}

int main (int argc, char ** argv) 
{
    printf("Hello world.\n");

    std::vector<Vec3f> pos;
    pos.push_back(Vec3f(0.0f, 0.0f, 0.0f));
    pos.push_back(Vec3f(0.0f, 1.0f, 0.0f));
    pos.push_back(Vec3f(0.0f, 2.0f, 0.0f));
    pos.push_back(Vec3f(0.0f, 3.0f, 0.0f));
    pos.push_back(Vec3f(0.0f, 4.0f, 0.0f));

    ClothSimulationSystem clothSystem = ClothSimulationSystem(pos);

    int maxSteps = 10;
    time_t firstTime;
    time(&firstTime);
    time_t lastTime;
    time(&lastTime);

    for(int step = 0; step < maxSteps; step++)
    {
        time_t thisTime;
        time(&thisTime);
        clothSystem.TimeStep((double) difftime(thisTime,lastTime));

        printf("Time: %lf\n", (long double) difftime(thisTime,firstTime));
        std::vector<Vec3f> currentPos = clothSystem.getPos();
        printVector(currentPos);

        lastTime = thisTime;
    }

    return 0;
}