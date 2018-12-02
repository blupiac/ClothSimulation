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
static bool wind = false;


void printVector(std::vector<Vec3f> vec)
{
    for(int i = 0; i < vec.size(); i++)
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

void step()
{
    float deltaT = 0.001f;
    Vec3f gravity = Vec3f(0.0f, -9.81f, 0.0f);

    if(wind)
    {
        clothSystem.ApplyForce(getWindForce());
    }

    clothSystem.ApplyForce(gravity);
    clothSystem.TimeStep(deltaT);

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
    c0.idxA = 0; c0.idxB = 1; c0.restlength = 3.0f;
    c1.idxA = 1; c1.idxB = 2; c1.restlength = 3.0f;
    c2.idxA = 2; c2.idxB = 3; c2.restlength = 3.0f;
    c3.idxA = 3; c3.idxB = 4; c3.restlength = 3.0f;
    constraints.push_back(c0);
    constraints.push_back(c1);
    constraints.push_back(c2);
    constraints.push_back(c3);

    clothSystem = ClothSimulationSystem(pos, constraints);
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

    clothSystem = ClothSimulationSystem(pos, constraints);
}

void keyboardEventListener (unsigned char keyPressed, int x, int y) 
{
    switch (keyPressed) 
    {
        case 'q':
        case 27:
            std::cout << "Quitting app." << std::endl;
            exit (0);
            break;
        case 's':
            step();
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
        default:
            break;
    }
}

int main (int argc, char ** argv) 
{
    srand ( unsigned ( time (NULL) ) );

    loadStringExample();

    glutInit(&argc, argv);                 // Initialize GLUT
    glutInitWindowSize(DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);   // Set the window's initial width & height
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
    glutCreateWindow("Cloth Simulation using Verlet integration"); // Create a window with the given title
    glClearColor(0.83f, 0.82f, 0.71, 1.0);
    glLineWidth(5);
    glEnable(GL_CULL_FACE);
    
    camera.resize (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);

    glutKeyboardFunc (keyboardEventListener);
    glutReshapeFunc (reshapeEventListener);
    glutMotionFunc (motionEventListener);
    glutMouseFunc (mouseEventListener);
    glutDisplayFunc(display); // Register display callback handler for window re-paint
    glutMainLoop();           // Enter the infinitely event-processing loop

    return 0;
}