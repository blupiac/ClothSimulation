//-----------------------------------------------------------------------------
// Author: Bernard Lupiac
// Created: 15/11/2018
//-----------------------------------------------------------------------------

#include <iostream>
#include <GL/glut.h>

#include "ClothSimulationSystem.hpp"
#include "Vec3.hpp"
#include "Camera.hpp"


static const unsigned int DEFAULT_SCREENWIDTH = 1024;
static const unsigned int DEFAULT_SCREENHEIGHT = 768;

static const float STANDARD_TIMESTEP = 0.001f;
static const float AUTO_UPDATE_FACTOR = 1.0e-7f;

static ClothSimulationSystem clothSystem;
static Camera camera;
static bool wind = false;
static bool autoUpdate = false;


void printVector(std::vector<Vec3f> vec)
{
    for(unsigned int i = 0; i < vec.size(); i++)
    {
        std::cout << "Point " << i << " : " << vec[i] << std::endl;
    }
    std::cout << std::endl;
}

Vec3f getWindForce()
{
    // https://stackoverflow.com/questions/686353/c-random-float-number-generation
    // random number from 0 to 10
    float randX = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/10.0f));
    float randY = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/10.0f));
    float randZ = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/10.0f));
    return Vec3f(randX, randY, randZ);
}

void renderScene() 
{
    std::vector<Vec3f> pos = clothSystem.getPos();
    std::vector<Constraint> constraints = clothSystem.getConstraints();

    //ground
    glBegin(GL_QUADS);
        glColor3d(0.46f,0.77f,0.68f);
        glVertex3f(-10.0f, 0.0f,-10.0f);
        glColor3d(0.46f,0.77f,0.68f);
        glVertex3f(-10.0f, 0.0f,10.0f);
        glColor3d(0.46f,0.77f,0.68f);
        glVertex3f(10.0f, 0.0f,10.0f);
        glColor3d(0.46f,0.77f,0.68f);
        glVertex3f(10.0f, 0.0f,-10.0f);
    glEnd();

    glBegin(GL_LINES);
        
    // each constraint = one line
    for(std::vector<Constraint>::iterator it = constraints.begin();
        it != constraints.end(); ++it) 
    {
        Constraint c = *it;
        Vec3f pA = pos[c.idxA];
        Vec3f pB = pos[c.idxB];

        glColor3d(0.85f,0.42,0.44);
        glVertex3f(pA[0], pA[1], pA[2]);
        glColor3d(0.85f,0.42,0.44);
        glVertex3f(pB[0], pB[1], pB[2]);
    }

    glEnd();
}

void applyCamera() 
{
    glLoadIdentity();

    Vec3f camPos = camera.getPos();
    Vec3f camFront = camera.getFront();
    Vec3f camUp = camera.getUp();

    gluLookAt(camPos[0],  camPos[1],  camPos[2],
                camPos[0] + camFront[0],  camPos[1] + camFront[1],  camPos[2] + camFront[2],
                camUp[0],  camUp[1],  camUp[2]);
}

void reshapeEventListener(int w, int h) 
{
    camera.resize (w, h);
}

void display () 
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    applyCamera();
    renderScene ();
    glFlush ();
    glutSwapBuffers (); 
}

void mouseEventListener (int button, int state, int x, int y) 
{
    camera.handleMouseClickEvent (button, state, x, y);
    display();
}

void motionEventListener (int x, int y) 
{
    camera.handleMouseMoveEvent (x, y);
    display();
}

void step(float deltaTime)
{
    Vec3f gravity = Vec3f(0.0f, -9.81f, 0.0f);

    if(wind)
    {
        clothSystem.ApplyForce(getWindForce());
    }

    clothSystem.ApplyForce(gravity);
    clothSystem.TimeStep(deltaTime);

    display();
}

void loadStringExample()
{
    std::vector<Vec3f> pos;
    pos.push_back(Vec3f(0.0f, 0.0f, 0.0f));
    pos.push_back(Vec3f(1.0f, 1.0f, 1.0f));
    pos.push_back(Vec3f(2.0f, 2.0f, 2.0f));
    pos.push_back(Vec3f(3.0f, 3.0f, 3.0f));
    pos.push_back(Vec3f(4.0f, 4.0f, 4.0f));

    std::vector<Constraint> constraints;
    Constraint c0, c1, c2, c3;
    c0.idxA = 0; c0.idxB = 1; c0.restlength = sqrt(3.0f);
    c1.idxA = 1; c1.idxB = 2; c1.restlength = sqrt(3.0f);
    c2.idxA = 2; c2.idxB = 3; c2.restlength = sqrt(3.0f);
    c3.idxA = 3; c3.idxB = 4; c3.restlength = sqrt(3.0f);
    constraints.push_back(c0);
    constraints.push_back(c1);
    constraints.push_back(c2);
    constraints.push_back(c3);

    std::vector<bool> isMovable;
    isMovable.resize(5);
    isMovable[0] = true;
    isMovable[1] = true;
    isMovable[2] = true;
    isMovable[3] = true;
    isMovable[4] = true;

    clothSystem = ClothSimulationSystem(pos, constraints, isMovable);
}

void loadCompressedStringExample()
{
    std::vector<Vec3f> pos;
    pos.push_back(Vec3f(0.0f, 0.0f, 0.0f));
    pos.push_back(Vec3f(1.0f, 1.0f, 1.0f));
    pos.push_back(Vec3f(2.0f, 2.0f, 2.0f));
    pos.push_back(Vec3f(3.0f, 3.0f, 3.0f));
    pos.push_back(Vec3f(4.0f, 4.0f, 4.0f));

    std::vector<Constraint> constraints;
    Constraint c0, c1, c2, c3;
    c0.idxA = 0; c0.idxB = 1; c0.restlength = 3.0f;
    c1.idxA = 1; c1.idxB = 2; c1.restlength = 3.0f;
    c2.idxA = 2; c2.idxB = 3; c2.restlength = 3.0f;
    c3.idxA = 3; c3.idxB = 4; c3.restlength = 3.0f;
    constraints.push_back(c0);
    constraints.push_back(c1);
    constraints.push_back(c2);
    constraints.push_back(c3);

    std::vector<bool> isMovable;
    isMovable.resize(5);
    isMovable[0] = true;
    isMovable[1] = true;
    isMovable[2] = true;
    isMovable[3] = true;
    isMovable[4] = true;

    clothSystem = ClothSimulationSystem(pos, constraints, isMovable);
}

void loadCubeExample()
{
    std::vector<Vec3f> pos;
    pos.push_back(Vec3f(1.0f, 1.0f, 1.0f));
    pos.push_back(Vec3f(2.0f, 1.0f, 1.0f));
    pos.push_back(Vec3f(2.0f, 2.0f, 1.0f));
    pos.push_back(Vec3f(1.0f, 2.0f, 1.0f));
    pos.push_back(Vec3f(1.0f, 1.0f, 2.0f));
    pos.push_back(Vec3f(2.0f, 1.0f, 2.0f));
    pos.push_back(Vec3f(2.0f, 2.0f, 2.0f));
    pos.push_back(Vec3f(1.0f, 2.0f, 2.0f));

    std::vector<Constraint> constraints;
    Constraint c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11;
    c0.idxA = 0; c0.idxB = 1; c0.restlength = 1.0f;
    c1.idxA = 1; c1.idxB = 2; c1.restlength = 1.0f;
    c2.idxA = 2; c2.idxB = 3; c2.restlength = 1.0f;
    c3.idxA = 3; c3.idxB = 0; c3.restlength = 1.0f;
    c4.idxA = 0; c4.idxB = 4; c4.restlength = 1.0f;
    c5.idxA = 1; c5.idxB = 5; c5.restlength = 1.0f;
    c6.idxA = 2; c6.idxB = 6; c6.restlength = 1.0f;
    c7.idxA = 3; c7.idxB = 7; c7.restlength = 1.0f;
    c8.idxA = 4; c8.idxB = 5; c8.restlength = 1.0f;
    c9.idxA = 5; c9.idxB = 6; c9.restlength = 1.0f;
    c10.idxA = 6; c10.idxB = 7; c10.restlength = 1.0f;
    c11.idxA = 7; c11.idxB = 4; c11.restlength = 1.0f;
    constraints.push_back(c0);
    constraints.push_back(c1);
    constraints.push_back(c2);
    constraints.push_back(c3);
    constraints.push_back(c4);
    constraints.push_back(c5);
    constraints.push_back(c6);
    constraints.push_back(c7);
    constraints.push_back(c8);
    constraints.push_back(c9);
    constraints.push_back(c10);
    constraints.push_back(c11);

    std::vector<bool> isMovable;
    isMovable.resize(8);
    isMovable[0] = true;
    isMovable[1] = true;
    isMovable[2] = true;
    isMovable[3] = true;
    isMovable[4] = true;
    isMovable[5] = true;
    isMovable[6] = true;
    isMovable[7] = true;

    clothSystem = ClothSimulationSystem(pos, constraints, isMovable);
}

void loadFixedStringExample()
{
    std::vector<Vec3f> pos;
    pos.push_back(Vec3f(0.0f, 3.0f, 0.0f));
    pos.push_back(Vec3f(1.0f, 4.0f, 1.0f));
    pos.push_back(Vec3f(2.0f, 5.0f, 2.0f));
    pos.push_back(Vec3f(3.0f, 6.0f, 3.0f));
    pos.push_back(Vec3f(4.0f, 7.0f, 4.0f));

    std::vector<Constraint> constraints;
    Constraint c0, c1, c2, c3;
    c0.idxA = 0; c0.idxB = 1; c0.restlength = sqrt(3.0f);
    c1.idxA = 1; c1.idxB = 2; c1.restlength = sqrt(3.0f);
    c2.idxA = 2; c2.idxB = 3; c2.restlength = sqrt(3.0f);
    c3.idxA = 3; c3.idxB = 4; c3.restlength = sqrt(3.0f);
    constraints.push_back(c0);
    constraints.push_back(c1);
    constraints.push_back(c2);
    constraints.push_back(c3);

    std::vector<bool> isMovable;
    isMovable.resize(5);
    isMovable[0] = true;
    isMovable[1] = true;
    isMovable[2] = true;
    isMovable[3] = true;
    isMovable[4] = false;

    clothSystem = ClothSimulationSystem(pos, constraints, isMovable);
}

void loadFixedCubeExample()
{
    std::vector<Vec3f> pos;
    pos.push_back(Vec3f(1.0f, 1.0f, 1.0f));
    pos.push_back(Vec3f(2.0f, 1.0f, 1.0f));
    pos.push_back(Vec3f(2.0f, 2.0f, 1.0f));
    pos.push_back(Vec3f(1.0f, 2.0f, 1.0f));
    pos.push_back(Vec3f(1.0f, 1.0f, 2.0f));
    pos.push_back(Vec3f(2.0f, 1.0f, 2.0f));
    pos.push_back(Vec3f(2.0f, 2.0f, 2.0f));
    pos.push_back(Vec3f(1.0f, 2.0f, 2.0f));

    std::vector<Constraint> constraints;
    Constraint c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11;
    c0.idxA = 0; c0.idxB = 1; c0.restlength = 1.0f;
    c1.idxA = 1; c1.idxB = 2; c1.restlength = 1.0f;
    c2.idxA = 2; c2.idxB = 3; c2.restlength = 1.0f;
    c3.idxA = 3; c3.idxB = 0; c3.restlength = 1.0f;
    c4.idxA = 0; c4.idxB = 4; c4.restlength = 1.0f;
    c5.idxA = 1; c5.idxB = 5; c5.restlength = 1.0f;
    c6.idxA = 2; c6.idxB = 6; c6.restlength = 1.0f;
    c7.idxA = 3; c7.idxB = 7; c7.restlength = 1.0f;
    c8.idxA = 4; c8.idxB = 5; c8.restlength = 1.0f;
    c9.idxA = 5; c9.idxB = 6; c9.restlength = 1.0f;
    c10.idxA = 6; c10.idxB = 7; c10.restlength = 1.0f;
    c11.idxA = 7; c11.idxB = 4; c11.restlength = 1.0f;
    constraints.push_back(c0);
    constraints.push_back(c1);
    constraints.push_back(c2);
    constraints.push_back(c3);
    constraints.push_back(c4);
    constraints.push_back(c5);
    constraints.push_back(c6);
    constraints.push_back(c7);
    constraints.push_back(c8);
    constraints.push_back(c9);
    constraints.push_back(c10);
    constraints.push_back(c11);

    std::vector<bool> isMovable;
    isMovable.resize(8);
    isMovable[0] = true;
    isMovable[1] = true;
    isMovable[2] = true;
    isMovable[3] = false;
    isMovable[4] = true;
    isMovable[5] = true;
    isMovable[6] = true;
    isMovable[7] = true;

    clothSystem = ClothSimulationSystem(pos, constraints, isMovable);
}

