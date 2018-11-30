//-----------------------------------------------------------------------------
// Author: Bernard Lupiac
// Created: 15/11/2018
//-----------------------------------------------------------------------------

#pragma once

#include <vector>

#include "Vec3.hpp"

struct Constraint {
    int idxA, idxB;
    float restlength;
};

// Sample code for physics simulation
class ClothSimulationSystem 
{

public:

    ClothSimulationSystem();
    
    ClothSimulationSystem(std::vector<Vec3f>& pos,
                            std::vector<Constraint>& constraints);
    std::vector<Vec3f> getPos();
    std::vector<Constraint> getConstraints();

    void ApplyForce(Vec3f forceDirection);
    void TimeStep(float stepSize);

private:

    std::vector<Vec3f> m_currPos, m_oldPos, m_forces;
    std::vector<Constraint> m_constraints;

    void AccumulateForces(float stepSize);
    void Verlet(float stepSize);
    void SatisfyConstraints();
};
