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
#include "Camera.hpp"

static const float particleMass = 1.0f;

ClothSimulationSystem::ClothSimulationSystem()
{

}

ClothSimulationSystem::ClothSimulationSystem(std::vector<Vec3f>& pos,
                            std::vector<Constraint>& constraints,
                            std::vector<bool>& isMovable)
{
    int numParticles = pos.size();

    m_currPos = pos;
    m_constraints = constraints;
    m_isMovable = isMovable;
    
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

std::vector<Constraint> ClothSimulationSystem::getConstraints()
{
    return m_constraints;
}

void ClothSimulationSystem::AccumulateForces(float stepSize)
{
    for(int i = 0; i < m_currPos.size(); i++)
    {
    	if(m_isMovable[i])
    	{
	        m_currPos[i] = m_currPos[i] + m_forces[i] * stepSize;
	        m_forces[i] = Vec3f(0.0f, 0.0f, 0.0f); // force has been applied
	    }
    }
}

void ClothSimulationSystem::Verlet(float stepSize) 
{
    for(int i = 0; i < m_currPos.size(); i++)
    {
    	if(m_isMovable[i])
    	{
    		Vec3f newPos = (m_currPos[i] + m_currPos[i]) - m_oldPos[i] +
	            m_forces[i] * (stepSize * stepSize / particleMass);

	        m_oldPos[i] = m_currPos[i];
	        m_currPos[i] = newPos;
    	}
    }
}

void ClothSimulationSystem::SatisfyConstraints()
{
    int numIter = 5;

    for(int i = 0; i < numIter; i++)
    {
        // makes sure constraints specified during creation are respected
        for(std::vector<Constraint>::iterator it = m_constraints.begin();
            it != m_constraints.end(); ++it) 
        {
            Constraint c = *it;
            Vec3f pA = m_currPos[c.idxA];
            Vec3f pB = m_currPos[c.idxB];

            Vec3f delta = pB - pA;
            float deltaLength = sqrt(delta.dot(delta));
            float diff = (deltaLength - c.restlength) / deltaLength;

            if(m_isMovable[c.idxA] && m_isMovable[c.idxB])
    		{
	            m_currPos[c.idxA] = pA + (delta * (0.5f * diff));
	            m_currPos[c.idxB] = pB - (delta * (0.5f * diff));
	        }
	        else if(m_isMovable[c.idxA] && !m_isMovable[c.idxB])
	        {
	        	m_currPos[c.idxA] = pA + (delta * diff);
	        }
	        else if(!m_isMovable[c.idxA] && m_isMovable[c.idxB])
	        {
	        	m_currPos[c.idxB] = pB - (delta * diff);
	        }
	        // else: none of them can move, tough luck
        }

        // makes sure y coordinate can't be negative
        for(int i = 0; i < m_currPos.size(); i++)
        {
            m_currPos[i][1] = std::max(0.0f, m_currPos[i][1]);
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
    AccumulateForces(stepSize);
    Verlet(stepSize);
    SatisfyConstraints();
} 