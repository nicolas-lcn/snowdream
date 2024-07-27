#include "model.h"

#include <cassert>
#include <cstring>

Vengine::Model::Model(Vengine::Device &device, const std::vector<Vertex> &vertices) : device(device) 
{
	createBuffers(vertices);
}

Vengine::Model::~Model() 
{
	vkDestroyBuffer(device.device(), vertexBuffer, nullptr);
	vkFreeMemory(device.device(), vertexBufferMemory, nullptr);
}

void Vengine::Model::createBuffers(const std::vector<Vertex> vertices) 
{
	vertexCount = static_cast<uint32_t>(vertices.size());
	assert(vertexCount >= 3);

	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
	device.createBuffer(bufferSize, 
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		vertexBuffer,
		vertexBufferMemory);

	void *data;
	vkMapMemory(device.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
	vkUnmapMemory(device.device(), vertexBufferMemory);
}

void Vengine::Model::draw(VkCommandBuffer commandBuffer) 
{
	vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
}

void Vengine::Model::bind(VkCommandBuffer commandBuffer) 
{
	VkBuffer buffers[] = {vertexBuffer};
	VkDeviceSize offsets[] = {0};
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

std::vector<VkVertexInputBindingDescription> Vengine::Model::Vertex::getBindingDescriptions() 
{
	return {{0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX}};	
}

std::vector<VkVertexInputAttributeDescription> Vengine::Model::Vertex::getAttributeDescriptions() 
{

	return {{0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)},
			{1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)}};
}