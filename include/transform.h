#ifndef TRANSFORM_H
#define TRANSFORM_H

// Include glm utils
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Vengine
{
	class Transform
	{
		Transform() = default;
		~Transform(){};

	public:

		glm::mat4 getLocalModelMatrix();

	private:
		//Local space Information
		glm::vec3 translation  = {0.0f, 0.0f, 0.0f};
		glm::vec3 scale = {1.0f, 1.0f, 1.0f};
		glm::vec3 rotation = {0.0f, 0.0f, 0.0}; //euler angle in degrees

		// Global space information
		glm::mat4 modelMatrix = glm::mat4(1.0f);

	};
}

#endif // TRANSFORM_H