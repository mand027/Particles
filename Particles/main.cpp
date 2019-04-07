//
//  main.cpp
//  Particles
//
//  Created by Armando Hernandez on 3/25/19.
//  Copyright © 2019 Armando Hernandez. All rights reserved.
//
// Please don't change lines 9-31 (It helps me to grade)
#ifdef __APPLE__
// For XCode only: New C++ terminal project. Build phases->Compile with libraries: add OpenGL and GLUT
// Import this whole code into a new C++ file (main.cpp, for example). Then run.
// Reference: https://en.wikibooks.org/wiki/OpenGL_Programming/Installation/Mac
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#endif
#ifdef _WIN32
// For VS on Windows only: Download CG_Demo.zip. UNZIP FIRST. Double click on CG_Demo/CG_Demo.sln
// Run
#include "freeglut.h"
#endif
#ifdef __unix__
// For Linux users only: g++ CG_Demo.cpp -lglut -lGL -o CG_Demo
// ./CG_Demo
// Reference: https://www.linuxjournal.com/content/introduction-opengl-programming
#include "GL/freeglut.h"
#include "GL/gl.h"
#endif

#include <stdio.h>
#include <math.h>
#include <cstdlib>
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include <iostream>
#include "cParticle.hpp"

const int HowManyBAlzzz = 100;
const float cubeSize = 10;
cParticle* p;
cParticle* p1;
cParticle* p2;
cParticle* particles[HowManyBAlzzz];

GLfloat* ka;
GLfloat* kd;
GLfloat* ks;
GLfloat* alpha;
//4D arrays for light constraints
GLfloat* L0pos;
GLfloat* Ia;
GLfloat* Id;
GLfloat* Is;

float randomBetween (float min, float max){
    float force = min + (max - min) * (float)rand() / RAND_MAX;
    return force;
}

void init() // FOR GLUT LOOP
{
    //material plane
    ka = new GLfloat[4];
    ka[0] = 0.0f; //red
    ka[1] = 0.0f; //green
    ka[2] = 0.1f; //blue
    ka[3] = 1.0f; //alpha
    
    kd = new GLfloat[4];
    kd[0] = 0.25f;
    kd[1] = 0.15f;
    kd[2] = 1.0f;
    kd[3] = 1.0f; //allways 1
    
    ks = new GLfloat[4];
    ks[0] = 0.2f;
    ks[1] = 0.2f;
    ks[2] = 0.3f;
    ks[3] = 1.0f;
    
    alpha = new GLfloat[1];
    alpha[0] = 50.0f;
    
    // minX, maxX, minY, maxY, minZ, maxZ
    float* bounds = new float[6];
    bounds[0] = 0;
    bounds[1] = 0;
    bounds[2] = 7;
    bounds[3] = 7;
    bounds[4] = 0;
    bounds[5] = 0;
    
    for(int i = 0; i < HowManyBAlzzz; i++)
    {
        
        p = new cParticle(bounds, 5, 0.3f);  // position, mass, radius
        p->forces[1] += p->mass * -9.81f;
        p->forces[0] += randomBetween(-1000, 1000);
        p->forces[2] += randomBetween(-1000, 1000);
        p->dragForce[1] += p->area;
        particles[i] = p;
    }

    //light begins
    L0pos = new GLfloat[4];
    L0pos[0] = 5.0f;
    L0pos[1] = 5.0f;
    L0pos[2] = 5.0f;
    L0pos[3] = 1.0f;
    //l0pos [3] == 1 point light
    //L0pos [3] == 0 directional light
    
    Ia = new GLfloat[4];
    Id = new GLfloat[4];
    Is = new GLfloat[4];
    for(int i =0; i<4; i++){
        Ia[i] = 1.0f;
        Id[i] = 1.0f;
        Is[i] = 1.0f;
    }
    //light is bright
    
    glEnable(GL_LIGHTING); //glcolor dies with this
    glEnable(GL_LIGHT0); //up to 8
    
    glLightfv(GL_LIGHT0, GL_POSITION, L0pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, Ia);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Id);
    glLightfv(GL_LIGHT0, GL_SPECULAR, Is);
    
    glEnable(GL_DEPTH_TEST);                                         // Enable check for close and far objects.
    glClearColor(0.0, 0.0, 0.0, 0.0);                                // Clear the color state.
    glMatrixMode(GL_MODELVIEW);                                      // Go to 3D mode.
    glLoadIdentity();                                                // Reset 3D view matrix.
}

