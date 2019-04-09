//
//  cParticle.cpp
//  Particles
//
//  Created by Armando Hernandez on 3/25/19.
//  Copyright © 2019 Armando Hernandez. All rights reserved.
//
#include "math.h"
#include "cParticle.hpp"

cParticle::cParticle(float* bounds, float m, float r)
{
    float minKd = 0.3f;
    float maxKd = 1.0f;
    float minKa = 0.05f;
    float maxKa = 0.2f;
    float minKs = 0.4f;
    float maxKs = 0.8f;
    
    restitutionCoeficient = 0.65f;
    radius = r;
    mass = m;
    area = 4.0f * M_PI * radius*radius;
    
    //material begins
    ka = new GLfloat[4];
    originalKa = new GLfloat[3];
    
    originalKa [0] = 0.1f + (0.3f - 0.1f) * (float)rand() / RAND_MAX;
    ka[0] = originalKa[0]; //red
    
    originalKa [1] = minKa + (maxKa - minKa) * (float)rand() / RAND_MAX;
    ka[1] =  originalKa [1]; //green
    
    originalKa [2] = minKa + (maxKa - minKa) * (float)rand() / RAND_MAX;
    ka[2] = originalKa [2]; //blue
    
    ka[3] = 1.0f; //alpha
    
    kd = new GLfloat[4];
    kd[0] = minKd + (maxKd - minKd) * (float)rand() / RAND_MAX;
    kd[1] = minKd + (maxKd - minKd) * (float)rand() / RAND_MAX;
    kd[2] = minKd + (maxKd - minKd) * (float)rand() / RAND_MAX;
    kd[3] = 1.0f; //allways 1
    
    ks = new GLfloat[4];
    ks[0] = minKs + (maxKs - minKs) * (float)rand() / RAND_MAX;
    ks[1] = minKs + (maxKs - minKs) * (float)rand() / RAND_MAX;;
    ks[2] = minKs + (maxKs - minKs) * (float)rand() / RAND_MAX;;
    ks[3] = 1.0f;
    
    alpha = new GLfloat[1];
    alpha[0] = 50.0f;
    // material ends
    
    //bounds minX maxX miny maxy minz maxz
    float x = bounds[0] + (bounds[1] - bounds[0]) * (float)rand() / RAND_MAX;
    float y = bounds[2] + (bounds[3] - bounds[2]) * (float)rand() / RAND_MAX;
    float z = bounds[4] + (bounds[5] - bounds[4]) * (float)rand() / RAND_MAX;
    
    position = new float[3];
    position[0] = x;
    position[1] = y;
    position[2] = z;
    
    oldPos = new float[3];
    oldPos[0] = position[0];
    oldPos[1] = position[1];
    oldPos[2] = position[2];
    
    forces = new float[3];
    forces[0] = forces[1] = forces[2] = 0;
    
    dragForce = new float[3];
    dragForce[0] = dragForce[1] = dragForce[2] = 0;
    
}

void cParticle::IntegrateVelvet(float dt)
{
    float* temp = new float[3];
    temp[0] = position[0];
    temp[1] = position[1];
    temp[2] = position[2];
    
    float* accel = new float[3];
    accel[0] = (dragForce[0] + forces[0]) / mass;
    accel[1] = (dragForce[1] + forces[1]) / mass;
    accel[2] = (dragForce[2] + forces[2]) / mass;

    position[0] += position[0] - oldPos[0] + accel[0] *dt *dt;
    position[1] += position[1] - oldPos[1] + accel[1] *dt *dt;
    position[2] += position[2] - oldPos[2] + accel[2] *dt *dt;
    
    oldPos[0] = temp[0];
    oldPos[1] = temp[1];
    oldPos[2] = temp[2];

}

void cParticle::Draw()
{
    glPushMatrix();
    {
        glTranslatef(position[0], position[1], position[2]);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ka);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, kd);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ks);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, alpha);
        glutSolidSphere(radius, 20, 20);
    }
    glPopMatrix();
}

void cParticle::CheckCollision(float cubeSize)
{
    
//    float* normal = new float[3];
//
//    if (position[1] + radius <= -1 * cubeSize || position[1] + radius >= cubeSize) {
//        // Checar porqué entró al if
//        bool what = position[1] + radius >= cubeSize ? true : false;
//
//        // Si entró por chocar con la tapa de arriba
//        if (what) {
//            normal[0] = normal[2] = 0;
//            normal[1] = -1;
//        } else {
//            normal[0] = normal[2] = 0;
//            normal[1] = 1;
//        }
//
//        float* helper = vectorTimesScalar(vectorTimesScalar(substractVectors(normal, forces), 2 * dotProduct(normal, forces)), restitutionCoeficient);
//        position[1] = what ? position[1] - radius : position[1] + radius;
//        forces[1] = helper[1];
//        dragForce[1] = -dragForce[1];
//    }
    
    if (position[0] + radius <= -cubeSize || position[0] + radius >= cubeSize) {
        bool what = position[0] + radius >= cubeSize ? true : false;
        
        position[0] = oldPos[0]; //what ? cubeSize - radius : -cubeSize + radius;
        forces[0] = -forces[0] * restitutionCoeficient;
        dragForce[0] = -dragForce[0];
    }
    
    if (position[1] + radius <= -cubeSize || position[1] + radius >= cubeSize)
    {
        bool what = position[1] + radius >= cubeSize ? true : false;
        
        position[1] = oldPos[1]; //what ? cubeSize - radius : -cubeSize + radius;
        forces[1] = -forces[1] * restitutionCoeficient;
        dragForce[1] = -dragForce[1];
    }
    
    if (position[2] + radius <= -cubeSize || position[2] + radius >= cubeSize) {
        bool what = position[2] + radius >= cubeSize ? true : false;
        
        position[2] = oldPos[2]; //what ? cubeSize - radius : -cubeSize + radius;
        forces[2] = -forces[2] * restitutionCoeficient;
        dragForce[2] = -dragForce[2];
    }
    
}

float cParticle::dotProduct(float *v1, float *v2) {
    return (v1[0] + v2[0]) * (v1[1] + v2[1]) * (v1[2] + v2[2]);
}

float* cParticle::vectorTimesScalar(float *v, float s) {
    float* result = new float[3];
    result[0] = v[0] * s;
    result[1] = v[1] * s;
    result[2] = v[2] * s;
    return result;
}

float* cParticle::substractVectors(float *v1, float *v2) {
    float* result = new float[3];
    result[0] = v1[0] - v2[0];
    result[1] = v1[1] - v2[1];
    result[2] = v1[2] - v2[2];
    return result;
}

bool cParticle::OnCollisionParticle(cParticle* other)
{
    bool chokado = false;
    
    float sumRadius = radius + other->radius;
    
    float dx = other->position[0] - position[0];
    dx *= dx;
    float dy = other->position[1] - position[1];
    dy *= dy;
    float dz = other->position[2] - position[2];
    dz *= dz;
    
    float sumR2 = sumRadius * sumRadius;
    float dist2 = dx+dy+dz;
    if( dist2 < sumR2){
        chokado = true;
        
        for(int i = 0; i < 3; i++){
            float va = position[i] - oldPos[i];
            float vb = other->position[i] - other->oldPos[i];
            if(va*vb < 0){
                forces[i] = -forces[i];
                other->forces[i] = -other->forces[i];
            }
            if(va<0){
                position[i] += 0.01f;
                other->position[i] -= 0.01f;
            }
            else{
                position[i] -= 0.01f;
                other->position[i] += 0.01f;
            }
        }
    }
    else{
        chokado = false;
    }
    
    return chokado;
}
