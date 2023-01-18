#include "ParticleSystem.hpp"

Particles::Particles(
    float LifeTime,
    float SpawnTime,
    std::string textureName,   
    glm::vec3 StartPosition, 
    glm::vec3 Velocity,
    glm::vec4 StartColor,
    glm::vec4 EndColor,

    glm::vec2 StartSize,
    glm::vec2 EndSize
    )
{
    ParcStartPosition = StartPosition; 
    ParcVelocity = Velocity;
    ParcStartColor = StartColor;
    ParcEndColor = EndColor;
    ParcLifeTime = LifeTime;
    ParcSpawnTime = SpawnTime;

    ParcStartSize = StartSize;
    ParcEndSize = EndSize;

    ParcTexture.Delete();

    ParcTexture = Texture(textureName);
}

void Particles::DrawParticles(glm::mat4 matrix, glm::vec3 LookDirection)
{
    float ElapsedTime = glutGet(GLUT_ELAPSED_TIME);

    glDisable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);

    if(ElapsedTime - crntTime >= ParcSpawnTime * 1000 && Enabled == true)
    {
        crntTime = ElapsedTime;

        Particle particle {
        ParcStartPosition, 
        ParcVelocity,
        ParcStartPosition + ParcVelocity * (ParcLifeTime / 10),
        ParcStartColor,
        ParcEndColor,

        ParcStartSize,
        ParcEndSize,

        ParcLifeTime,
        ElapsedTime,

        ParcStartPosition,
        ParcStartSize,
        ParcStartColor
        };

        CurrentParticles.push_back(particle);
    }


    for(int i = CurrentParticles.size() - 1; i >= 0; i)
    {
        if(ElapsedTime - CurrentParticles[i].currentTime <= CurrentParticles[i].LifeTime * 1000)
        {
            glm::vec4 SColor = CurrentParticles[i].StartColor;
            glm::vec4 EColor = CurrentParticles[i].EndColor;

            glm::vec2 SSize = CurrentParticles[i].StartSize;
            glm::vec2 ESize = CurrentParticles[i].EndSize;

            //Color Interpolation

            CurrentParticles[i].CurrentColor.x = SColor.x + ((EColor.x - SColor.x) * ((ElapsedTime -  CurrentParticles[i].currentTime) / (CurrentParticles[i].LifeTime * 1000)));
            CurrentParticles[i].CurrentColor.y = SColor.y + ((EColor.y - SColor.y) * ((ElapsedTime -  CurrentParticles[i].currentTime) / (CurrentParticles[i].LifeTime * 1000)));
            CurrentParticles[i].CurrentColor.z = SColor.z + ((EColor.z - SColor.z) * ((ElapsedTime -  CurrentParticles[i].currentTime) / (CurrentParticles[i].LifeTime * 1000)));
            CurrentParticles[i].CurrentColor.w = SColor.w + ((EColor.w - SColor.w) * ((ElapsedTime -  CurrentParticles[i].currentTime) / (CurrentParticles[i].LifeTime * 1000)));

            //Size Interpolation

            CurrentParticles[i].CurrentSize.x = SSize.x + ((ESize.x - SSize.x) * ((ElapsedTime -  CurrentParticles[i].currentTime) / (CurrentParticles[i].LifeTime * 1000)));
            CurrentParticles[i].CurrentSize.y = SSize.y + ((ESize.y - SSize.y) * ((ElapsedTime -  CurrentParticles[i].currentTime) / (CurrentParticles[i].LifeTime * 1000)));

            //Velocity

            
            glm::vec3 VelData = CurrentParticles[i].StartPosition;
            glm::vec3 VelEndPos = CurrentParticles[i].VelocityEndPosition;

            CurrentParticles[i].CurrentPosition.x = VelData.x + ((VelEndPos.x - VelData.x) * ((ElapsedTime -  CurrentParticles[i].currentTime) / (CurrentParticles[i].LifeTime * 1000)));
            CurrentParticles[i].CurrentPosition.y = VelData.y + ((VelEndPos.y - VelData.y) * ((ElapsedTime -  CurrentParticles[i].currentTime) / (CurrentParticles[i].LifeTime * 1000)));
            CurrentParticles[i].CurrentPosition.z = VelData.z + ((VelEndPos.z - VelData.z) * ((ElapsedTime -  CurrentParticles[i].currentTime) / (CurrentParticles[i].LifeTime * 1000)));


            //Draw Particle

            glLoadMatrixf(glm::value_ptr(matrix * glm::inverse(glm::lookAt(CurrentParticles[i].CurrentPosition, CurrentParticles[i].CurrentPosition - LookDirection, glm::vec3(0.0f, 1.0f, 0.0f)))));
            

            ParcTexture.Bind();


            glColor4f(CurrentParticles[i].CurrentColor.x, CurrentParticles[i].CurrentColor.y, CurrentParticles[i].CurrentColor.z, CurrentParticles[i].CurrentColor.w);


            glBegin(GL_QUADS);

            glTexCoord2f(0.01f, 0.01f);
            glVertex3f(CurrentParticles[i].CurrentSize.x, -CurrentParticles[i].CurrentSize.y, 0.0f);


            glTexCoord2f(0.99f, 0.01f);
            glVertex3f(-CurrentParticles[i].CurrentSize.x, -CurrentParticles[i].CurrentSize.y, 0.0f);


            glTexCoord2f(0.99f, 0.99f);
            glVertex3f(-CurrentParticles[i].CurrentSize.x, CurrentParticles[i].CurrentSize.y, 0.0f);


            glTexCoord2f(0.01f, 0.99f);
            glVertex3f(CurrentParticles[i].CurrentSize.x, CurrentParticles[i].CurrentSize.y, 0.0f);

            glEnd();


            ParcTexture.Unbind();


            glPopMatrix();

            i--;
        }else{
            CurrentParticles.erase(CurrentParticles.begin() + i);

            if(CurrentParticles.size() <= 0)
            {
                i = -1;
            }
        }
    }


    glEnable(GL_LIGHTING);
}