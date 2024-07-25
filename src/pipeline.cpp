#include "pipeline.h"

#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cassert>



std::vector<char> Vengine::Pipeline::loadShader(const std::string& filePath) 
{
	std::ifstream file(filePath, std::ios::ate | std::ios::binary);

	if(!file.is_open())
	{
		throw std::runtime_error("Failed to open file : " + filePath);

	}

	size_t fileSize = static_cast<size_t>(file.tellg());
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}



Vengine::Pipeline::Pipeline(Vengine::Device& device, const std::string& vertexFilePath, const std::string& fragmentFilePath, const Vengine::ConfigurationInfo& configInfo): 
device(device)
{
	createGraphicsPipeline(vertexFilePath, fragmentFilePath, configInfo);
}


Vengine::Pipeline::~Pipeline() 
{
	vkDestroyShaderModule(device.device(), vertexModule, nullptr);
	vkDestroyShaderModule(device.device(), fragmentModule, nullptr);

	vkDestroyPipeline(device.device(), graphicsPipeline, nullptr);
}

void Vengine::Pipeline::bind(VkCommandBuffer commandBuffer) 
{
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
}

void Vengine::Pipeline::createGraphicsPipeline(const std::string& vertexFilePath, const std::string& fragmentFilePath, const ConfigurationInfo& configInfo) 
{

	assert(configInfo.pipelineLayout != VK_NULL_HANDLE);
	assert(configInfo.renderPass != VK_NULL_HANDLE);

	auto vertexChars = loadShader(vertexFilePath);
	auto fragmentChars = loadShader(fragmentFilePath);

	createShaderModule(vertexChars, &vertexModule);
	createShaderModule(fragmentChars, &fragmentModule);
 
	VkPipelineShaderStageCreateInfo shaderStages[2];
	shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
	shaderStages[0].module = vertexModule;
	shaderStages[0].pName = "main";
	shaderStages[0].flags = 0;
	shaderStages[0].pNext = NULL;
	shaderStages[0].pSpecializationInfo = nullptr;

	shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	shaderStages[1].module = fragmentModule;
	shaderStages[1].pName = "main";
	shaderStages[1].flags = 0;
	shaderStages[1].pNext = NULL;
	shaderStages[1].pSpecializationInfo = nullptr;
	
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexAttributeDescriptionCount = 0;
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.pVertexAttributeDescriptions = nullptr;
	vertexInputInfo.pVertexBindingDescriptions = nullptr;

	VkPipelineViewportStateCreateInfo viewportInfo{};
	viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportInfo.viewportCount = 1;
	viewportInfo.pViewports = &configInfo.viewport;
	viewportInfo.scissorCount = 1;
	viewportInfo.pScissors = &configInfo.scissor;

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &configInfo.inputAssemblyInfo;
	pipelineInfo.pViewportState = &viewportInfo;
	pipelineInfo.pRasterizationState = &configInfo.rasterizationInfo;
	pipelineInfo.pMultisampleState  = &configInfo.multisampleInfo;

	pipelineInfo.pColorBlendState = &configInfo.colorBlendInfo;
	pipelineInfo.pDepthStencilState = &configInfo.depthStencilInfo;
	pipelineInfo.pDynamicState = nullptr;

	pipelineInfo.layout = configInfo.pipelineLayout;
	pipelineInfo.renderPass = configInfo.renderPass;
	pipelineInfo.subpass = configInfo.subpass;

	pipelineInfo.basePipelineIndex = -1;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	if(vkCreateGraphicsPipelines(device.device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
	{
		throw std::runtime_error("Unable to create graphics pipeline");
	}

}

void Vengine::Pipeline::createShaderModule(const std::vector<char> &buffer, VkShaderModule *module) 
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = buffer.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());
	if(vkCreateShaderModule(device.device(), &createInfo, nullptr, module) != VK_SUCCESS)
	{
		throw std::runtime_error("Unable to create Shader Module");
	}
}

Vengine::ConfigurationInfo Vengine::Pipeline::defaultConfig(uint32_t width, uint32_t height) 
{
	Vengine::ConfigurationInfo configurationInfo{};
  	configurationInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;	
  	configurationInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	configurationInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
	configurationInfo.inputAssemblyInfo.pNext = NULL;
	configurationInfo.inputAssemblyInfo.flags = 0;


	configurationInfo.viewport.x = 0.0f;
	configurationInfo.viewport.y = 0.0f;
	configurationInfo.viewport.width = static_cast<float>(width);
	configurationInfo.viewport.height = static_cast<float>(height);
	configurationInfo.viewport.minDepth = 0.0f;
	configurationInfo.viewport.maxDepth = 1.0f;

	configurationInfo.scissor.offset = {0, 0};
	configurationInfo.scissor.extent = {width, height}; 

	configurationInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	configurationInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
	configurationInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
	configurationInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
	configurationInfo.rasterizationInfo.lineWidth = 1.0f;
	configurationInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
	configurationInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	configurationInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
	configurationInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
	configurationInfo.rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
	configurationInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional
	configurationInfo.rasterizationInfo.pNext = NULL;
	configurationInfo.rasterizationInfo.flags = 0;

	configurationInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	configurationInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
	configurationInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	configurationInfo.multisampleInfo.minSampleShading = 1.0f;           // Optional
	configurationInfo.multisampleInfo.pSampleMask = nullptr;             // Optional
	configurationInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
	configurationInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional

	configurationInfo.colorBlendAttachment.colorWriteMask =
	  VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
	  VK_COLOR_COMPONENT_A_BIT;
	configurationInfo.colorBlendAttachment.blendEnable = VK_FALSE;
	configurationInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
	configurationInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
	configurationInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
	configurationInfo.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
	configurationInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
	configurationInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional

	configurationInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	configurationInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
	configurationInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
	configurationInfo.colorBlendInfo.attachmentCount = 1;
	configurationInfo.colorBlendInfo.pAttachments = &configurationInfo.colorBlendAttachment;
	configurationInfo.colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
	configurationInfo.colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
	configurationInfo.colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
	configurationInfo.colorBlendInfo.blendConstants[3] = 0.0f;  // Optional

	configurationInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	configurationInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
	configurationInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
	configurationInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
	configurationInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
	configurationInfo.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
	configurationInfo.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
	configurationInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
	configurationInfo.depthStencilInfo.front = {};  // Optional
	configurationInfo.depthStencilInfo.back = {};   // 

	return configurationInfo;
}