void loadFixedStrongCubeExample()
{
    std::vector<Vec3f> pos;
    pos.push_back(Vec3f(1.0f, 1.0f, 1.0f));
    pos.push_back(Vec3f(2.0f, 1.0f, 1.0f));
    pos.push_back(Vec3f(2.0f, 2.0f, 1.0f));
    pos.push_back(Vec3f(1.0f, 2.0f, 1.0f));
    pos.push_back(Vec3f(1.0f, 1.0f, 2.0f));
    pos.push_back(Vec3f(2.0f, 1.0f, 2.0f));
    pos.push_back(Vec3f(2.0f, 2.0f, 2.0f));
    pos.push_back(Vec3f(1.0f, 2.0f, 2.0f));

    std::vector<Constraint> constraints;
    Constraint c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11;
    c0.idxA = 0; c0.idxB = 1; c0.restlength = 1.0f;
    c1.idxA = 1; c1.idxB = 2; c1.restlength = 1.0f;
    c2.idxA = 2; c2.idxB = 3; c2.restlength = 1.0f;
    c3.idxA = 3; c3.idxB = 0; c3.restlength = 1.0f;
    c4.idxA = 0; c4.idxB = 4; c4.restlength = 1.0f;
    c5.idxA = 1; c5.idxB = 5; c5.restlength = 1.0f;
    c6.idxA = 2; c6.idxB = 6; c6.restlength = 1.0f;
    c7.idxA = 3; c7.idxB = 7; c7.restlength = 1.0f;
    c8.idxA = 4; c8.idxB = 5; c8.restlength = 1.0f;
    c9.idxA = 5; c9.idxB = 6; c9.restlength = 1.0f;
    c10.idxA = 6; c10.idxB = 7; c10.restlength = 1.0f;
    c11.idxA = 7; c11.idxB = 4; c11.restlength = 1.0f;
    constraints.push_back(c0);
    constraints.push_back(c1);
    constraints.push_back(c2);
    constraints.push_back(c3);
    constraints.push_back(c4);
    constraints.push_back(c5);
    constraints.push_back(c6);
    constraints.push_back(c7);
    constraints.push_back(c8);
    constraints.push_back(c9);
    constraints.push_back(c10);
    constraints.push_back(c11);

    // diagonal constraints
    Constraint d0, d1, d2, d3;
    d0.idxA = 0; d0.idxB = 6; d0.restlength = sqrt(3.0f);
    d1.idxA = 1; d1.idxB = 7; d1.restlength = sqrt(3.0f);
    d2.idxA = 2; d2.idxB = 4; d2.restlength = sqrt(3.0f);
    d3.idxA = 3; d3.idxB = 5; d3.restlength = sqrt(3.0f);
    constraints.push_back(d0);
    constraints.push_back(d1);
    constraints.push_back(d2);
    constraints.push_back(d3);

    std::vector<bool> isMovable;
    isMovable.resize(8);
    isMovable[0] = true;
    isMovable[1] = true;
    isMovable[2] = true;
    isMovable[3] = false;
    isMovable[4] = true;
    isMovable[5] = true;
    isMovable[6] = true;
    isMovable[7] = true;

    clothSystem = ClothSimulationSystem(pos, constraints, isMovable);
}

void loadFixedExtraStrongCubeExample()
{
    std::vector<Vec3f> pos;
    pos.push_back(Vec3f(1.0f, 1.0f, 1.0f));
    pos.push_back(Vec3f(2.0f, 1.0f, 1.0f));
    pos.push_back(Vec3f(2.0f, 2.0f, 1.0f));
    pos.push_back(Vec3f(1.0f, 2.0f, 1.0f));
    pos.push_back(Vec3f(1.0f, 1.0f, 2.0f));
    pos.push_back(Vec3f(2.0f, 1.0f, 2.0f));
    pos.push_back(Vec3f(2.0f, 2.0f, 2.0f));
    pos.push_back(Vec3f(1.0f, 2.0f, 2.0f));

    std::vector<Constraint> constraints;
    Constraint c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11;
    c0.idxA = 0; c0.idxB = 1; c0.restlength = 1.0f;
    c1.idxA = 1; c1.idxB = 2; c1.restlength = 1.0f;
    c2.idxA = 2; c2.idxB = 3; c2.restlength = 1.0f;
    c3.idxA = 3; c3.idxB = 0; c3.restlength = 1.0f;
    c4.idxA = 0; c4.idxB = 4; c4.restlength = 1.0f;
    c5.idxA = 1; c5.idxB = 5; c5.restlength = 1.0f;
    c6.idxA = 2; c6.idxB = 6; c6.restlength = 1.0f;
    c7.idxA = 3; c7.idxB = 7; c7.restlength = 1.0f;
    c8.idxA = 4; c8.idxB = 5; c8.restlength = 1.0f;
    c9.idxA = 5; c9.idxB = 6; c9.restlength = 1.0f;
    c10.idxA = 6; c10.idxB = 7; c10.restlength = 1.0f;
    c11.idxA = 7; c11.idxB = 4; c11.restlength = 1.0f;
    constraints.push_back(c0);
    constraints.push_back(c1);
    constraints.push_back(c2);
    constraints.push_back(c3);
    constraints.push_back(c4);
    constraints.push_back(c5);
    constraints.push_back(c6);
    constraints.push_back(c7);
    constraints.push_back(c8);
    constraints.push_back(c9);
    constraints.push_back(c10);
    constraints.push_back(c11);

    // diagonal constraints
    Constraint d0, d1, d2, d3;
    d0.idxA = 0; d0.idxB = 6; d0.restlength = sqrt(3.0f);
    d1.idxA = 1; d1.idxB = 7; d1.restlength = sqrt(3.0f);
    d2.idxA = 2; d2.idxB = 4; d2.restlength = sqrt(3.0f);
    d3.idxA = 3; d3.idxB = 5; d3.restlength = sqrt(3.0f);
    constraints.push_back(d0);
    constraints.push_back(d1);
    constraints.push_back(d2);
    constraints.push_back(d3);

    // side diagonal constraints
    Constraint s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11;
    s0.idxA = 0; s0.idxB = 2; s0.restlength = sqrt(2.0f);
    s1.idxA = 1; s1.idxB = 3; s1.restlength = sqrt(2.0f);
    s2.idxA = 1; s2.idxB = 6; s2.restlength = sqrt(2.0f);
    s3.idxA = 5; s3.idxB = 2; s3.restlength = sqrt(2.0f);
    s4.idxA = 5; s4.idxB = 7; s4.restlength = sqrt(2.0f);
    s5.idxA = 4; s5.idxB = 6; s5.restlength = sqrt(2.0f);
    s6.idxA = 4; s6.idxB = 3; s6.restlength = sqrt(2.0f);
    s7.idxA = 0; s7.idxB = 7; s7.restlength = sqrt(2.0f);
    s8.idxA = 0; s8.idxB = 5; s8.restlength = sqrt(2.0f);
    s9.idxA = 1; s9.idxB = 4; s9.restlength = sqrt(2.0f);
    s10.idxA = 2; s10.idxB = 7; s10.restlength = sqrt(2.0f);
    s11.idxA = 3; s11.idxB = 6; s11.restlength = sqrt(2.0f);
    constraints.push_back(s0);
    constraints.push_back(s1);
    constraints.push_back(s2);
    constraints.push_back(s3);
    constraints.push_back(s4);
    constraints.push_back(s5);
    constraints.push_back(s6);
    constraints.push_back(s7);
    constraints.push_back(s8);
    constraints.push_back(s9);
    constraints.push_back(s10);
    constraints.push_back(s11);

    std::vector<bool> isMovable;
    isMovable.resize(8);
    isMovable[0] = true;
    isMovable[1] = true;
    isMovable[2] = true;
    isMovable[3] = false;
    isMovable[4] = true;
    isMovable[5] = true;
    isMovable[6] = true;
    isMovable[7] = true;

    clothSystem = ClothSimulationSystem(pos, constraints, isMovable);
}


