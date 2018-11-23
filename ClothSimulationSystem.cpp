//-----------------------------------------------------------------------------
// Author: Bernard Lupiac
// Created: 15/11/2018
// Based on: https://www.gamasutra.com/view/feature/131313/advanced_character_physics.php
// and class notes
//-----------------------------------------------------------------------------

#include <math.h>
#include <iostream>

#include "ClothSimulationSystem.hpp"
#include "Vec3.hpp"

static const float particleMass = 1.0f;

ClothSimulationSystem::ClothSimulationSystem(std::vector<Vec3f>& pos,
                            std::vector<Constraint>& constraints)
{
    int numParticles = pos.size();

    m_currPos = pos;

    m_constraints = m_constraints;
    
    m_oldPos.resize(numParticles);
    m_forces.resize(numParticles);

    for(int i = 0; i < m_currPos.size(); i++)
    {
        m_oldPos[i] = m_currPos[i];
        m_forces[i] = Vec3f(0.0f, 0.0f, 0.0f);
    }
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
    for(int i = 0; i < m_currPos.size(); i++)
    {
        Vec3f newPos = (m_currPos[i] + m_currPos[i]) - m_oldPos[i] +
            m_forces[i] * (stepSize * stepSize / particleMass);

        m_oldPos[i] = m_currPos[i];
        m_currPos[i] = newPos;
    }
}

void ClothSimulationSystem::SatisfyConstraints()
{
    int numIter = 5;

    for(int i = 0; i < numIter; i++)
    {
        for(std::vector<Constraint>::iterator it = m_constraints.begin();
            it != m_constraints.end(); ++it) 
        {
            Constraint c = *it;
            Vec3f pA = m_currPos[c.idxA];
            Vec3f pB = m_currPos[c.idxB];

            Vec3f delta = pB - pA;
            float deltaLength = sqrt(delta.dot(delta));
            float diff = (deltaLength - c.restlength) / deltaLength;

            std::cout << diff << std::endl;

            m_currPos[c.idxA] = pA - (delta * (0.5f * diff));
            m_currPos[c.idxB] = pB + (delta * (0.5f * diff));
        }
    }
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