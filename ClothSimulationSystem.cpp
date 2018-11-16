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

    m_currPos = &pos;
    
    m_oldPos = new std::vector<Vec3f>();
    m_oldPos->resize(numParticles);

    m_forces = new std::vector<Vec3f>();
    m_forces->resize(numParticles);
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