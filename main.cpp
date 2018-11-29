//-----------------------------------------------------------------------------
// Author: Bernard Lupiac
// Created: 15/11/2018
//-----------------------------------------------------------------------------

#include <iostream>
#include <time.h>
#include <GL/glut.h>

#include "ClothSimulationSystem.hpp"
#include "Vec3.hpp"

//#define CLOCK_TIME

void display() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer
 
    // Draw a Red 1x1 Square centered at origin
    glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
        glColor3f(1.0f, 0.0f, 0.0f); // Red
        glVertex2f(-0.5f, -0.5f);    // x, y
        glVertex2f( 0.5f, -0.5f);
        glVertex2f( 0.5f,  0.5f);
        glVertex2f(-0.5f,  0.5f);
    glEnd();
 
    glFlush();  // Render now
}

void printVector(std::vector<Vec3f> vec)
{
    for(int i = 0; i < vec.size(); i++)
    {
        std::cout << "Point " << i << " : " << vec[i] << std::endl;
    }
    std::cout << std::endl;
}

int main (int argc, char ** argv) 
{
    std::vector<Vec3f> pos;
    pos.push_back(Vec3f(0.0f, 0.0f, 0.0f));
    pos.push_back(Vec3f(1.0f, 1.0f, 1.0f));
    pos.push_back(Vec3f(2.0f, 2.0f, 2.0f));
    pos.push_back(Vec3f(3.0f, 3.0f, 3.0f));
    pos.push_back(Vec3f(4.0f, 4.0f, 4.0f));

    std::vector<Constraint> constraints;
    Constraint c0, c1, c2, c3, c4;
    c0.idxA = 0; c0.idxB = 1; c0.restlength = 3.0f;
    c1.idxA = 1; c1.idxB = 2; c1.restlength = 3.0f;
    c2.idxA = 2; c2.idxB = 3; c2.restlength = 3.0f;
    c3.idxA = 3; c3.idxB = 4; c3.restlength = 3.0f;
    constraints.push_back(c0);
    constraints.push_back(c1);
    constraints.push_back(c2);
    constraints.push_back(c3);

    ClothSimulationSystem clothSystem = ClothSimulationSystem(pos, constraints);

    int maxSteps = 100;
#ifdef CLOCK_TIME
    clock_t firstClock, lastClock;
    lastClock = firstClock = clock();
#else
    float deltaT = 0.001f;
    float currTime = 0.000f;
#endif

    Vec3f gravity = Vec3f(0.0f, -9.81f, 0.0f);

    for(int step = 0; step < maxSteps; step++)
    {
#ifdef CLOCK_TIME
        clock_t thisClock = clock();
        double deltaT = (thisClock - lastClock) / (double) CLOCKS_PER_SEC;
        double currTime = (thisClock - firstClock) / (double) CLOCKS_PER_SEC;
#else
        currTime += deltaT;
#endif

        clothSystem.ApplyForce(gravity);
        clothSystem.TimeStep(deltaT);

        std::cout << "Time: " << currTime << ", deltaT: " << deltaT << std::endl;
        std::vector<Vec3f> currentPos = clothSystem.getPos();
        printVector(currentPos);

#ifdef CLOCK_TIME
        lastClock = thisClock;
#endif
    }

    glutInit(&argc, argv);                 // Initialize GLUT
    glutCreateWindow("OpenGL Setup Test"); // Create a window with the given title
    glutInitWindowSize(320, 320);   // Set the window's initial width & height
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
    glutDisplayFunc(display); // Register display callback handler for window re-paint
    glutMainLoop();           // Enter the infinitely event-processing loop
    return 0;
}