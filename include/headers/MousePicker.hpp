#pragma once

#include "Model.hpp"

class MousePicker
{
public:
    MousePicker(glm::mat4 projection, glm::mat4 view);

    void Update(glm::mat4 view, glm::vec2 windowSize, glm::vec2 cursorPosition);

    glm::vec3 getCurrentRay()
    {
        return currentRay;
    }
        
private:
    glm::vec3 currentRay;

    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;

    glm::vec3 calculateMouseRay(glm::vec2 windowSize, glm::vec2 cursorPosition);
    glm::vec4 toEyeSpace(glm::vec4 clipCoords);
    glm::vec3 toWorldCoords(glm::vec4 eyeCoords);
};