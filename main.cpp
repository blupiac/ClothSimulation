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

    std::vector<Constraint> constraints;
    Constraint c0, c1, c2, c3, c4;
    c0.idxA = 0; c0.idxB = 1; c0.restlength = 1.0f;
    c1.idxA = 1; c1.idxB = 2; c1.restlength = 1.0f;
    c2.idxA = 2; c2.idxB = 3; c2.restlength = 1.0f;
    c3.idxA = 3; c3.idxB = 4; c3.restlength = 1.0f;
    constraints.push_back(c0);
    constraints.push_back(c1);
    constraints.push_back(c2);
    constraints.push_back(c3);

    ClothSimulationSystem clothSystem = ClothSimulationSystem(pos, constraints);

    int maxSteps = 100;
    clock_t firstClock, lastClock;
    lastClock = firstClock = clock();

    Vec3f gravity = Vec3f(0.0f, -9.81f, 0.0f);

    for(int step = 0; step < maxSteps; step++)
    {
        clock_t thisClock = clock();
        double deltaT = (thisClock - lastClock) / (double) CLOCKS_PER_SEC;
        double currTime = (thisClock - firstClock) / (double) CLOCKS_PER_SEC;

        clothSystem.ApplyForce(gravity);
        clothSystem.TimeStep(deltaT);

        printf("Time: %lf, deltaT: %lf\n", currTime, deltaT);
        std::vector<Vec3f> currentPos = clothSystem.getPos();
        printVector(currentPos);

        lastClock = thisClock;
    }

    return 0;
}