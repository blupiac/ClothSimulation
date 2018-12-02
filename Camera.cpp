//-----------------------------------------------------------------------------
// Author: Bernard Lupiac
// Created: 01/12/2018
// Based on: https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/7.3.camera_mouse_zoom/camera_mouse_zoom.cpp
//-----------------------------------------------------------------------------

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <iostream>
#include <string>
#include <math.h>

#include "Camera.hpp"

const float ROTATE_SENSITIVITY = 0.1f;
const float MOVE_SPEED = 0.005f;
const float ZOOM_SPEED = 0.01f;
const float ZOOM_WHEEL_SPEED = 0.1f;

const float NEAR = 0.01f;
const float FAR = 50.0f;

Camera::Camera () 
{
    rotating = moving = zooming = false;

    yaw   = -90.0f;   // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
    pitch =  0.0f;
    lastX =  800.0f / 2.0f;
    lastY =  600.0f / 2.0f;
    lastZoom = 0.0f;
    fov   =  45.0f;

    cameraPos[0] = 2.0f; cameraPos[1] = 2.0f; cameraPos[2] = 10.0f;
    cameraFront[0] = 0.0f; cameraFront[1] = 0.0f; cameraFront[2] = -1.0f;
    cameraUp[0] = 0.0f; cameraUp[1] = 1.0f; cameraUp[2] = 0.0f;
    cameraRight[0] = 1.0f; cameraRight[1] = 0.0f; cameraRight[2] = 0.0f;
    worldUp[0] = 0.0f; worldUp[1] = 1.0f; worldUp[2] = 0.0f;
}

Vec3f Camera::getPos ()
{
    return cameraPos;
}

Vec3f Camera::getFront ()
{
    return cameraFront;
}

Vec3f Camera::getUp ()
{
    return cameraUp;
}


void Camera::resize (int W, int H) 
{
    glViewport (0, 0, (GLint)W, (GLint)H);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    float aspectRatio = static_cast<float>(W)/static_cast<float>(H);
    gluPerspective (fov, aspectRatio, NEAR, FAR);
    glMatrixMode (GL_MODELVIEW);
}


void Camera::handleMouseClickEvent (int button, int state, int x, int y) {
    if (state == GLUT_UP) 
    {
        moving = false;
        rotating = false;
        zooming = false;
    } 
    else 
    {
        if (button == GLUT_LEFT_BUTTON) 
        {
            lastX = x;
            lastY = y;
            moving = false;
            rotating = true;
            zooming = false;
        } 
        else if (button == GLUT_RIGHT_BUTTON) 
        {
            lastX = x;
            lastY = y;
            moving = true;
            rotating = false;
            zooming = false;
        } 
        else if (button == GLUT_MIDDLE_BUTTON) 
        {
            if (zooming == false) 
            {
                lastZoom = y;
                moving = false;
                rotating = false;
                zooming = true;
            }
        }
        else if(button == 3) // mouse wheel scroll down
        {
            cameraPos = cameraPos + cameraFront * ZOOM_WHEEL_SPEED;
        }
        else if(button == 4) // mouse wheel scroll down
        {
            cameraPos = cameraPos - cameraFront * ZOOM_WHEEL_SPEED;
        }
    }
}

float toRadians(float degrees)
{
    return degrees * 3.14159265f / 180.0f;
}

void Camera::handleMouseMoveEvent(int x, int y) 
{
	if (rotating)
    {
        float xOffset = x - lastX;
        float yOffset = lastY - y; // reversed since y-coordinates go from bottom to top

        xOffset *= ROTATE_SENSITIVITY;
        yOffset *= ROTATE_SENSITIVITY;

        yaw += xOffset;
        pitch += yOffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        Vec3f front;
        front[0] = cos(toRadians(yaw)) * cos(toRadians(pitch));
        front[1] = sin(toRadians(pitch));
        front[2] = sin(toRadians(yaw)) * cos(toRadians(pitch));
        cameraFront = front.normalize();

        lastX = x;
        lastY = y;
    } 
    else if (moving) 
    {
        float xOffset = x - lastX;
        float yOffset = lastY - y;

        cameraRight = cameraFront.cross(cameraUp).normalize();
        cameraUp = cameraRight.cross(cameraFront).normalize();

        cameraPos = cameraPos - cameraRight * (MOVE_SPEED * xOffset);    
        cameraPos = cameraPos - cameraUp * (MOVE_SPEED * yOffset);

        lastX = x;
        lastY = y;
    }
    else if (zooming) 
    {
        float zoomOffset = lastZoom - y;
        float zoomSpeed = ZOOM_SPEED * zoomOffset;
        cameraPos = cameraPos + cameraFront * zoomSpeed;

        lastZoom = y;
    }
}