void display()                                                       // Called for each frame (about 60 times per second).
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);              // Clear color and depth buffers.
    glLoadIdentity();                                                // Reset 3D view matrix.
    gluLookAt(0.0, 15.0, 45.0,                                        // Where the camera is.
              0.0, 0.0, 0.0,                                         // To where the camera points at.
              0.0, 1.0, 0.0);                                        // "UP" vector.
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ka);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, kd);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ks);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, alpha);
    
    glBegin(GL_POLYGON);                // Draw A Quad
    glVertex3f(-15.0f, -10.0f, -15.0f);    // Top Left
    glVertex3f( 15.0f, -10.0f, -15.0f);    // Top Right
    glVertex3f( 15.0f, -10.0f, 15.0f);    // Bottom Right
    glVertex3f(-15.0f, -10.0f, 15.0f);    // Bottom Left
    glEnd();
    
    for(int i = 0; i < HowManyBAlzzz; i++)
    {
        particles[i]->Draw();
    }
    
    glutSwapBuffers();                                               // Swap the hidden and visible buffers.
}

void reshape(int x, int y)                                           // Called when the window geometry changes.
{
    glMatrixMode(GL_PROJECTION);                                     // Go to 2D mode.
    glLoadIdentity();                                                // Reset the 2D matrix.
    gluPerspective(40.0, (GLdouble)x / (GLdouble)y, 0.5, 80.0);      // Configure the camera lens aperture.
    glMatrixMode(GL_MODELVIEW);                                      // Go to 3D mode.
    glViewport(0, 0, x, y);                                          // Configure the camera frame dimensions.
    gluLookAt(0.0, 0.0, 4.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);
    display();
}

void idle()                                                          // Called when drawing is finished.
{
    
    for(int i = 0; i < HowManyBAlzzz; i++)
    {
        bool coshado = false;
        particles[i]->forces[1] += p->mass * -9.81f;
        particles[i]->IntegrateVelvet(0.001f);
        particles[i]->CheckCollision(cubeSize);
        p1 = particles[i];
        
        for(int j = 0; j < HowManyBAlzzz; j++){
            p2 = particles[j];
            
            if( i!= j && p1->OnCollisionParticle(p2))
            {
                coshado = true;
                p1->ka[0] = 1.0f;
                p1->ka[1] = 0.0f;
                p1->ka[2] = 0.0f;

                p2->ka[0] = 1.0f;
                p2->ka[1] = 0.0f;
                p2->ka[2] = 0.0f;
            }
        }
        if(!coshado){
            p1->ka[0] = p1->originalKa[0];;
            p1->ka[1] = p1->originalKa[1];
            p1->ka[2] = p1->originalKa[2];
        }
    }
    
    glutPostRedisplay();                                             // Display again.
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);                                           // Init GLUT with command line parameters.
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);        // Use 2 buffers (hidden and visible). Use the depth buffer. Use 3 color channels.
    glutInitWindowSize(1000, 800);
    glutCreateWindow("CG first program");
    
    init();
    glutReshapeFunc(reshape);                                        // Reshape CALLBACK function.
    glutDisplayFunc(display);                                        // Display CALLBACK function.
    glutIdleFunc(idle);                                              // Idle CALLBACK function.
    glutMainLoop();                                                  // Begin graphics program.
    return 0;                                                        // ANSI C requires a return value.
}

