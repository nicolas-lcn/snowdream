#include "transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::mat4 Vengine::Transform::getLocalModelMatrix() 
{
	const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    // Y * X * Z
    const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

    // translation * rotation * scale (also know as TRS matrix)
    return glm::translate(glm::mat4(1.0f), translation) * rotationMatrix * glm::scale(glm::mat4(1.0f), scale);
}