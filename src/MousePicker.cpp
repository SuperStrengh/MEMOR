#include "MousePicker.hpp"

MousePicker::MousePicker(glm::mat4 projection, glm::mat4 view)
{
    projectionMatrix = projection;
    viewMatrix = view;
}

glm::vec3 MousePicker::calculateMouseRay(glm::vec2 windowSize, glm::vec2 cursorPosition)
{
    float MouseX = (2.0f * cursorPosition.x) / windowSize.x - 1;
    float MouseY = -((2.0f * cursorPosition.y) / windowSize.y - 1);

    glm::vec4 clipCoords = glm::vec4(MouseX, MouseY, -1.0f, 1.0f);
    glm::vec4 eyeCoords = toEyeSpace(clipCoords);
    glm::vec3 WorldRay = toWorldCoords(eyeCoords);

    return WorldRay;
}

glm::vec4 MousePicker::toEyeSpace(glm::vec4 clipCoords)
{
    glm::mat4 invertedProjection = glm::inverse(projectionMatrix);
    glm::vec4 eyeCoords = invertedProjection * clipCoords;

    return glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
}

glm::vec3 MousePicker::toWorldCoords(glm::vec4 eyeCoords)
{
    glm::mat4 invertedView = glm::inverse(viewMatrix);
    glm::vec4 rayWorld = invertedView * eyeCoords;
    glm::vec3 mouseRay = glm::normalize(glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z));

    return mouseRay;
}

void MousePicker::Update(glm::mat4 view, glm::vec2 windowSize, glm::vec2 cursorPosition)
{
    viewMatrix = view;
    currentRay = calculateMouseRay(windowSize, cursorPosition);
}