void loadClothPatchExample()
{

    std::vector<Vec3f> pos;
    pos.push_back(Vec3f(-3.0f, 10.0f, 0.0f));
    pos.push_back(Vec3f(-2.0f, 10.0f, 0.0f));
    pos.push_back(Vec3f(-1.0f, 10.0f, 0.0f));
    pos.push_back(Vec3f(0.0f, 10.0f, 0.0f));
    pos.push_back(Vec3f(1.0f, 10.0f, 0.0f));
    pos.push_back(Vec3f(2.0f, 10.0f, 0.0f));
    pos.push_back(Vec3f(3.0f, 10.0f, 0.0f));

    pos.push_back(Vec3f(-3.0f, 9.0f, 0.0f));
    pos.push_back(Vec3f(-2.0f, 9.0f, 0.0f));
    pos.push_back(Vec3f(-1.0f, 9.0f, 0.0f));
    pos.push_back(Vec3f(0.0f, 9.0f, 0.0f));
    pos.push_back(Vec3f(1.0f, 9.0f, 0.0f));
    pos.push_back(Vec3f(2.0f, 9.0f, 0.0f));
    pos.push_back(Vec3f(3.0f, 9.0f, 0.0f));

    pos.push_back(Vec3f(-3.0f, 8.0f, 0.0f));
    pos.push_back(Vec3f(-2.0f, 8.0f, 0.0f));
    pos.push_back(Vec3f(-1.0f, 8.0f, 0.0f));
    pos.push_back(Vec3f(0.0f, 8.0f, 0.0f));
    pos.push_back(Vec3f(1.0f, 8.0f, 0.0f));
    pos.push_back(Vec3f(2.0f, 8.0f, 0.0f));
    pos.push_back(Vec3f(3.0f, 8.0f, 0.0f));
    
    pos.push_back(Vec3f(-3.0f, 7.0f, 0.0f));
    pos.push_back(Vec3f(-2.0f, 7.0f, 0.0f));
    pos.push_back(Vec3f(-1.0f, 7.0f, 0.0f));
    pos.push_back(Vec3f(0.0f, 7.0f, 0.0f));
    pos.push_back(Vec3f(1.0f, 7.0f, 0.0f));
    pos.push_back(Vec3f(2.0f, 7.0f, 0.0f));
    pos.push_back(Vec3f(3.0f, 7.0f, 0.0f));
    
    pos.push_back(Vec3f(-3.0f, 6.0f, 0.0f));
    pos.push_back(Vec3f(-2.0f, 6.0f, 0.0f));
    pos.push_back(Vec3f(-1.0f, 6.0f, 0.0f));
    pos.push_back(Vec3f(0.0f, 6.0f, 0.0f));
    pos.push_back(Vec3f(1.0f, 6.0f, 0.0f));
    pos.push_back(Vec3f(2.0f, 6.0f, 0.0f));
    pos.push_back(Vec3f(3.0f, 6.0f, 0.0f));
    
    pos.push_back(Vec3f(-3.0f, 5.0f, 0.0f));
    pos.push_back(Vec3f(-2.0f, 5.0f, 0.0f));
    pos.push_back(Vec3f(-1.0f, 5.0f, 0.0f));
    pos.push_back(Vec3f(0.0f, 5.0f, 0.0f));
    pos.push_back(Vec3f(1.0f, 5.0f, 0.0f));
    pos.push_back(Vec3f(2.0f, 5.0f, 0.0f));
    pos.push_back(Vec3f(3.0f, 5.0f, 0.0f));
    
    pos.push_back(Vec3f(-3.0f, 4.0f, 0.0f));
    pos.push_back(Vec3f(-2.0f, 4.0f, 0.0f));
    pos.push_back(Vec3f(-1.0f, 4.0f, 0.0f));
    pos.push_back(Vec3f(0.0f, 4.0f, 0.0f));
    pos.push_back(Vec3f(1.0f, 4.0f, 0.0f));
    pos.push_back(Vec3f(2.0f, 4.0f, 0.0f));
    pos.push_back(Vec3f(3.0f, 4.0f, 0.0f));

    std::vector<Constraint> constraints;

    // horizontal contraints
    Constraint h0, h1, h2, h3, h4, h5, h6, h7, h8, h9,
                h10, h11, h12, h13, h14, h15, h16, h17, h18, h19,
                h20, h21, h22, h23, h24, h25, h26, h27, h28, h29,
                h30, h31, h32, h33, h34, h35, h36, h37, h38, h39,
                h40, h41;

    h0.idxA = 0;   h0.idxB = 1;   h0.restlength = 1.0f;     constraints.push_back(h0);
    h1.idxA = 1;   h1.idxB = 2;   h1.restlength = 1.0f;     constraints.push_back(h1);
    h2.idxA = 2;   h2.idxB = 3;   h2.restlength = 1.0f;     constraints.push_back(h2);
    h3.idxA = 3;   h3.idxB = 4;   h3.restlength = 1.0f;     constraints.push_back(h3);
    h4.idxA = 4;   h4.idxB = 5;   h4.restlength = 1.0f;     constraints.push_back(h4);
    h5.idxA = 5;   h5.idxB = 6;   h5.restlength = 1.0f;     constraints.push_back(h5);

    h6.idxA = 7;   h6.idxB = 8;   h6.restlength = 1.0f;     constraints.push_back(h6);
    h7.idxA = 8;   h7.idxB = 9;   h7.restlength = 1.0f;     constraints.push_back(h7);
    h8.idxA = 9;   h8.idxB = 10;  h8.restlength = 1.0f;     constraints.push_back(h8);
    h9.idxA = 10;  h9.idxB = 11;  h9.restlength = 1.0f;     constraints.push_back(h9);
    h10.idxA = 11; h10.idxB = 12; h10.restlength = 1.0f;    constraints.push_back(h10);
    h11.idxA = 12; h11.idxB = 13; h11.restlength = 1.0f;    constraints.push_back(h11);

    h12.idxA = 14; h12.idxB = 15; h12.restlength = 1.0f;    constraints.push_back(h12);
    h13.idxA = 15; h13.idxB = 16; h13.restlength = 1.0f;    constraints.push_back(h13);
    h14.idxA = 16; h14.idxB = 17; h14.restlength = 1.0f;    constraints.push_back(h14);
    h15.idxA = 17; h15.idxB = 18; h15.restlength = 1.0f;    constraints.push_back(h15);
    h16.idxA = 18; h16.idxB = 19; h16.restlength = 1.0f;    constraints.push_back(h16);
    h17.idxA = 19; h17.idxB = 20; h17.restlength = 1.0f;    constraints.push_back(h17);

    h18.idxA = 21; h18.idxB = 22; h18.restlength = 1.0f;    constraints.push_back(h18);
    h19.idxA = 22; h19.idxB = 23; h19.restlength = 1.0f;    constraints.push_back(h19);
    h20.idxA = 23; h20.idxB = 24; h20.restlength = 1.0f;    constraints.push_back(h20);
    h21.idxA = 24; h21.idxB = 25; h21.restlength = 1.0f;    constraints.push_back(h21);
    h22.idxA = 25; h22.idxB = 26; h22.restlength = 1.0f;    constraints.push_back(h22);
    h23.idxA = 26; h23.idxB = 27; h23.restlength = 1.0f;    constraints.push_back(h23);

    h24.idxA = 28; h24.idxB = 29; h24.restlength = 1.0f;    constraints.push_back(h24);
    h25.idxA = 29; h25.idxB = 30; h25.restlength = 1.0f;    constraints.push_back(h25);
    h26.idxA = 30; h26.idxB = 31; h26.restlength = 1.0f;    constraints.push_back(h26);
    h27.idxA = 31; h27.idxB = 32; h27.restlength = 1.0f;    constraints.push_back(h27);
    h28.idxA = 32; h28.idxB = 33; h28.restlength = 1.0f;    constraints.push_back(h28);
    h29.idxA = 33; h29.idxB = 34; h29.restlength = 1.0f;    constraints.push_back(h29);

    h30.idxA = 35; h30.idxB = 36; h30.restlength = 1.0f;    constraints.push_back(h30);
    h31.idxA = 36; h31.idxB = 37; h31.restlength = 1.0f;    constraints.push_back(h31);
    h32.idxA = 37; h32.idxB = 38; h32.restlength = 1.0f;    constraints.push_back(h32);
    h33.idxA = 38; h33.idxB = 39; h33.restlength = 1.0f;    constraints.push_back(h33);
    h34.idxA = 39; h34.idxB = 40; h34.restlength = 1.0f;    constraints.push_back(h34);
    h35.idxA = 40; h35.idxB = 41; h35.restlength = 1.0f;    constraints.push_back(h35);

    h36.idxA = 42; h36.idxB = 43; h36.restlength = 1.0f;    constraints.push_back(h36);
    h37.idxA = 43; h37.idxB = 44; h37.restlength = 1.0f;    constraints.push_back(h37);
    h38.idxA = 44; h38.idxB = 45; h38.restlength = 1.0f;    constraints.push_back(h38);
    h39.idxA = 45; h39.idxB = 46; h39.restlength = 1.0f;    constraints.push_back(h39);
    h40.idxA = 46; h40.idxB = 47; h40.restlength = 1.0f;    constraints.push_back(h40);
    h41.idxA = 47; h41.idxB = 48; h41.restlength = 1.0f;    constraints.push_back(h41);


    // vertical contraints
    Constraint v0, v1, v2, v3, v4, v5, v6, v7, v8, v9,
                v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,
                v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,
                v30, v31, v32, v33, v34, v35, v36, v37, v38, v39,
                v40, v41;

    v0.idxA = 0;   v0.idxB = 7;   v0.restlength = 1.0f;     constraints.push_back(v0);
    v1.idxA = 1;   v1.idxB = 8;   v1.restlength = 1.0f;     constraints.push_back(v1);
    v2.idxA = 2;   v2.idxB = 9;   v2.restlength = 1.0f;     constraints.push_back(v2);
    v3.idxA = 3;   v3.idxB = 10;  v3.restlength = 1.0f;     constraints.push_back(v3);
    v4.idxA = 4;   v4.idxB = 11;  v4.restlength = 1.0f;     constraints.push_back(v4);
    v5.idxA = 5;   v5.idxB = 12;  v5.restlength = 1.0f;     constraints.push_back(v5);
    v6.idxA = 6;   v6.idxB = 13;  v6.restlength = 1.0f;     constraints.push_back(v6);

    v7.idxA = 7;   v7.idxB = 14;  v7.restlength = 1.0f;     constraints.push_back(v7);
    v8.idxA = 8;   v8.idxB = 15;  v8.restlength = 1.0f;     constraints.push_back(v8);
    v9.idxA = 9;   v9.idxB = 16;  v9.restlength = 1.0f;     constraints.push_back(v9);
    v10.idxA = 10; v10.idxB = 17; v10.restlength = 1.0f;    constraints.push_back(v10);
    v11.idxA = 11; v11.idxB = 18; v11.restlength = 1.0f;    constraints.push_back(v11);
    v12.idxA = 12; v12.idxB = 19; v12.restlength = 1.0f;    constraints.push_back(v12);
    v13.idxA = 13; v13.idxB = 20; v13.restlength = 1.0f;    constraints.push_back(v13);

    v14.idxA = 14; v14.idxB = 21; v14.restlength = 1.0f;    constraints.push_back(v14);
    v15.idxA = 15; v15.idxB = 22; v15.restlength = 1.0f;    constraints.push_back(v15);
    v16.idxA = 16; v16.idxB = 23; v16.restlength = 1.0f;    constraints.push_back(v16);
    v17.idxA = 17; v17.idxB = 24; v17.restlength = 1.0f;    constraints.push_back(v17);
    v18.idxA = 18; v18.idxB = 25; v18.restlength = 1.0f;    constraints.push_back(v18);
    v19.idxA = 19; v19.idxB = 26; v19.restlength = 1.0f;    constraints.push_back(v19);
    v20.idxA = 20; v20.idxB = 27; v20.restlength = 1.0f;    constraints.push_back(v20);

    v21.idxA = 21; v21.idxB = 28; v21.restlength = 1.0f;    constraints.push_back(v21);
    v22.idxA = 22; v22.idxB = 29; v22.restlength = 1.0f;    constraints.push_back(v22);
    v23.idxA = 23; v23.idxB = 30; v23.restlength = 1.0f;    constraints.push_back(v23);
    v24.idxA = 24; v24.idxB = 31; v24.restlength = 1.0f;    constraints.push_back(v24);
    v25.idxA = 25; v25.idxB = 32; v25.restlength = 1.0f;    constraints.push_back(v25);
    v26.idxA = 26; v26.idxB = 33; v26.restlength = 1.0f;    constraints.push_back(v26);
    v27.idxA = 27; v27.idxB = 34; v27.restlength = 1.0f;    constraints.push_back(v27);

    v28.idxA = 28; v28.idxB = 35; v28.restlength = 1.0f;    constraints.push_back(v28);
    v29.idxA = 29; v29.idxB = 36; v29.restlength = 1.0f;    constraints.push_back(v29);
    v30.idxA = 30; v30.idxB = 37; v30.restlength = 1.0f;    constraints.push_back(v30);
    v31.idxA = 31; v31.idxB = 38; v31.restlength = 1.0f;    constraints.push_back(v31);
    v32.idxA = 32; v32.idxB = 39; v32.restlength = 1.0f;    constraints.push_back(v32);
    v33.idxA = 33; v33.idxB = 40; v33.restlength = 1.0f;    constraints.push_back(v33);
    v34.idxA = 34; v34.idxB = 41; v34.restlength = 1.0f;    constraints.push_back(v34);

    v35.idxA = 35; v35.idxB = 42; v35.restlength = 1.0f;    constraints.push_back(v35);
    v36.idxA = 36; v36.idxB = 43; v36.restlength = 1.0f;    constraints.push_back(v36);
    v37.idxA = 37; v37.idxB = 44; v37.restlength = 1.0f;    constraints.push_back(v37);
    v38.idxA = 38; v38.idxB = 45; v38.restlength = 1.0f;    constraints.push_back(v38);
    v39.idxA = 39; v39.idxB = 46; v39.restlength = 1.0f;    constraints.push_back(v39);
    v40.idxA = 40; v40.idxB = 47; v40.restlength = 1.0f;    constraints.push_back(v40);
    v41.idxA = 41; v41.idxB = 48; v41.restlength = 1.0f;    constraints.push_back(v41);


    std::vector<bool> isMovable;
    isMovable.resize(49);
    isMovable[0] = false;
    isMovable[1] = true;
    isMovable[2] = true;
    isMovable[3] = true;
    isMovable[4] = true;
    isMovable[5] = true;
    isMovable[6] = true;

    isMovable[7] = false;
    isMovable[8] = true;
    isMovable[9] = true;
    isMovable[10] = true;
    isMovable[11] = true;
    isMovable[12] = true;
    isMovable[13] = true;
    
    isMovable[14] = false;
    isMovable[15] = true;
    isMovable[16] = true;
    isMovable[17] = true;
    isMovable[18] = true;
    isMovable[19] = true;
    isMovable[20] = true;
    
    isMovable[21] = false;
    isMovable[22] = true;
    isMovable[23] = true;
    isMovable[24] = true;
    isMovable[25] = true;
    isMovable[26] = true;
    isMovable[27] = true;
    
    isMovable[28] = false;
    isMovable[29] = true;
    isMovable[30] = true;
    isMovable[31] = true;
    isMovable[32] = true;
    isMovable[33] = true;
    isMovable[34] = true;
    
    isMovable[35] = false;
    isMovable[36] = true;
    isMovable[37] = true;
    isMovable[38] = true;
    isMovable[39] = true;
    isMovable[40] = true;
    isMovable[41] = true;
    
    isMovable[42] = false;
    isMovable[43] = true;
    isMovable[44] = true;
    isMovable[45] = true;
    isMovable[46] = true;
    isMovable[47] = true;
    isMovable[48] = true;

    clothSystem = ClothSimulationSystem(pos, constraints, isMovable);
}

