#include "Renderer.h"
#include "Walnut/Random.h"

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
		{
			return;
		}

		m_FinalImage->Resize(width, height);
	}
	else
	{
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];
}

uint32_t Renderer::PerPixel(glm::vec2 coord)
{
	uint8_t r = (uint8_t)(coord.x * 255.0f);
	uint8_t g = (uint8_t)(coord.y * 255.0f);

	glm::vec3 rayOrigin(0.0f,0.0f, 2.0f);
	glm::vec3 rayDirection(coord.x, coord.y, -1.0f);
	float radius = 0.5f;
	glm::vec3 sphereOrigin = glm::vec3(0.0f);
	glm::vec3 lightDir = glm::vec3(-1.0f, -1.0f, 1.0f);

	lightDir = glm::normalize(lightDir);

	// (bx^2 + by^2)t^2 + (2axbx + 2ayby)t + ax^2 + ay^2 -r^2) = 0
	float a = glm::dot(rayDirection, rayDirection);
	float b = 2 * glm::dot(rayOrigin, rayDirection);
	float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

	float discriminant = b * b - 4.0 * a * c;
	float light = 1.0f;
	if (discriminant >= 0.0f)
	{
		float t[] = { (-b - glm::sqrt(discriminant)) / (2.0f * a),
					(-b + glm::sqrt(discriminant)) / (2.0f * a) };

		for (int i = 0; i < 2; i++)
		{
			glm::vec3 hitPosition = rayOrigin + rayDirection * t[i];
			glm::vec3 normal = hitPosition - sphereOrigin;
			glm::normalize(normal);
			glm::vec3 col = glm::vec3(normal * 0.5f + 0.5f);
			light = glm::max(glm::dot(normal, -lightDir),0.0f);
			//return 0xff000000 | (uint8_t)col.z << 16 | (uint8_t)col.y << 8 | (uint8_t)col.x;

		}

		return 0xffffcc55 * light;
	}

	return 0xff333333;
}

void Renderer::Render()
{

	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
	
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			glm::vec2 coord = { (float)x / (float)m_FinalImage->GetWidth(), (float)y / (float)m_FinalImage->GetHeight() };
			coord = coord * 2.0f - 1.0f;
			m_ImageData[x + y * m_FinalImage->GetWidth()] = PerPixel(coord);

		}
	}


	m_FinalImage->SetData(m_ImageData);
}
