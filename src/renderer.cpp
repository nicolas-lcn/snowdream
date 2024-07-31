#include "renderer.h"

Vengine::Renderer::Renderer(Window &window, Device& device) :
window(window),
device(device)
{
	recreateSwapChain();
	createCommandBuffers();
}

Vengine::Renderer::~Renderer() 
{
	freeCommandBuffers();
}

void Vengine::Renderer::createCommandBuffers() 
{
	commandBuffers.resize(swapChain->imageCount());
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = device.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

	if(vkAllocateCommandBuffers(device.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		throw std::runtime_error("Failed to allocate command buffers");
}

void Vengine::Renderer::freeCommandBuffers() 
{
	vkFreeCommandBuffers(device.device(), device.getCommandPool(), static_cast<float>(commandBuffers.size()), commandBuffers.data());
	commandBuffers.clear();
}


void Vengine::Renderer::recreateSwapChain() 
{
	auto extent = window.getExtent();
	while(extent.width == 0 || extent.height == 0)
	{
		extent = window.getExtent();
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(device.device());

	if(swapChain ==  nullptr)
	{
		swapChain = std::make_unique<SwapChain>(device, extent);
	}
	else{
		swapChain = std::make_unique<SwapChain>(device, extent, std::move(swapChain));
		if(swapChain->imageCount() != commandBuffers.size())
		{
			freeCommandBuffers();
			createCommandBuffers();
		}
	}
	
}

VkCommandBuffer Vengine::Renderer::beginFrame() 
{
	assert(!isFrameStarted);
	auto result = swapChain->acquireNextImage(&currentImageIndex);

	if(result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		recreateSwapChain();
		return nullptr;
	}

	if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		throw std::runtime_error("Failed to acquire swap chain image");

	isFrameStarted = true;
	auto commandBuffer = getCurrentCommandBuffer();
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if(vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		throw std::runtime_error("Failed to begin record command buffer");

	return commandBuffer;
}

void Vengine::Renderer::endFrame() 
{
	assert(isFrameStarted);
	auto commandBuffer = getCurrentCommandBuffer();

	if(vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to record command buffer");
	}

	auto result = swapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
	
	if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR  || window.isResized())
	{
		window.resetFlags();
		recreateSwapChain();
	}

	if(result != VK_SUCCESS)
		throw std::runtime_error("Failed to present swap chain image");

	isFrameStarted = false;
}

void Vengine::Renderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) 
{
	assert(isFrameStarted);
	assert(commandBuffer == getCurrentCommandBuffer());
	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = swapChain->getRenderPass();
	renderPassInfo.framebuffer = swapChain->getFrameBuffer(currentImageIndex);

	renderPassInfo.renderArea.offset = {0, 0};
	renderPassInfo.renderArea.extent = swapChain->getSwapChainExtent();

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = {0.1f, 0.1f, 0.1f, 0.1f};
	clearValues[1].depthStencil = {1.0f, 0};

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapChain->getSwapChainExtent().width);
	viewport.height = static_cast<float>(swapChain->getSwapChainExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor{{0, 0}, swapChain->getSwapChainExtent()};
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void Vengine::Renderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) 
{
	assert(isFrameStarted);
	assert(commandBuffer == getCurrentCommandBuffer());

	vkCmdEndRenderPass(commandBuffer);

}

