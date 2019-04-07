//
//  cParticle.hpp
//  Particles
//
//  Created by Armando Hernandez on 3/25/19.
//  Copyright © 2019 Armando Hernandez. All rights reserved.
//// Please don't change lines 9-31 (It helps me to grade)
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
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#ifndef cParticle_hpp
#define cParticle_hpp

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <cstdlib>

#endif /* cParticle_hpp */

class cParticle{
public:
    //variables
    float* position;
    float* oldPos;
    float mass;
    float area;
    float restitutionCoeficient;
    float radius;
    float* forces;
    float* dragForce;
    
    //material
    GLfloat* originalKa;
    GLfloat* ka;
    GLfloat* kd;
    GLfloat* ks;
    
    GLfloat* alpha;
    
    cParticle(float* p, float m, float r);
    ~cParticle();
    void Draw();
    void Update();
    void IntegrateVelvet(float dt);
    void CheckCollision(float cubeSize);
    float dotProduct(float* v1, float* v2);
    float* vectorTimesScalar(float* v, float s);
    float* substractVectors(float* v1, float* v2);
    bool OnCollisionParticle(cParticle* other);
};
