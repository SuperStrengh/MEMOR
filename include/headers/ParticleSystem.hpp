#pragma once

#include "Model.hpp"

class Particles
{
public:
    Particles(
    float LifeTime = 5.0f,
    float SpawnTime = 1.0f,

    std::string textureName = "res/gfx/GrassSprite.png",   
    glm::vec3 StartPosition = glm::vec3(0.0f, 0.0f, 0.0f), 
    glm::vec3 Velocity = glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec4 StartColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
    glm::vec4 EndColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),

    glm::vec2 StartSize = glm::vec2(1.0f, 1.0f),
    glm::vec2 EndSize = glm::vec2(1.0f, 1.0f)
    );


    bool Enabled = true;
    bool Show = true;

    void DrawParticles(glm::mat4 matrix = glm::mat4(1.0f), glm::vec3 LookDirection = glm::vec3(0.0f, 25.0f, 45.0f));



    glm::vec3 ParcStartPosition = glm::vec3(0.0f, 0.0f, 0.0f); 
    glm::vec3 ParcVelocity = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec4 ParcStartColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec4 ParcEndColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    glm::vec2 ParcStartSize = glm::vec2(1.0f, 1.0f);
    glm::vec2 ParcEndSize = glm::vec2(1.0f, 1.0f);

    Texture ParcTexture = Texture("res/gfx/GrassSprite.png");

    float ParcLifeTime = 1.0f;
    float ParcSpawnTime = 0.25f;
private:
    float crntTime = 0.0f;


    struct Particle {
    glm::vec3 StartPosition;
    glm::vec3 Velocity;
    glm::vec3 VelocityEndPosition;
    glm::vec4 StartColor;
    glm::vec4 EndColor;

    glm::vec2 StartSize;
    glm::vec2 EndSize;
    

    float LifeTime;
    float currentTime;

    glm::vec3 CurrentPosition;
    glm::vec2 CurrentSize;
    glm::vec4 CurrentColor;
    };

    std::vector <Particle> CurrentParticles;
};