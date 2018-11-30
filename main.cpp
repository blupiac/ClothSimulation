//-----------------------------------------------------------------------------
// Author: Bernard Lupiac
// Created: 15/11/2018
//-----------------------------------------------------------------------------

#include <iostream>
#include <time.h>
#include <GL/glut.h>

#include "ClothSimulationSystem.hpp"
#include "Vec3.hpp"
#include "Camera.hpp"


static const unsigned int DEFAULT_SCREENWIDTH = 1024;
static const unsigned int DEFAULT_SCREENHEIGHT = 768;

static ClothSimulationSystem clothSystem;
static Camera camera;

void renderScene() 
{
    std::vector<Vec3f> pos = clothSystem.getPos();
    std::vector<Constraint> constraints = clothSystem.getConstraints();

    glBegin(GL_LINES);
        
    // each constraint = one line
    for(std::vector<Constraint>::iterator it = constraints.begin();
        it != constraints.end(); ++it) 
    {
        Constraint c = *it;
        Vec3f pA = pos[c.idxA];
        Vec3f pB = pos[c.idxB];

        glVertex3f(pA[0], pA[1], pA[2]);
        glVertex3f(pB[0], pB[1], pB[2]);
    }

    glEnd();
}

void reshapeEventListener(int w, int h) 
{
    camera.resize (w, h);
}

void display () 
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply (); 
    renderScene ();
    glFlush ();
    glutSwapBuffers (); 
}

void mouseEventListener (int button, int state, int x, int y) 
{
    camera.handleMouseClickEvent (button, state, x, y);
}

void motionEventListener (int x, int y) 
{
    camera.handleMouseMoveEvent (x, y);
}

void printVector(std::vector<Vec3f> vec)
{
    for(int i = 0; i < vec.size(); i++)
    {
        std::cout << "Point " << i << " : " << vec[i] << std::endl;
    }
    std::cout << std::endl;
}

void step()
{
    float deltaT = 0.001f;
    Vec3f gravity = Vec3f(0.0f, -9.81f, 0.0f);

    clothSystem.ApplyForce(gravity);
    clothSystem.TimeStep(deltaT);

    display();
}

void keyboardEventListener (unsigned char keyPressed, int x, int y) 
{

    switch (keyPressed) 
    {
        case 'q':
        case 27:
            exit (0);
            break;
        case 's':
            step();
            break;
        default:
            break;
    }
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

    clothSystem = ClothSimulationSystem(pos, constraints);

    glutInit(&argc, argv);                 // Initialize GLUT
    glutInitWindowSize(DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);   // Set the window's initial width & height
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
    glutCreateWindow("OpenGL Setup Test"); // Create a window with the given title
    
    camera.resize (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);

    glutKeyboardFunc (keyboardEventListener);
    glutReshapeFunc (reshapeEventListener);
    glutMotionFunc (motionEventListener);
    glutMouseFunc (mouseEventListener);
    glutDisplayFunc(display); // Register display callback handler for window re-paint
    glutMainLoop();           // Enter the infinitely event-processing loop

    return 0;
}