void loadStrongClothPatchExample()
{

    std::vector<Vec3f> pos;
    pos.push_back(Vec3f(-3.0f, 10.0f, 0.0f));
    pos.push_back(Vec3f(-2.0f, 10.0f, 0.0f));
    pos.push_back(Vec3f(-1.0f, 10.0f, 0.0f));
    pos.push_back(Vec3f(0.0f, 10.0f, 0.0f));
    pos.push_back(Vec3f(1.0f, 10.0f, 0.0f));
    pos.push_back(Vec3f(2.0f, 10.0f, 0.0f));
    pos.push_back(Vec3f(3.0f, 10.0f, 0.0f));

    pos.push_back(Vec3f(-3.0f, 9.0f, 0.0f));
    pos.push_back(Vec3f(-2.0f, 9.0f, 0.0f));
    pos.push_back(Vec3f(-1.0f, 9.0f, 0.0f));
    pos.push_back(Vec3f(0.0f, 9.0f, 0.0f));
    pos.push_back(Vec3f(1.0f, 9.0f, 0.0f));
    pos.push_back(Vec3f(2.0f, 9.0f, 0.0f));
    pos.push_back(Vec3f(3.0f, 9.0f, 0.0f));

    pos.push_back(Vec3f(-3.0f, 8.0f, 0.0f));
    pos.push_back(Vec3f(-2.0f, 8.0f, 0.0f));
    pos.push_back(Vec3f(-1.0f, 8.0f, 0.0f));
    pos.push_back(Vec3f(0.0f, 8.0f, 0.0f));
    pos.push_back(Vec3f(1.0f, 8.0f, 0.0f));
    pos.push_back(Vec3f(2.0f, 8.0f, 0.0f));
    pos.push_back(Vec3f(3.0f, 8.0f, 0.0f));
    
    pos.push_back(Vec3f(-3.0f, 7.0f, 0.0f));
    pos.push_back(Vec3f(-2.0f, 7.0f, 0.0f));
    pos.push_back(Vec3f(-1.0f, 7.0f, 0.0f));
    pos.push_back(Vec3f(0.0f, 7.0f, 0.0f));
    pos.push_back(Vec3f(1.0f, 7.0f, 0.0f));
    pos.push_back(Vec3f(2.0f, 7.0f, 0.0f));
    pos.push_back(Vec3f(3.0f, 7.0f, 0.0f));
    
    pos.push_back(Vec3f(-3.0f, 6.0f, 0.0f));
    pos.push_back(Vec3f(-2.0f, 6.0f, 0.0f));
    pos.push_back(Vec3f(-1.0f, 6.0f, 0.0f));
    pos.push_back(Vec3f(0.0f, 6.0f, 0.0f));
    pos.push_back(Vec3f(1.0f, 6.0f, 0.0f));
    pos.push_back(Vec3f(2.0f, 6.0f, 0.0f));
    pos.push_back(Vec3f(3.0f, 6.0f, 0.0f));
    
    pos.push_back(Vec3f(-3.0f, 5.0f, 0.0f));
    pos.push_back(Vec3f(-2.0f, 5.0f, 0.0f));
    pos.push_back(Vec3f(-1.0f, 5.0f, 0.0f));
    pos.push_back(Vec3f(0.0f, 5.0f, 0.0f));
    pos.push_back(Vec3f(1.0f, 5.0f, 0.0f));
    pos.push_back(Vec3f(2.0f, 5.0f, 0.0f));
    pos.push_back(Vec3f(3.0f, 5.0f, 0.0f));
    
    pos.push_back(Vec3f(-3.0f, 4.0f, 0.0f));
    pos.push_back(Vec3f(-2.0f, 4.0f, 0.0f));
    pos.push_back(Vec3f(-1.0f, 4.0f, 0.0f));
    pos.push_back(Vec3f(0.0f, 4.0f, 0.0f));
    pos.push_back(Vec3f(1.0f, 4.0f, 0.0f));
    pos.push_back(Vec3f(2.0f, 4.0f, 0.0f));
    pos.push_back(Vec3f(3.0f, 4.0f, 0.0f));

    std::vector<Constraint> constraints;

    // horizontal contraints
    Constraint h0, h1, h2, h3, h4, h5, h6, h7, h8, h9,
                h10, h11, h12, h13, h14, h15, h16, h17, h18, h19,
                h20, h21, h22, h23, h24, h25, h26, h27, h28, h29,
                h30, h31, h32, h33, h34, h35, h36, h37, h38, h39,
                h40, h41;

    h0.idxA = 0;   h0.idxB = 1;   h0.restlength = 1.0f;     constraints.push_back(h0);
    h1.idxA = 1;   h1.idxB = 2;   h1.restlength = 1.0f;     constraints.push_back(h1);
    h2.idxA = 2;   h2.idxB = 3;   h2.restlength = 1.0f;     constraints.push_back(h2);
    h3.idxA = 3;   h3.idxB = 4;   h3.restlength = 1.0f;     constraints.push_back(h3);
    h4.idxA = 4;   h4.idxB = 5;   h4.restlength = 1.0f;     constraints.push_back(h4);
    h5.idxA = 5;   h5.idxB = 6;   h5.restlength = 1.0f;     constraints.push_back(h5);

    h6.idxA = 7;   h6.idxB = 8;   h6.restlength = 1.0f;     constraints.push_back(h6);
    h7.idxA = 8;   h7.idxB = 9;   h7.restlength = 1.0f;     constraints.push_back(h7);
    h8.idxA = 9;   h8.idxB = 10;  h8.restlength = 1.0f;     constraints.push_back(h8);
    h9.idxA = 10;  h9.idxB = 11;  h9.restlength = 1.0f;     constraints.push_back(h9);
    h10.idxA = 11; h10.idxB = 12; h10.restlength = 1.0f;    constraints.push_back(h10);
    h11.idxA = 12; h11.idxB = 13; h11.restlength = 1.0f;    constraints.push_back(h11);

    h12.idxA = 14; h12.idxB = 15; h12.restlength = 1.0f;    constraints.push_back(h12);
    h13.idxA = 15; h13.idxB = 16; h13.restlength = 1.0f;    constraints.push_back(h13);
    h14.idxA = 16; h14.idxB = 17; h14.restlength = 1.0f;    constraints.push_back(h14);
    h15.idxA = 17; h15.idxB = 18; h15.restlength = 1.0f;    constraints.push_back(h15);
    h16.idxA = 18; h16.idxB = 19; h16.restlength = 1.0f;    constraints.push_back(h16);
    h17.idxA = 19; h17.idxB = 20; h17.restlength = 1.0f;    constraints.push_back(h17);

    h18.idxA = 21; h18.idxB = 22; h18.restlength = 1.0f;    constraints.push_back(h18);
    h19.idxA = 22; h19.idxB = 23; h19.restlength = 1.0f;    constraints.push_back(h19);
    h20.idxA = 23; h20.idxB = 24; h20.restlength = 1.0f;    constraints.push_back(h20);
    h21.idxA = 24; h21.idxB = 25; h21.restlength = 1.0f;    constraints.push_back(h21);
    h22.idxA = 25; h22.idxB = 26; h22.restlength = 1.0f;    constraints.push_back(h22);
    h23.idxA = 26; h23.idxB = 27; h23.restlength = 1.0f;    constraints.push_back(h23);

    h24.idxA = 28; h24.idxB = 29; h24.restlength = 1.0f;    constraints.push_back(h24);
    h25.idxA = 29; h25.idxB = 30; h25.restlength = 1.0f;    constraints.push_back(h25);
    h26.idxA = 30; h26.idxB = 31; h26.restlength = 1.0f;    constraints.push_back(h26);
    h27.idxA = 31; h27.idxB = 32; h27.restlength = 1.0f;    constraints.push_back(h27);
    h28.idxA = 32; h28.idxB = 33; h28.restlength = 1.0f;    constraints.push_back(h28);
    h29.idxA = 33; h29.idxB = 34; h29.restlength = 1.0f;    constraints.push_back(h29);

    h30.idxA = 35; h30.idxB = 36; h30.restlength = 1.0f;    constraints.push_back(h30);
    h31.idxA = 36; h31.idxB = 37; h31.restlength = 1.0f;    constraints.push_back(h31);
    h32.idxA = 37; h32.idxB = 38; h32.restlength = 1.0f;    constraints.push_back(h32);
    h33.idxA = 38; h33.idxB = 39; h33.restlength = 1.0f;    constraints.push_back(h33);
    h34.idxA = 39; h34.idxB = 40; h34.restlength = 1.0f;    constraints.push_back(h34);
    h35.idxA = 40; h35.idxB = 41; h35.restlength = 1.0f;    constraints.push_back(h35);

    h36.idxA = 42; h36.idxB = 43; h36.restlength = 1.0f;    constraints.push_back(h36);
    h37.idxA = 43; h37.idxB = 44; h37.restlength = 1.0f;    constraints.push_back(h37);
    h38.idxA = 44; h38.idxB = 45; h38.restlength = 1.0f;    constraints.push_back(h38);
    h39.idxA = 45; h39.idxB = 46; h39.restlength = 1.0f;    constraints.push_back(h39);
    h40.idxA = 46; h40.idxB = 47; h40.restlength = 1.0f;    constraints.push_back(h40);
    h41.idxA = 47; h41.idxB = 48; h41.restlength = 1.0f;    constraints.push_back(h41);


    // vertical contraints
    Constraint v0, v1, v2, v3, v4, v5, v6, v7, v8, v9,
                v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,
                v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,
                v30, v31, v32, v33, v34, v35, v36, v37, v38, v39,
                v40, v41;

    v0.idxA = 0;   v0.idxB = 7;   v0.restlength = 1.0f;     constraints.push_back(v0);
    v1.idxA = 1;   v1.idxB = 8;   v1.restlength = 1.0f;     constraints.push_back(v1);
    v2.idxA = 2;   v2.idxB = 9;   v2.restlength = 1.0f;     constraints.push_back(v2);
    v3.idxA = 3;   v3.idxB = 10;  v3.restlength = 1.0f;     constraints.push_back(v3);
    v4.idxA = 4;   v4.idxB = 11;  v4.restlength = 1.0f;     constraints.push_back(v4);
    v5.idxA = 5;   v5.idxB = 12;  v5.restlength = 1.0f;     constraints.push_back(v5);
    v6.idxA = 6;   v6.idxB = 13;  v6.restlength = 1.0f;     constraints.push_back(v6);

    v7.idxA = 7;   v7.idxB = 14;  v7.restlength = 1.0f;     constraints.push_back(v7);
    v8.idxA = 8;   v8.idxB = 15;  v8.restlength = 1.0f;     constraints.push_back(v8);
    v9.idxA = 9;   v9.idxB = 16;  v9.restlength = 1.0f;     constraints.push_back(v9);
    v10.idxA = 10; v10.idxB = 17; v10.restlength = 1.0f;    constraints.push_back(v10);
    v11.idxA = 11; v11.idxB = 18; v11.restlength = 1.0f;    constraints.push_back(v11);
    v12.idxA = 12; v12.idxB = 19; v12.restlength = 1.0f;    constraints.push_back(v12);
    v13.idxA = 13; v13.idxB = 20; v13.restlength = 1.0f;    constraints.push_back(v13);

    v14.idxA = 14; v14.idxB = 21; v14.restlength = 1.0f;    constraints.push_back(v14);
    v15.idxA = 15; v15.idxB = 22; v15.restlength = 1.0f;    constraints.push_back(v15);
    v16.idxA = 16; v16.idxB = 23; v16.restlength = 1.0f;    constraints.push_back(v16);
    v17.idxA = 17; v17.idxB = 24; v17.restlength = 1.0f;    constraints.push_back(v17);
    v18.idxA = 18; v18.idxB = 25; v18.restlength = 1.0f;    constraints.push_back(v18);
    v19.idxA = 19; v19.idxB = 26; v19.restlength = 1.0f;    constraints.push_back(v19);
    v20.idxA = 20; v20.idxB = 27; v20.restlength = 1.0f;    constraints.push_back(v20);

    v21.idxA = 21; v21.idxB = 28; v21.restlength = 1.0f;    constraints.push_back(v21);
    v22.idxA = 22; v22.idxB = 29; v22.restlength = 1.0f;    constraints.push_back(v22);
    v23.idxA = 23; v23.idxB = 30; v23.restlength = 1.0f;    constraints.push_back(v23);
    v24.idxA = 24; v24.idxB = 31; v24.restlength = 1.0f;    constraints.push_back(v24);
    v25.idxA = 25; v25.idxB = 32; v25.restlength = 1.0f;    constraints.push_back(v25);
    v26.idxA = 26; v26.idxB = 33; v26.restlength = 1.0f;    constraints.push_back(v26);
    v27.idxA = 27; v27.idxB = 34; v27.restlength = 1.0f;    constraints.push_back(v27);

    v28.idxA = 28; v28.idxB = 35; v28.restlength = 1.0f;    constraints.push_back(v28);
    v29.idxA = 29; v29.idxB = 36; v29.restlength = 1.0f;    constraints.push_back(v29);
    v30.idxA = 30; v30.idxB = 37; v30.restlength = 1.0f;    constraints.push_back(v30);
    v31.idxA = 31; v31.idxB = 38; v31.restlength = 1.0f;    constraints.push_back(v31);
    v32.idxA = 32; v32.idxB = 39; v32.restlength = 1.0f;    constraints.push_back(v32);
    v33.idxA = 33; v33.idxB = 40; v33.restlength = 1.0f;    constraints.push_back(v33);
    v34.idxA = 34; v34.idxB = 41; v34.restlength = 1.0f;    constraints.push_back(v34);

    v35.idxA = 35; v35.idxB = 42; v35.restlength = 1.0f;    constraints.push_back(v35);
    v36.idxA = 36; v36.idxB = 43; v36.restlength = 1.0f;    constraints.push_back(v36);
    v37.idxA = 37; v37.idxB = 44; v37.restlength = 1.0f;    constraints.push_back(v37);
    v38.idxA = 38; v38.idxB = 45; v38.restlength = 1.0f;    constraints.push_back(v38);
    v39.idxA = 39; v39.idxB = 46; v39.restlength = 1.0f;    constraints.push_back(v39);
    v40.idxA = 40; v40.idxB = 47; v40.restlength = 1.0f;    constraints.push_back(v40);
    v41.idxA = 41; v41.idxB = 48; v41.restlength = 1.0f;    constraints.push_back(v41);


    // diagonal foward contraints
    Constraint f0, f1, f2, f3, f4, f5, f6, f7, f8, f9,
                f10, f11, f12, f13, f14, f15, f16, f17, f18, f19,
                f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
                f30, f31, f32, f33, f34, f35;

    f0.idxA = 0;   f0.idxB = 8;   f0.restlength = sqrt(2.0f);     constraints.push_back(f0);
    f1.idxA = 1;   f1.idxB = 9;   f1.restlength = sqrt(2.0f);     constraints.push_back(f1);
    f2.idxA = 2;   f2.idxB = 10;  f2.restlength = sqrt(2.0f);     constraints.push_back(f2);
    f3.idxA = 3;   f3.idxB = 11;  f3.restlength = sqrt(2.0f);     constraints.push_back(f3);
    f4.idxA = 4;   f4.idxB = 12;  f4.restlength = sqrt(2.0f);     constraints.push_back(f4);
    f5.idxA = 5;   f5.idxB = 13;  f5.restlength = sqrt(2.0f);     constraints.push_back(f5);
    
    f6.idxA = 7;   f6.idxB = 15;  f6.restlength = sqrt(2.0f);     constraints.push_back(f6);
    f7.idxA = 8;   f7.idxB = 16;  f7.restlength = sqrt(2.0f);     constraints.push_back(f7);
    f8.idxA = 9;   f8.idxB = 17;  f8.restlength = sqrt(2.0f);     constraints.push_back(f8);
    f9.idxA = 10;  f9.idxB = 18;  f9.restlength = sqrt(2.0f);     constraints.push_back(f9);
    f10.idxA = 11; f10.idxB = 19; f10.restlength = sqrt(2.0f);    constraints.push_back(f10);
    f11.idxA = 12; f11.idxB = 20; f11.restlength = sqrt(2.0f);    constraints.push_back(f11);

    f12.idxA = 14; f12.idxB = 22; f12.restlength = sqrt(2.0f);    constraints.push_back(f12);
    f13.idxA = 15; f13.idxB = 23; f13.restlength = sqrt(2.0f);    constraints.push_back(f13);
    f14.idxA = 16; f14.idxB = 24; f14.restlength = sqrt(2.0f);    constraints.push_back(f14);
    f15.idxA = 17; f15.idxB = 25; f15.restlength = sqrt(2.0f);    constraints.push_back(f15);
    f16.idxA = 18; f16.idxB = 26; f16.restlength = sqrt(2.0f);    constraints.push_back(f16);
    f17.idxA = 19; f17.idxB = 27; f17.restlength = sqrt(2.0f);    constraints.push_back(f17);

    f18.idxA = 21; f18.idxB = 29; f18.restlength = sqrt(2.0f);    constraints.push_back(f18);
    f19.idxA = 22; f19.idxB = 30; f19.restlength = sqrt(2.0f);    constraints.push_back(f19);
    f20.idxA = 23; f20.idxB = 31; f20.restlength = sqrt(2.0f);    constraints.push_back(f20);
    f21.idxA = 24; f21.idxB = 32; f21.restlength = sqrt(2.0f);    constraints.push_back(f21);
    f22.idxA = 25; f22.idxB = 33; f22.restlength = sqrt(2.0f);    constraints.push_back(f22);
    f23.idxA = 26; f23.idxB = 34; f23.restlength = sqrt(2.0f);    constraints.push_back(f23);

    f24.idxA = 28; f24.idxB = 36; f24.restlength = sqrt(2.0f);    constraints.push_back(f24);
    f25.idxA = 29; f25.idxB = 37; f25.restlength = sqrt(2.0f);    constraints.push_back(f25);
    f26.idxA = 30; f26.idxB = 38; f26.restlength = sqrt(2.0f);    constraints.push_back(f26);
    f27.idxA = 31; f27.idxB = 39; f27.restlength = sqrt(2.0f);    constraints.push_back(f27);
    f28.idxA = 32; f28.idxB = 40; f28.restlength = sqrt(2.0f);    constraints.push_back(f28);
    f29.idxA = 33; f29.idxB = 41; f29.restlength = sqrt(2.0f);    constraints.push_back(f29);

    f30.idxA = 35; f30.idxB = 43; f30.restlength = sqrt(2.0f);    constraints.push_back(f30);
    f31.idxA = 36; f31.idxB = 44; f31.restlength = sqrt(2.0f);    constraints.push_back(f31);
    f32.idxA = 37; f32.idxB = 45; f32.restlength = sqrt(2.0f);    constraints.push_back(f32);
    f33.idxA = 38; f33.idxB = 46; f33.restlength = sqrt(2.0f);    constraints.push_back(f33);
    f34.idxA = 39; f34.idxB = 47; f34.restlength = sqrt(2.0f);    constraints.push_back(f34);
    f35.idxA = 40; f35.idxB = 48; f35.restlength = sqrt(2.0f);    constraints.push_back(f35);

    // diagonal backward contraints
    Constraint b0, b1, b2, b3, b4, b5, b6, b7, b8, b9,
                b10, b11, b12, b13, b14, b15, b16, b17, b18, b19,
                b20, b21, b22, b23, b24, b25, b26, b27, b28, b29,
                b30, b31, b32, b33, b34, b35;

    b0.idxA = 1;   b0.idxB = 7;   b0.restlength = sqrt(2.0f);     constraints.push_back(b0);
    b1.idxA = 2;   b1.idxB = 8;   b1.restlength = sqrt(2.0f);     constraints.push_back(b1);
    b2.idxA = 3;   b2.idxB = 9;   b2.restlength = sqrt(2.0f);     constraints.push_back(b2);
    b3.idxA = 4;   b3.idxB = 10;  b3.restlength = sqrt(2.0f);     constraints.push_back(b3);
    b4.idxA = 5;   b4.idxB = 11;  b4.restlength = sqrt(2.0f);     constraints.push_back(b4);
    b5.idxA = 6;   b5.idxB = 12;  b5.restlength = sqrt(2.0f);     constraints.push_back(b5);
    
    b6.idxA = 8;   b6.idxB = 14;  b6.restlength = sqrt(2.0f);     constraints.push_back(b6);
    b7.idxA = 9;   b7.idxB = 15;  b7.restlength = sqrt(2.0f);     constraints.push_back(b7);
    b8.idxA = 10;  b8.idxB = 16;  b8.restlength = sqrt(2.0f);     constraints.push_back(b8);
    b9.idxA = 11;  b9.idxB = 17;  b9.restlength = sqrt(2.0f);     constraints.push_back(b9);
    b10.idxA = 12; b10.idxB = 18; b10.restlength = sqrt(2.0f);    constraints.push_back(b10);
    b11.idxA = 13; b11.idxB = 19; b11.restlength = sqrt(2.0f);    constraints.push_back(b11);

    b12.idxA = 15; b12.idxB = 21; b12.restlength = sqrt(2.0f);    constraints.push_back(b12);
    b13.idxA = 16; b13.idxB = 22; b13.restlength = sqrt(2.0f);    constraints.push_back(b13);
    b14.idxA = 17; b14.idxB = 23; b14.restlength = sqrt(2.0f);    constraints.push_back(b14);
    b15.idxA = 18; b15.idxB = 24; b15.restlength = sqrt(2.0f);    constraints.push_back(b15);
    b16.idxA = 19; b16.idxB = 25; b16.restlength = sqrt(2.0f);    constraints.push_back(b16);
    b17.idxA = 20; b17.idxB = 26; b17.restlength = sqrt(2.0f);    constraints.push_back(b17);

    b18.idxA = 22; b18.idxB = 28; b18.restlength = sqrt(2.0f);    constraints.push_back(b18);
    b19.idxA = 23; b19.idxB = 29; b19.restlength = sqrt(2.0f);    constraints.push_back(b19);
    b20.idxA = 24; b20.idxB = 30; b20.restlength = sqrt(2.0f);    constraints.push_back(b20);
    b21.idxA = 25; b21.idxB = 31; b21.restlength = sqrt(2.0f);    constraints.push_back(b21);
    b22.idxA = 26; b22.idxB = 32; b22.restlength = sqrt(2.0f);    constraints.push_back(b22);
    b23.idxA = 27; b23.idxB = 33; b23.restlength = sqrt(2.0f);    constraints.push_back(b23);

    b24.idxA = 29; b24.idxB = 35; b24.restlength = sqrt(2.0f);    constraints.push_back(b24);
    b25.idxA = 30; b25.idxB = 36; b25.restlength = sqrt(2.0f);    constraints.push_back(b25);
    b26.idxA = 31; b26.idxB = 37; b26.restlength = sqrt(2.0f);    constraints.push_back(b26);
    b27.idxA = 32; b27.idxB = 38; b27.restlength = sqrt(2.0f);    constraints.push_back(b27);
    b28.idxA = 33; b28.idxB = 39; b28.restlength = sqrt(2.0f);    constraints.push_back(b28);
    b29.idxA = 34; b29.idxB = 40; b29.restlength = sqrt(2.0f);    constraints.push_back(b29);

    b30.idxA = 36; b30.idxB = 42; b30.restlength = sqrt(2.0f);    constraints.push_back(b30);
    b31.idxA = 37; b31.idxB = 43; b31.restlength = sqrt(2.0f);    constraints.push_back(b31);
    b32.idxA = 38; b32.idxB = 44; b32.restlength = sqrt(2.0f);    constraints.push_back(b32);
    b33.idxA = 39; b33.idxB = 45; b33.restlength = sqrt(2.0f);    constraints.push_back(b33);
    b34.idxA = 40; b34.idxB = 46; b34.restlength = sqrt(2.0f);    constraints.push_back(b34);
    b35.idxA = 41; b35.idxB = 47; b35.restlength = sqrt(2.0f);    constraints.push_back(b35);


    std::vector<bool> isMovable;
    isMovable.resize(49);
    isMovable[0] = false;
    isMovable[1] = true;
    isMovable[2] = true;
    isMovable[3] = true;
    isMovable[4] = true;
    isMovable[5] = true;
    isMovable[6] = true;

    isMovable[7] = false;
    isMovable[8] = true;
    isMovable[9] = true;
    isMovable[10] = true;
    isMovable[11] = true;
    isMovable[12] = true;
    isMovable[13] = true;
    
    isMovable[14] = false;
    isMovable[15] = true;
    isMovable[16] = true;
    isMovable[17] = true;
    isMovable[18] = true;
    isMovable[19] = true;
    isMovable[20] = true;
    
    isMovable[21] = false;
    isMovable[22] = true;
    isMovable[23] = true;
    isMovable[24] = true;
    isMovable[25] = true;
    isMovable[26] = true;
    isMovable[27] = true;
    
    isMovable[28] = false;
    isMovable[29] = true;
    isMovable[30] = true;
    isMovable[31] = true;
    isMovable[32] = true;
    isMovable[33] = true;
    isMovable[34] = true;
    
    isMovable[35] = false;
    isMovable[36] = true;
    isMovable[37] = true;
    isMovable[38] = true;
    isMovable[39] = true;
    isMovable[40] = true;
    isMovable[41] = true;
    
    isMovable[42] = false;
    isMovable[43] = true;
    isMovable[44] = true;
    isMovable[45] = true;
    isMovable[46] = true;
    isMovable[47] = true;
    isMovable[48] = true;

    clothSystem = ClothSimulationSystem(pos, constraints, isMovable);
}

