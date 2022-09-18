#include "Renderer.h"
#include "Walnut/Random.h"

namespace Utils
{
	static uint32_t ConvertToRGBA(const glm::vec4& color)
	{
		uint8_t r = (uint8_t)(color.r * 255.0f);
		uint8_t g = (uint8_t)(color.g * 255.0f);
		uint8_t b = (uint8_t)(color.b * 255.0f);
		uint8_t a = (uint8_t)(color.a * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}
}

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

glm::vec4 Renderer::TraceRay(const Ray& ray)
{

	// (bx^2 + by^2)t^2 + (2axbx + 2ayby)t + ax^2 + ay^2 -r^2) = 0
	float a = glm::dot(ray.Direction, ray.Direction);
	float b = 2 * glm::dot(ray.Origin, ray.Direction);
	float c = glm::dot(ray.Origin, ray.Origin) - radius * radius;

	float discriminant = b * b - 4.0 * a * c;



	if (discriminant < 0.0f)
	{
		return glm::vec4(bgColor[0], bgColor[1], bgColor[2], 1.0f);
	}

	float t0 = (-b + glm::sqrt(discriminant)) / (2 * a);
	float closestPoint = (-b - glm::sqrt(discriminant)) / (2 * a);

	glm::vec3 hitPoint = ray.Origin + ray.Direction * closestPoint;

	glm::vec3 sphereColor = glm::vec3(defaultColor[0], defaultColor[1], defaultColor[2]);
	glm::vec3 normal = glm::normalize(hitPoint);

	glm::vec3 lightDir = glm::normalize(glm::vec3( - 1, -1, -1));

	float d = glm::max(0.0f,glm::dot(normal, -lightDir));

	sphereColor *= d;
	return glm::vec4(sphereColor, 1.0f);
}

void Renderer::Render(const Camera& Camera)
{

	

	Ray ray;
	ray.Origin = Camera.GetPosition();

	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
	
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			ray.Direction = Camera.GetRayDirections()[x + y * m_FinalImage->GetWidth()];

			glm::vec4 color = TraceRay(ray);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(color);

		}
	}


	m_FinalImage->SetData(m_ImageData);
}
