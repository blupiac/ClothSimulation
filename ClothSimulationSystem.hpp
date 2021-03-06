//---------------------------------------------------------------------------------------
// Author: Bernard Lupiac
// Created: 15/11/2018
// Based on: https://www.gamasutra.com/view/feature/131313/advanced_character_physics.php
// and class notes
//---------------------------------------------------------------------------------------

#pragma once

#include <vector>

#include "Vec3.hpp"

struct Constraint {
    int idxA, idxB;
    float restlength;
};


class ClothSimulationSystem 
{

public:

    ClothSimulationSystem();
    
    ClothSimulationSystem(std::vector<Vec3f>& pos,
                            std::vector<Constraint>& constraints,
                            std::vector<bool>& isMovable);
    std::vector<Vec3f> getPos();
    std::vector<Constraint> getConstraints();

    void ApplyForce(Vec3f forceDirection);
    void TimeStep(float stepSize);

private:

    std::vector<Vec3f> m_currPos, m_oldPos, m_forces;
    std::vector<Constraint> m_constraints;
    std::vector<bool> m_isMovable;

    void AccumulateForces(float stepSize);
    void Verlet(float stepSize);
    void SatisfyConstraints();
};