void loadExtraStrongClothPatchExample()
{

    std::vector<Vec3f> pos;
    pos.push_back(Vec3f(-3.0f, 10.0f, 0.0f));
    pos.push_back(Vec3f(-2.0f, 10.0f, 0.0f));
    pos.push_back(Vec3f(-1.0f, 10.0f, 0.0f));
    pos.push_back(Vec3f(0.0f, 10.0f, 0.0f));
    pos.push_back(Vec3f(1.0f, 10.0f, 0.0f));
    pos.push_back(Vec3f(2.0f, 10.0f, 0.0f));
    pos.push_back(Vec3f(3.0f, 10.0f, 0.0f));

    pos.push_back(Vec3f(-3.0f, 9.0f, 0.0f));
    pos.push_back(Vec3f(-2.0f, 9.0f, 0.0f));
    pos.push_back(Vec3f(-1.0f, 9.0f, 0.0f));
    pos.push_back(Vec3f(0.0f, 9.0f, 0.0f));
    pos.push_back(Vec3f(1.0f, 9.0f, 0.0f));
    pos.push_back(Vec3f(2.0f, 9.0f, 0.0f));
    pos.push_back(Vec3f(3.0f, 9.0f, 0.0f));

    pos.push_back(Vec3f(-3.0f, 8.0f, 0.0f));
    pos.push_back(Vec3f(-2.0f, 8.0f, 0.0f));
    pos.push_back(Vec3f(-1.0f, 8.0f, 0.0f));
    pos.push_back(Vec3f(0.0f, 8.0f, 0.0f));
    pos.push_back(Vec3f(1.0f, 8.0f, 0.0f));
    pos.push_back(Vec3f(2.0f, 8.0f, 0.0f));
    pos.push_back(Vec3f(3.0f, 8.0f, 0.0f));
    
    pos.push_back(Vec3f(-3.0f, 7.0f, 0.0f));
    pos.push_back(Vec3f(-2.0f, 7.0f, 0.0f));
    pos.push_back(Vec3f(-1.0f, 7.0f, 0.0f));
    pos.push_back(Vec3f(0.0f, 7.0f, 0.0f));
    pos.push_back(Vec3f(1.0f, 7.0f, 0.0f));
    pos.push_back(Vec3f(2.0f, 7.0f, 0.0f));
    pos.push_back(Vec3f(3.0f, 7.0f, 0.0f));
    
    pos.push_back(Vec3f(-3.0f, 6.0f, 0.0f));
    pos.push_back(Vec3f(-2.0f, 6.0f, 0.0f));
    pos.push_back(Vec3f(-1.0f, 6.0f, 0.0f));
    pos.push_back(Vec3f(0.0f, 6.0f, 0.0f));
    pos.push_back(Vec3f(1.0f, 6.0f, 0.0f));
    pos.push_back(Vec3f(2.0f, 6.0f, 0.0f));
    pos.push_back(Vec3f(3.0f, 6.0f, 0.0f));
    
    pos.push_back(Vec3f(-3.0f, 5.0f, 0.0f));
    pos.push_back(Vec3f(-2.0f, 5.0f, 0.0f));
    pos.push_back(Vec3f(-1.0f, 5.0f, 0.0f));
    pos.push_back(Vec3f(0.0f, 5.0f, 0.0f));
    pos.push_back(Vec3f(1.0f, 5.0f, 0.0f));
    pos.push_back(Vec3f(2.0f, 5.0f, 0.0f));
    pos.push_back(Vec3f(3.0f, 5.0f, 0.0f));
    
    pos.push_back(Vec3f(-3.0f, 4.0f, 0.0f));
    pos.push_back(Vec3f(-2.0f, 4.0f, 0.0f));
    pos.push_back(Vec3f(-1.0f, 4.0f, 0.0f));
    pos.push_back(Vec3f(0.0f, 4.0f, 0.0f));
    pos.push_back(Vec3f(1.0f, 4.0f, 0.0f));
    pos.push_back(Vec3f(2.0f, 4.0f, 0.0f));
    pos.push_back(Vec3f(3.0f, 4.0f, 0.0f));

    std::vector<Constraint> constraints;

    // horizontal contraints
    Constraint h0, h1, h2, h3, h4, h5, h6, h7, h8, h9,
                h10, h11, h12, h13, h14, h15, h16, h17, h18, h19,
                h20, h21, h22, h23, h24, h25, h26, h27, h28, h29,
                h30, h31, h32, h33, h34, h35, h36, h37, h38, h39,
                h40, h41;

    h0.idxA = 0;   h0.idxB = 1;   h0.restlength = 1.0f;     constraints.push_back(h0);
    h1.idxA = 1;   h1.idxB = 2;   h1.restlength = 1.0f;     constraints.push_back(h1);
    h2.idxA = 2;   h2.idxB = 3;   h2.restlength = 1.0f;     constraints.push_back(h2);
    h3.idxA = 3;   h3.idxB = 4;   h3.restlength = 1.0f;     constraints.push_back(h3);
    h4.idxA = 4;   h4.idxB = 5;   h4.restlength = 1.0f;     constraints.push_back(h4);
    h5.idxA = 5;   h5.idxB = 6;   h5.restlength = 1.0f;     constraints.push_back(h5);

    h6.idxA = 7;   h6.idxB = 8;   h6.restlength = 1.0f;     constraints.push_back(h6);
    h7.idxA = 8;   h7.idxB = 9;   h7.restlength = 1.0f;     constraints.push_back(h7);
    h8.idxA = 9;   h8.idxB = 10;  h8.restlength = 1.0f;     constraints.push_back(h8);
    h9.idxA = 10;  h9.idxB = 11;  h9.restlength = 1.0f;     constraints.push_back(h9);
    h10.idxA = 11; h10.idxB = 12; h10.restlength = 1.0f;    constraints.push_back(h10);
    h11.idxA = 12; h11.idxB = 13; h11.restlength = 1.0f;    constraints.push_back(h11);

    h12.idxA = 14; h12.idxB = 15; h12.restlength = 1.0f;    constraints.push_back(h12);
    h13.idxA = 15; h13.idxB = 16; h13.restlength = 1.0f;    constraints.push_back(h13);
    h14.idxA = 16; h14.idxB = 17; h14.restlength = 1.0f;    constraints.push_back(h14);
    h15.idxA = 17; h15.idxB = 18; h15.restlength = 1.0f;    constraints.push_back(h15);
    h16.idxA = 18; h16.idxB = 19; h16.restlength = 1.0f;    constraints.push_back(h16);
    h17.idxA = 19; h17.idxB = 20; h17.restlength = 1.0f;    constraints.push_back(h17);

    h18.idxA = 21; h18.idxB = 22; h18.restlength = 1.0f;    constraints.push_back(h18);
    h19.idxA = 22; h19.idxB = 23; h19.restlength = 1.0f;    constraints.push_back(h19);
    h20.idxA = 23; h20.idxB = 24; h20.restlength = 1.0f;    constraints.push_back(h20);
    h21.idxA = 24; h21.idxB = 25; h21.restlength = 1.0f;    constraints.push_back(h21);
    h22.idxA = 25; h22.idxB = 26; h22.restlength = 1.0f;    constraints.push_back(h22);
    h23.idxA = 26; h23.idxB = 27; h23.restlength = 1.0f;    constraints.push_back(h23);

    h24.idxA = 28; h24.idxB = 29; h24.restlength = 1.0f;    constraints.push_back(h24);
    h25.idxA = 29; h25.idxB = 30; h25.restlength = 1.0f;    constraints.push_back(h25);
    h26.idxA = 30; h26.idxB = 31; h26.restlength = 1.0f;    constraints.push_back(h26);
    h27.idxA = 31; h27.idxB = 32; h27.restlength = 1.0f;    constraints.push_back(h27);
    h28.idxA = 32; h28.idxB = 33; h28.restlength = 1.0f;    constraints.push_back(h28);
    h29.idxA = 33; h29.idxB = 34; h29.restlength = 1.0f;    constraints.push_back(h29);

    h30.idxA = 35; h30.idxB = 36; h30.restlength = 1.0f;    constraints.push_back(h30);
    h31.idxA = 36; h31.idxB = 37; h31.restlength = 1.0f;    constraints.push_back(h31);
    h32.idxA = 37; h32.idxB = 38; h32.restlength = 1.0f;    constraints.push_back(h32);
    h33.idxA = 38; h33.idxB = 39; h33.restlength = 1.0f;    constraints.push_back(h33);
    h34.idxA = 39; h34.idxB = 40; h34.restlength = 1.0f;    constraints.push_back(h34);
    h35.idxA = 40; h35.idxB = 41; h35.restlength = 1.0f;    constraints.push_back(h35);

    h36.idxA = 42; h36.idxB = 43; h36.restlength = 1.0f;    constraints.push_back(h36);
    h37.idxA = 43; h37.idxB = 44; h37.restlength = 1.0f;    constraints.push_back(h37);
    h38.idxA = 44; h38.idxB = 45; h38.restlength = 1.0f;    constraints.push_back(h38);
    h39.idxA = 45; h39.idxB = 46; h39.restlength = 1.0f;    constraints.push_back(h39);
    h40.idxA = 46; h40.idxB = 47; h40.restlength = 1.0f;    constraints.push_back(h40);
    h41.idxA = 47; h41.idxB = 48; h41.restlength = 1.0f;    constraints.push_back(h41);


    // vertical contraints
    Constraint v0, v1, v2, v3, v4, v5, v6, v7, v8, v9,
                v10, v11, v12, v13, v14, v15, v16, v17, v18, v19,
                v20, v21, v22, v23, v24, v25, v26, v27, v28, v29,
                v30, v31, v32, v33, v34, v35, v36, v37, v38, v39,
                v40, v41;

    v0.idxA = 0;   v0.idxB = 7;   v0.restlength = 1.0f;     constraints.push_back(v0);
    v1.idxA = 1;   v1.idxB = 8;   v1.restlength = 1.0f;     constraints.push_back(v1);
    v2.idxA = 2;   v2.idxB = 9;   v2.restlength = 1.0f;     constraints.push_back(v2);
    v3.idxA = 3;   v3.idxB = 10;  v3.restlength = 1.0f;     constraints.push_back(v3);
    v4.idxA = 4;   v4.idxB = 11;  v4.restlength = 1.0f;     constraints.push_back(v4);
    v5.idxA = 5;   v5.idxB = 12;  v5.restlength = 1.0f;     constraints.push_back(v5);
    v6.idxA = 6;   v6.idxB = 13;  v6.restlength = 1.0f;     constraints.push_back(v6);

    v7.idxA = 7;   v7.idxB = 14;  v7.restlength = 1.0f;     constraints.push_back(v7);
    v8.idxA = 8;   v8.idxB = 15;  v8.restlength = 1.0f;     constraints.push_back(v8);
    v9.idxA = 9;   v9.idxB = 16;  v9.restlength = 1.0f;     constraints.push_back(v9);
    v10.idxA = 10; v10.idxB = 17; v10.restlength = 1.0f;    constraints.push_back(v10);
    v11.idxA = 11; v11.idxB = 18; v11.restlength = 1.0f;    constraints.push_back(v11);
    v12.idxA = 12; v12.idxB = 19; v12.restlength = 1.0f;    constraints.push_back(v12);
    v13.idxA = 13; v13.idxB = 20; v13.restlength = 1.0f;    constraints.push_back(v13);

    v14.idxA = 14; v14.idxB = 21; v14.restlength = 1.0f;    constraints.push_back(v14);
    v15.idxA = 15; v15.idxB = 22; v15.restlength = 1.0f;    constraints.push_back(v15);
    v16.idxA = 16; v16.idxB = 23; v16.restlength = 1.0f;    constraints.push_back(v16);
    v17.idxA = 17; v17.idxB = 24; v17.restlength = 1.0f;    constraints.push_back(v17);
    v18.idxA = 18; v18.idxB = 25; v18.restlength = 1.0f;    constraints.push_back(v18);
    v19.idxA = 19; v19.idxB = 26; v19.restlength = 1.0f;    constraints.push_back(v19);
    v20.idxA = 20; v20.idxB = 27; v20.restlength = 1.0f;    constraints.push_back(v20);

    v21.idxA = 21; v21.idxB = 28; v21.restlength = 1.0f;    constraints.push_back(v21);
    v22.idxA = 22; v22.idxB = 29; v22.restlength = 1.0f;    constraints.push_back(v22);
    v23.idxA = 23; v23.idxB = 30; v23.restlength = 1.0f;    constraints.push_back(v23);
    v24.idxA = 24; v24.idxB = 31; v24.restlength = 1.0f;    constraints.push_back(v24);
    v25.idxA = 25; v25.idxB = 32; v25.restlength = 1.0f;    constraints.push_back(v25);
    v26.idxA = 26; v26.idxB = 33; v26.restlength = 1.0f;    constraints.push_back(v26);
    v27.idxA = 27; v27.idxB = 34; v27.restlength = 1.0f;    constraints.push_back(v27);

    v28.idxA = 28; v28.idxB = 35; v28.restlength = 1.0f;    constraints.push_back(v28);
    v29.idxA = 29; v29.idxB = 36; v29.restlength = 1.0f;    constraints.push_back(v29);
    v30.idxA = 30; v30.idxB = 37; v30.restlength = 1.0f;    constraints.push_back(v30);
    v31.idxA = 31; v31.idxB = 38; v31.restlength = 1.0f;    constraints.push_back(v31);
    v32.idxA = 32; v32.idxB = 39; v32.restlength = 1.0f;    constraints.push_back(v32);
    v33.idxA = 33; v33.idxB = 40; v33.restlength = 1.0f;    constraints.push_back(v33);
    v34.idxA = 34; v34.idxB = 41; v34.restlength = 1.0f;    constraints.push_back(v34);

    v35.idxA = 35; v35.idxB = 42; v35.restlength = 1.0f;    constraints.push_back(v35);
    v36.idxA = 36; v36.idxB = 43; v36.restlength = 1.0f;    constraints.push_back(v36);
    v37.idxA = 37; v37.idxB = 44; v37.restlength = 1.0f;    constraints.push_back(v37);
    v38.idxA = 38; v38.idxB = 45; v38.restlength = 1.0f;    constraints.push_back(v38);
    v39.idxA = 39; v39.idxB = 46; v39.restlength = 1.0f;    constraints.push_back(v39);
    v40.idxA = 40; v40.idxB = 47; v40.restlength = 1.0f;    constraints.push_back(v40);
    v41.idxA = 41; v41.idxB = 48; v41.restlength = 1.0f;    constraints.push_back(v41);


    // diagonal foward contraints
    Constraint f0, f1, f2, f3, f4, f5, f6, f7, f8, f9,
                f10, f11, f12, f13, f14, f15, f16, f17, f18, f19,
                f20, f21, f22, f23, f24, f25, f26, f27, f28, f29,
                f30, f31, f32, f33, f34, f35;

    f0.idxA = 0;   f0.idxB = 8;   f0.restlength = sqrt(2.0f);     constraints.push_back(f0);
    f1.idxA = 1;   f1.idxB = 9;   f1.restlength = sqrt(2.0f);     constraints.push_back(f1);
    f2.idxA = 2;   f2.idxB = 10;  f2.restlength = sqrt(2.0f);     constraints.push_back(f2);
    f3.idxA = 3;   f3.idxB = 11;  f3.restlength = sqrt(2.0f);     constraints.push_back(f3);
    f4.idxA = 4;   f4.idxB = 12;  f4.restlength = sqrt(2.0f);     constraints.push_back(f4);
    f5.idxA = 5;   f5.idxB = 13;  f5.restlength = sqrt(2.0f);     constraints.push_back(f5);
    
    f6.idxA = 7;   f6.idxB = 15;  f6.restlength = sqrt(2.0f);     constraints.push_back(f6);
    f7.idxA = 8;   f7.idxB = 16;  f7.restlength = sqrt(2.0f);     constraints.push_back(f7);
    f8.idxA = 9;   f8.idxB = 17;  f8.restlength = sqrt(2.0f);     constraints.push_back(f8);
    f9.idxA = 10;  f9.idxB = 18;  f9.restlength = sqrt(2.0f);     constraints.push_back(f9);
    f10.idxA = 11; f10.idxB = 19; f10.restlength = sqrt(2.0f);    constraints.push_back(f10);
    f11.idxA = 12; f11.idxB = 20; f11.restlength = sqrt(2.0f);    constraints.push_back(f11);

    f12.idxA = 14; f12.idxB = 22; f12.restlength = sqrt(2.0f);    constraints.push_back(f12);
    f13.idxA = 15; f13.idxB = 23; f13.restlength = sqrt(2.0f);    constraints.push_back(f13);
    f14.idxA = 16; f14.idxB = 24; f14.restlength = sqrt(2.0f);    constraints.push_back(f14);
    f15.idxA = 17; f15.idxB = 25; f15.restlength = sqrt(2.0f);    constraints.push_back(f15);
    f16.idxA = 18; f16.idxB = 26; f16.restlength = sqrt(2.0f);    constraints.push_back(f16);
    f17.idxA = 19; f17.idxB = 27; f17.restlength = sqrt(2.0f);    constraints.push_back(f17);

    f18.idxA = 21; f18.idxB = 29; f18.restlength = sqrt(2.0f);    constraints.push_back(f18);
    f19.idxA = 22; f19.idxB = 30; f19.restlength = sqrt(2.0f);    constraints.push_back(f19);
    f20.idxA = 23; f20.idxB = 31; f20.restlength = sqrt(2.0f);    constraints.push_back(f20);
    f21.idxA = 24; f21.idxB = 32; f21.restlength = sqrt(2.0f);    constraints.push_back(f21);
    f22.idxA = 25; f22.idxB = 33; f22.restlength = sqrt(2.0f);    constraints.push_back(f22);
    f23.idxA = 26; f23.idxB = 34; f23.restlength = sqrt(2.0f);    constraints.push_back(f23);

    f24.idxA = 28; f24.idxB = 36; f24.restlength = sqrt(2.0f);    constraints.push_back(f24);
    f25.idxA = 29; f25.idxB = 37; f25.restlength = sqrt(2.0f);    constraints.push_back(f25);
    f26.idxA = 30; f26.idxB = 38; f26.restlength = sqrt(2.0f);    constraints.push_back(f26);
    f27.idxA = 31; f27.idxB = 39; f27.restlength = sqrt(2.0f);    constraints.push_back(f27);
    f28.idxA = 32; f28.idxB = 40; f28.restlength = sqrt(2.0f);    constraints.push_back(f28);
    f29.idxA = 33; f29.idxB = 41; f29.restlength = sqrt(2.0f);    constraints.push_back(f29);

    f30.idxA = 35; f30.idxB = 43; f30.restlength = sqrt(2.0f);    constraints.push_back(f30);
    f31.idxA = 36; f31.idxB = 44; f31.restlength = sqrt(2.0f);    constraints.push_back(f31);
    f32.idxA = 37; f32.idxB = 45; f32.restlength = sqrt(2.0f);    constraints.push_back(f32);
    f33.idxA = 38; f33.idxB = 46; f33.restlength = sqrt(2.0f);    constraints.push_back(f33);
    f34.idxA = 39; f34.idxB = 47; f34.restlength = sqrt(2.0f);    constraints.push_back(f34);
    f35.idxA = 40; f35.idxB = 48; f35.restlength = sqrt(2.0f);    constraints.push_back(f35);


    // diagonal backward contraints
    Constraint b0, b1, b2, b3, b4, b5, b6, b7, b8, b9,
                b10, b11, b12, b13, b14, b15, b16, b17, b18, b19,
                b20, b21, b22, b23, b24, b25, b26, b27, b28, b29,
                b30, b31, b32, b33, b34, b35;

    b0.idxA = 1;   b0.idxB = 7;   b0.restlength = sqrt(2.0f);     constraints.push_back(b0);
    b1.idxA = 2;   b1.idxB = 8;   b1.restlength = sqrt(2.0f);     constraints.push_back(b1);
    b2.idxA = 3;   b2.idxB = 9;   b2.restlength = sqrt(2.0f);     constraints.push_back(b2);
    b3.idxA = 4;   b3.idxB = 10;  b3.restlength = sqrt(2.0f);     constraints.push_back(b3);
    b4.idxA = 5;   b4.idxB = 11;  b4.restlength = sqrt(2.0f);     constraints.push_back(b4);
    b5.idxA = 6;   b5.idxB = 12;  b5.restlength = sqrt(2.0f);     constraints.push_back(b5);
    
    b6.idxA = 8;   b6.idxB = 14;  b6.restlength = sqrt(2.0f);     constraints.push_back(b6);
    b7.idxA = 9;   b7.idxB = 15;  b7.restlength = sqrt(2.0f);     constraints.push_back(b7);
    b8.idxA = 10;  b8.idxB = 16;  b8.restlength = sqrt(2.0f);     constraints.push_back(b8);
    b9.idxA = 11;  b9.idxB = 17;  b9.restlength = sqrt(2.0f);     constraints.push_back(b9);
    b10.idxA = 12; b10.idxB = 18; b10.restlength = sqrt(2.0f);    constraints.push_back(b10);
    b11.idxA = 13; b11.idxB = 19; b11.restlength = sqrt(2.0f);    constraints.push_back(b11);

    b12.idxA = 15; b12.idxB = 21; b12.restlength = sqrt(2.0f);    constraints.push_back(b12);
    b13.idxA = 16; b13.idxB = 22; b13.restlength = sqrt(2.0f);    constraints.push_back(b13);
    b14.idxA = 17; b14.idxB = 23; b14.restlength = sqrt(2.0f);    constraints.push_back(b14);
    b15.idxA = 18; b15.idxB = 24; b15.restlength = sqrt(2.0f);    constraints.push_back(b15);
    b16.idxA = 19; b16.idxB = 25; b16.restlength = sqrt(2.0f);    constraints.push_back(b16);
    b17.idxA = 20; b17.idxB = 26; b17.restlength = sqrt(2.0f);    constraints.push_back(b17);

    b18.idxA = 22; b18.idxB = 28; b18.restlength = sqrt(2.0f);    constraints.push_back(b18);
    b19.idxA = 23; b19.idxB = 29; b19.restlength = sqrt(2.0f);    constraints.push_back(b19);
    b20.idxA = 24; b20.idxB = 30; b20.restlength = sqrt(2.0f);    constraints.push_back(b20);
    b21.idxA = 25; b21.idxB = 31; b21.restlength = sqrt(2.0f);    constraints.push_back(b21);
    b22.idxA = 26; b22.idxB = 32; b22.restlength = sqrt(2.0f);    constraints.push_back(b22);
    b23.idxA = 27; b23.idxB = 33; b23.restlength = sqrt(2.0f);    constraints.push_back(b23);

    b24.idxA = 29; b24.idxB = 35; b24.restlength = sqrt(2.0f);    constraints.push_back(b24);
    b25.idxA = 30; b25.idxB = 36; b25.restlength = sqrt(2.0f);    constraints.push_back(b25);
    b26.idxA = 31; b26.idxB = 37; b26.restlength = sqrt(2.0f);    constraints.push_back(b26);
    b27.idxA = 32; b27.idxB = 38; b27.restlength = sqrt(2.0f);    constraints.push_back(b27);
    b28.idxA = 33; b28.idxB = 39; b28.restlength = sqrt(2.0f);    constraints.push_back(b28);
    b29.idxA = 34; b29.idxB = 40; b29.restlength = sqrt(2.0f);    constraints.push_back(b29);

    b30.idxA = 36; b30.idxB = 42; b30.restlength = sqrt(2.0f);    constraints.push_back(b30);
    b31.idxA = 37; b31.idxB = 43; b31.restlength = sqrt(2.0f);    constraints.push_back(b31);
    b32.idxA = 38; b32.idxB = 44; b32.restlength = sqrt(2.0f);    constraints.push_back(b32);
    b33.idxA = 39; b33.idxB = 45; b33.restlength = sqrt(2.0f);    constraints.push_back(b33);
    b34.idxA = 40; b34.idxB = 46; b34.restlength = sqrt(2.0f);    constraints.push_back(b34);
    b35.idxA = 41; b35.idxB = 47; b35.restlength = sqrt(2.0f);    constraints.push_back(b35);


    // extra horizontal contraints
    Constraint eh0, eh1, eh2, eh3, eh4, eh5, eh6, eh7, eh8, eh9, eh10, eh11;

    eh0.idxA = 0;   eh0.idxB = 2;   eh0.restlength = 2.0f;     constraints.push_back(eh0);
    eh1.idxA = 2;   eh1.idxB = 4;   eh1.restlength = 2.0f;     constraints.push_back(eh1);
    eh2.idxA = 4;   eh2.idxB = 6;   eh2.restlength = 2.0f;     constraints.push_back(eh2);

    eh3.idxA = 14;  eh3.idxB = 16;  eh3.restlength = 2.0f;     constraints.push_back(eh3);
    eh4.idxA = 16;  eh4.idxB = 18;  eh4.restlength = 2.0f;     constraints.push_back(eh4);
    eh5.idxA = 18;  eh5.idxB = 20;  eh5.restlength = 2.0f;     constraints.push_back(eh5);

    eh6.idxA = 28;  eh6.idxB = 30;  eh6.restlength = 2.0f;     constraints.push_back(eh6);
    eh7.idxA = 30;  eh7.idxB = 32;  eh7.restlength = 2.0f;     constraints.push_back(eh7);
    eh8.idxA = 32;  eh8.idxB = 34;  eh8.restlength = 2.0f;     constraints.push_back(eh8);

    eh9.idxA = 42;  eh9.idxB = 44;  eh9.restlength = 2.0f;     constraints.push_back(eh9);
    eh10.idxA = 44; eh10.idxB = 46; eh10.restlength = 2.0f;    constraints.push_back(eh10);
    eh11.idxA = 46; eh11.idxB = 48; eh11.restlength = 2.0f;    constraints.push_back(eh11);

    // extra vertical contraints
    Constraint ev0, ev1, ev2, ev3, ev4, ev5, ev6, ev7, ev8, ev9, ev10, ev11;

    ev0.idxA = 0;   ev0.idxB = 14;  ev0.restlength = 2.0f;     constraints.push_back(ev0);
    ev1.idxA = 2;   ev1.idxB = 16;  ev1.restlength = 2.0f;     constraints.push_back(ev1);
    ev2.idxA = 4;   ev2.idxB = 18;  ev2.restlength = 2.0f;     constraints.push_back(ev2);
    ev3.idxA = 6;   ev3.idxB = 20;  ev3.restlength = 2.0f;     constraints.push_back(ev3);

    ev4.idxA = 14;  ev4.idxB = 28;  ev4.restlength = 2.0f;     constraints.push_back(ev4);
    ev5.idxA = 16;  ev5.idxB = 30;  ev5.restlength = 2.0f;     constraints.push_back(ev5);
    ev6.idxA = 18;  ev6.idxB = 32;  ev6.restlength = 2.0f;     constraints.push_back(ev6);
    ev7.idxA = 20;  ev7.idxB = 34;  ev7.restlength = 2.0f;     constraints.push_back(ev7);

    ev8.idxA = 28;  ev8.idxB = 42;  ev8.restlength = 2.0f;     constraints.push_back(ev8);
    ev9.idxA = 30;  ev9.idxB = 44;  ev9.restlength = 2.0f;     constraints.push_back(ev9);
    ev10.idxA = 32; ev10.idxB = 46; ev10.restlength = 2.0f;    constraints.push_back(ev10);
    ev11.idxA = 34; ev11.idxB = 48; ev11.restlength = 2.0f;    constraints.push_back(ev11);


    // extra diagonal contraints
    Constraint ed0, ed1, ed2, ed3, ed4, ed5, ed6, ed7, ed8, ed9, ed10, ed11,
                ed12, ed13, ed14, ed15, ed16, ed17;

    ed0.idxA = 0;   ed0.idxB = 16;  ed0.restlength = 2.0f * sqrt(2.0f);     constraints.push_back(ed0);
    ed1.idxA = 2;   ed1.idxB = 14;  ed1.restlength = 2.0f * sqrt(2.0f);     constraints.push_back(ed1);
    ed2.idxA = 2;   ed2.idxB = 18;  ed2.restlength = 2.0f * sqrt(2.0f);     constraints.push_back(ed2);
    ed3.idxA = 4;   ed3.idxB = 16;  ed3.restlength = 2.0f * sqrt(2.0f);     constraints.push_back(ed3);
    ed4.idxA = 4;   ed4.idxB = 20;  ed4.restlength = 2.0f * sqrt(2.0f);     constraints.push_back(ed4);
    ed5.idxA = 6;   ed5.idxB = 18;  ed5.restlength = 2.0f * sqrt(2.0f);     constraints.push_back(ed5);

    ed6.idxA = 14;  ed6.idxB = 30;  ed6.restlength =  2.0f * sqrt(2.0f);    constraints.push_back(ed6);
    ed7.idxA = 16;  ed7.idxB = 28;  ed7.restlength =  2.0f * sqrt(2.0f);    constraints.push_back(ed7);
    ed8.idxA = 16;  ed8.idxB = 32;  ed8.restlength =  2.0f * sqrt(2.0f);    constraints.push_back(ed8);
    ed9.idxA = 18;  ed9.idxB = 30;  ed9.restlength =  2.0f * sqrt(2.0f);    constraints.push_back(ed9);
    ed10.idxA = 18; ed10.idxB = 34; ed10.restlength = 2.0f * sqrt(2.0f);    constraints.push_back(ed10);
    ed11.idxA = 20; ed11.idxB = 32; ed11.restlength = 2.0f * sqrt(2.0f);    constraints.push_back(ed11);

    ed12.idxA = 28;  ed12.idxB = 44;  ed12.restlength = 2.0f * sqrt(2.0f);  constraints.push_back(ed12);
    ed13.idxA = 30;  ed13.idxB = 42;  ed13.restlength = 2.0f * sqrt(2.0f);  constraints.push_back(ed13);
    ed14.idxA = 30;  ed14.idxB = 46;  ed14.restlength = 2.0f * sqrt(2.0f);  constraints.push_back(ed14);
    ed15.idxA = 32;  ed15.idxB = 44;  ed15.restlength = 2.0f * sqrt(2.0f);  constraints.push_back(ed15);
    ed16.idxA = 32;  ed16.idxB = 48;  ed16.restlength = 2.0f * sqrt(2.0f);  constraints.push_back(ed16);
    ed17.idxA = 34;  ed17.idxB = 46;  ed17.restlength = 2.0f * sqrt(2.0f);  constraints.push_back(ed17);



    std::vector<bool> isMovable;
    isMovable.resize(49);
    isMovable[0] = false;
    isMovable[1] = true;
    isMovable[2] = true;
    isMovable[3] = true;
    isMovable[4] = true;
    isMovable[5] = true;
    isMovable[6] = true;

    isMovable[7] = false;
    isMovable[8] = true;
    isMovable[9] = true;
    isMovable[10] = true;
    isMovable[11] = true;
    isMovable[12] = true;
    isMovable[13] = true;
    
    isMovable[14] = false;
    isMovable[15] = true;
    isMovable[16] = true;
    isMovable[17] = true;
    isMovable[18] = true;
    isMovable[19] = true;
    isMovable[20] = true;
    
    isMovable[21] = false;
    isMovable[22] = true;
    isMovable[23] = true;
    isMovable[24] = true;
    isMovable[25] = true;
    isMovable[26] = true;
    isMovable[27] = true;
    
    isMovable[28] = false;
    isMovable[29] = true;
    isMovable[30] = true;
    isMovable[31] = true;
    isMovable[32] = true;
    isMovable[33] = true;
    isMovable[34] = true;
    
    isMovable[35] = false;
    isMovable[36] = true;
    isMovable[37] = true;
    isMovable[38] = true;
    isMovable[39] = true;
    isMovable[40] = true;
    isMovable[41] = true;
    
    isMovable[42] = false;
    isMovable[43] = true;
    isMovable[44] = true;
    isMovable[45] = true;
    isMovable[46] = true;
    isMovable[47] = true;
    isMovable[48] = true;

    clothSystem = ClothSimulationSystem(pos, constraints, isMovable);
}



