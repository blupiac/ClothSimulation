//-----------------------------------------------------------------------------
// Author: Bernard Lupiac
// Created: 15/11/2018
// Based on: https://www.gamasutra.com/view/feature/131313/advanced_character_physics.php
// and class notes
//-----------------------------------------------------------------------------

#include "ClothSimulationSystem.hpp"
#include "Vec3.hpp"

ClothSimulationSystem::ClothSimulationSystem(std::vector<Vec3f>& pos,
                            std::vector<Constraint>& constraints)
{
    int numParticles = pos.size();

    m_currPos = pos;

    m_constraints = m_constraints;
    
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

void ClothSimulationSystem::ApplyForce(Vec3f forceDirection)
{
    for(int i = 0; i < m_forces.size(); i++)
    {
        m_forces[i] = m_forces[i] + forceDirection;
    }
}

void ClothSimulationSystem::TimeStep(float stepSize) 
{
    AccumulateForces();
    Verlet(stepSize);
    SatisfyConstraints();
} 