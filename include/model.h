#ifndef MODEL_H
#define MODEL_H

#include "device.h"

// std
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>


namespace Vengine
{

	class Model
	{
	public:
		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 color;

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
		};

		Model(Vengine::Device &device, const std::vector<Vertex>& vertices);
		~Model();

		Model(const Model&) = delete;
		Model &operator=(const Model&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);
	private:
		void createBuffers(const std::vector<Vertex> vertices);

		Device &device;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;
	};
}


#endif // MODEL_H