void printUsage()
{
    std::cout << "==================================================================" << std::endl;
    std::cout << "======================== CLOTH SIMULATION ========================" << std::endl;
    std::cout << "==================================================================" << std::endl << std::endl;

    std::cout << "Project made by Bernard Lupiac in nov-dec 2018, for the INF1608 project." << std::endl << std::endl;

    std::cout << "Left click + drag to rotate camera." << std::endl;
    std::cout << "Right click + drag to move in the scene." << std::endl;
    std::cout << "Mousewheel click + drag, or scrolling to zoom in and out." << std::endl << std::endl;

    std::cout << "Press 'S' to make a timestep in the simulation." << std::endl;
    std::cout << "(tip: keeping 'S' pressed makes the system advance in a quasi-realistic speed)" << std::endl;
    std::cout << "Press 'A' to toggle automatic timestep." << std::endl;
    std::cout << "Press 'R' to reset the camera position and rotation." << std::endl;
    std::cout << "Press 'W' to toggle wind force on the simulation." << std::endl << std::endl;
    
    std::cout << "Press '1' to load the string example." << std::endl;
    std::cout << "Press '2' to load the cube example." << std::endl;
    std::cout << "Press '3' to load the fixed string example." << std::endl;
    std::cout << "Press '4' to load the fixed cube example." << std::endl;
    std::cout << "Press '5' to load the strong fixed cube example." << std::endl;
    std::cout << "Press '6' to load the extra strong fixed cube example." << std::endl;
    std::cout << "Press '7' to load the cloth patch example." << std::endl;
    std::cout << "Press '8' to load the strong cloth patch example." << std::endl;
    std::cout << "Press '9' to load the extra strong cloth patch example." << std::endl;
    std::cout << "Press '0' to load the compressed string example." << std::endl << std::endl;

    std::cout << "Press 'Q' or 'Esc' to quit the application." << std::endl << std::endl;
}

