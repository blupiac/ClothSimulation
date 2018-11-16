//-----------------------------------------------------------------------------
// Author: Bernard Lupiac
// Created: 15/11/2018
//-----------------------------------------------------------------------------

#pragma once

#include <vector>

#include "Vec3.hpp"

// Sample code for physics simulation
class ClothSimulationSystem 
{

public:

    ClothSimulationSystem(std::vector<Vec3f>& pos);

    void TimeStep(float stepSize);

private:

    std::vector<Vec3f> *m_currPos, *m_oldPos, *m_forces;

    void AccumulateForces();
    void Verlet(float stepSize);
    void SatisfyConstraints();
};
