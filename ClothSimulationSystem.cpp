//-----------------------------------------------------------------------------
// Author: Bernard Lupiac
// Created: 15/11/2018
// Based on: https://www.gamasutra.com/view/feature/131313/advanced_character_physics.php
// and class notes
//-----------------------------------------------------------------------------

#include "ClothSimulationSystem.hpp"
#include "Vec3.hpp"

ClothSimulationSystem::ClothSimulationSystem(std::vector<Vec3f>& pos)
{
    int numParticles = pos.size();

    m_currPos = pos;
    
    m_oldPos.resize(numParticles);
    m_forces.resize(numParticles);
}

std::vector<Vec3f> ClothSimulationSystem::getPos()
{
    return m_currPos;
}

void ClothSimulationSystem::AccumulateForces()
{

}

void ClothSimulationSystem::Verlet(float stepSize) 
{

}

void ClothSimulationSystem::SatisfyConstraints()
{

}

void ClothSimulationSystem::TimeStep(float stepSize) 
{
    AccumulateForces();
    Verlet(stepSize);
    SatisfyConstraints();
} 