void keyboardEventListener (unsigned char keyPressed, int x, int y) 
{
    // avoids unused warning
    (void) x; (void) y;

    switch (keyPressed) 
    {
        case 'q':
        case 27:
            std::cout << "Quitting app." << std::endl;
            exit (0);
            break;
        case 's':
            step(STANDARD_TIMESTEP);
            break;
        case 'a':
            autoUpdate = !autoUpdate;
            if(autoUpdate)
            {
                std::cout << "Automatic update enabled." << std::endl;
            }
            else
            {
                std::cout << "Automatic update disabled." << std::endl;
            }
            display();
            break;
        case 'r':
            std::cout << "Resetting camera." << std::endl;
            camera = Camera();
            display();
            break;
        case 'w':
            wind = !wind;
            if(wind)
            {
                std::cout << "Wind enabled." << std::endl;
            }
            else
            {
                std::cout << "Wind disabled." << std::endl;
            }
            display();
            break;
        case '1':
            std::cout << "Loading string example." << std::endl;
            loadStringExample();
            display();
            break;
        case '2':
            std::cout << "Loading cube example." << std::endl;
            loadCubeExample();
            display();
            break;
        case '3':
            std::cout << "Loading fixed string example." << std::endl;
            loadFixedStringExample();
            display();
            break;
        case '4':
            std::cout << "Loading fixed cube example." << std::endl;
            loadFixedCubeExample();
            display();
            break;
        case '5':
            std::cout << "Loading fixed strong cube example." << std::endl;
            loadFixedStrongCubeExample();
            display();
            break;
        case '6':
            std::cout << "Loading fixed extra strong cube example." << std::endl;
            loadFixedExtraStrongCubeExample();
            display();
            break;
        case '7':
            std::cout << "Loading cloth patch example." << std::endl;
            loadClothPatchExample();
            display();
            break;
        case '8':
            std::cout << "Loading strong cloth patch example." << std::endl;
            loadStrongClothPatchExample();
            display();
            break;
        case '9':
            std::cout << "Loading extra strong cloth patch example." << std::endl;
            loadExtraStrongClothPatchExample();
            display();
            break;
        case '0':
            std::cout << "Loading compressed string example." << std::endl;
            loadCompressedStringExample();
            display();
            break;
        default:
            printUsage();
            break;
    }
}

void idle()
{
    if(autoUpdate)
    {
        static float deltaTime = glutGet ((GLenum)GLUT_ELAPSED_TIME);
        step(deltaTime * AUTO_UPDATE_FACTOR);
    }
}

int main (int argc, char ** argv) 
{
    srand ( unsigned ( time (NULL) ) );

    printUsage();

    loadStringExample();

    glutInit(&argc, argv);                 // Initialize GLUT
    glutInitWindowSize(DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);   // Set the window's initial width & height
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
    glutCreateWindow("Cloth Simulation using Verlet integration"); // Create a window with the given title
    glClearColor(0.83f, 0.82f, 0.71, 1.0);
    glLineWidth(5);
    glEnable(GL_CULL_FACE);
    
    camera.resize (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);

    glutIdleFunc (idle);
    glutKeyboardFunc (keyboardEventListener);
    glutReshapeFunc (reshapeEventListener);
    glutMotionFunc (motionEventListener);
    glutMouseFunc (mouseEventListener);
    glutDisplayFunc(display); // Register display callback handler for window re-paint
    glutMainLoop();           // Enter the infinitely event-processing loop

    return 0;
}