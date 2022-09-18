#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"
#include "Renderer.h"
#include "Camera.h"


using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:

	ExampleLayer()
		: m_Camera(45.0f, 0.1f, 100.0f) {}

	virtual void OnUpdate(float ts) override
	{
		m_Camera.OnUpdate(ts);
	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last Render Time: %.3fms", m_LastRenderTime);
		if (ImGui::Button("Render")) 
		{
			Render();
		}

		ImGui::SliderFloat("Radius", &m_Renderer.radius, 0.2f, 1.5f, "%f",1.0f );
		ImGui::ColorEdit3("Color", m_Renderer.defaultColor);
		ImGui::ColorEdit3("Background", m_Renderer.bgColor);


		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f,0.0f));
		ImGui::Begin("Viewport");

		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;

		auto image = m_Renderer.GetFinalImage();

		if (image)
		{
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(),(float)image->GetHeight()}, ImVec2(0,1), ImVec2(1,0));
		}

		ImGui::End();
		ImGui::PopStyleVar();

		Render();

	}

	void Render()
	{
		Timer timer;

		m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Camera.OnResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render(m_Camera);

		m_LastRenderTime = timer.ElapsedMillis();
	}

private:
	Renderer m_Renderer;
	Camera m_Camera;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
	uint32_t* m_ImageData = nullptr;
	float m_LastRenderTime;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Height = 576;
	spec.Width = 720;
	spec.Name = "Ray Tracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}