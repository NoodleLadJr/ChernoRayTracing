#pragma once

#include "Walnut/Image.h"
#include <memory>
#include "glm/glm.hpp"

class Renderer
{
public:
	Renderer() = default;
	
	void Render();

	void OnResize(uint32_t width, uint32_t height);

	std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }

	//ImGui properties
	float defaultColor[3] = {0.0f, 0.4f, 1.0f};
	float bgColor[3] = { 0.2f, 0.2f, 0.2f };
	float radius = 0.5f;

private:
	glm::vec4 PerPixel(glm::vec2 coord);

	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;

};

