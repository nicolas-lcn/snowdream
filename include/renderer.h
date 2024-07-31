#ifndef RENDERER_H
#define RENDERER_H

#include "window.h"
#include "swapchain.h"

#include <memory>
#include <vector>

namespace Vengine
{
	class Renderer
	{
	public:
		Renderer(Window &window, Device& device);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer &operator=(const Renderer&) = delete;

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

		inline bool isFrameInProgress() const {return isFrameStarted; }
		inline VkCommandBuffer getCurrentCommandBuffer() const 
		{
			assert(isFrameStarted);
			return commandBuffers[currentImageIndex];
		}
		inline VkRenderPass getSwapChainRenderPass() const {return swapChain->getRenderPass();}


	private:
		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();

		Window& window;
		Device& device;
		std::unique_ptr<SwapChain> swapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex;
		bool isFrameStarted;
	};
}

#endif // RENDERER_H