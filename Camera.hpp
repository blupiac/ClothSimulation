#pragma once

#include "Vec3.hpp"
//-----------------------------------------------------------------------------
// Author: Bernard Lupiac
// Created: 01/12/2018
// Based on: https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/7.3.camera_mouse_zoom/camera_mouse_zoom.cpp
//-----------------------------------------------------------------------------


class Camera {
public:
    Camera ();
    virtual ~Camera () {}

    void resize (int W, int H);

    Vec3f getPos ();
    Vec3f getFront ();
    Vec3f getUp ();

    // Connecting typical GLUT events
    void handleMouseClickEvent (int button, int state, int x, int y);
    void handleMouseMoveEvent (int x, int y);
  
private:
    bool rotating, moving, zooming;
    float yaw, pitch, lastX, lastY, lastZoom, fov;

    Vec3f cameraPos, cameraFront, cameraUp, cameraRight, worldUp;
};