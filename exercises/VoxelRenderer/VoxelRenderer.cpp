#include "VoxelRenderer.h"


#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

VoxelRenderer::VoxelRenderer() : m_projection(1.0f)
{
	m_mainWindow = std::make_shared<GLWindow>();
	m_vertexShaderPath = "shaders/vertex.shader";
	m_fragmentShaderPath = "shaders/fragment.shader";
	//m_mainWindow->Initialize();

	Initialize();
}

void VoxelRenderer::Initialize()
{
	m_mainWindow->Initialize();
	InitializeShader();
	InitializeCamera();
	GenerateVoxelList();
}

void VoxelRenderer::InitializeVoxel(glm::vec3 position)
{
	std::shared_ptr<Voxel> obj = std::make_shared<Voxel>();
	obj->CreateVoxel();
	obj->SetPosition(position);
	m_voxelList.push_back(obj);
}

void VoxelRenderer::InitializeShader()
{
	m_shader = std::make_shared<Shader>();
	m_shader->CreateFromFile(m_vertexShaderPath, m_fragmentShaderPath);

	m_uniformModel = m_shader->GetUniformLocation("model");
	m_viewProjectionUniform = m_shader->GetUniformLocation("ViewProjectionMatrix");
	m_uniformColor = m_shader->GetUniformLocation("color");
}

void VoxelRenderer::InitializeCamera()
{
	m_camera = new Camera(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -90.0f, 5.0f, 0.5f);

	std::cout << glGetString(GL_VERSION) << std::endl << glGetString(GL_VENDOR) << std::endl << glGetString(GL_RENDERER);

	m_projection = glm::perspective(glm::radians(45.0f), (GLfloat)m_mainWindow->GetBufferWidth() / (GLfloat)m_mainWindow->GetBufferHeight(), 0.1f, 100.0f);

	//m_shader->UseShader();

	//glUseProgram(0);
}

void VoxelRenderer::GenerateVoxelList()
{
	std::vector<float> heights = CreateHeightMap(voxelCountX, voxelCountZ, glm::ivec2(0, 0));
	std::vector<glm::ivec3> positions;

	//std::cout << std::endl << heights.size();

	//for (int x = 0; x < voxelCountX; x++)
	//{
	//	for (int z = 0; z < voxelCountZ; z++)
	//	{
	//		positions.push_back(glm::ivec3(static_cast<GLint>(x), static_cast<GLint>(abs(heights[x * voxelCountZ + z]) * 10.f), static_cast<GLint>(z)));
	//	}
	//}

	//for (int i = 0; i < positions.size(); i++)
	//{
	//	for (int j = 0; j < positions[i].y; j++)
	//	{
	//		InitializeVoxel(glm::ivec3(positions[i].x, j, positions[i].z));
	//	}
	//}

	InitializeVoxel(glm::ivec3(0.f, 0.f, 0.f));
}

std::vector<float> VoxelRenderer::CreateHeightMap(unsigned int width, unsigned int height, glm::ivec2 coords)
{
	std::vector<float> pixels(height * width);
	for (unsigned int j = 0; j < height; ++j)
	{
		for (unsigned int i = 0; i < width; ++i)
		{
			float x = static_cast<float>(i) / (width - 1) + coords.x;
			float y = static_cast<float>(j) / (height - 1) + coords.y;
			pixels[j * width + i] = stb_perlin_fbm_noise3(x, y, 0.0f, 1.9f, 0.5f, 8) * 1.f;
		}
	}

	return pixels;
}

void VoxelRenderer::RunApplication()
{
	while (!m_mainWindow->GetShouldClose())
	{
		ClearWindow(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

		GLfloat now = glfwGetTime();
		m_deltaTime = now - m_lastTime;
		m_lastTime = now;

		glfwPollEvents();

		UpdateCamera(m_deltaTime);

		m_shader->UseShader();

		for (auto voxel : m_voxelList)
		{
			RenderVoxel(voxel);
		}

		glUseProgram(0);

		m_mainWindow->SwapBuffers();
	}
}

void VoxelRenderer::UpdateCamera(GLfloat deltaTime)
{
	m_camera->KeyControl(m_mainWindow->GetKeys(), deltaTime);
	m_camera->MouseControl(m_mainWindow->GetXChange(), m_mainWindow->GetYChange());
}

void VoxelRenderer::RenderVoxel(std::shared_ptr<Voxel> voxel)
{
	glm::mat4 model(1.0f);

	glm::vec3 pos = voxel->GetPosition();

	model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));
	model = glm::scale(model, glm::vec3(1.f, 1.f, 1.f));
	glUniformMatrix4fv(m_uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(m_viewProjectionUniform, 1, GL_FALSE, glm::value_ptr(m_projection * m_camera->CalculateViewMatrix()));
	//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->CalculateViewMatrix()));
	if (voxel->GetPosition().y > 2)
	{
		glUniform4f(m_uniformColor, 1.f, 1.f, 1.f, 1.f);
	}

	else if (voxel->GetPosition().y >= 1)
	{
		glUniform4f(m_uniformColor, .7f, .4f, 0.f, 1.f);
	}

	else
	{
		glUniform4f(m_uniformColor, 0.f, 1.f, 0.f, 1.f);
	}

	voxel->RenderVoxel();
}

void VoxelRenderer::ClearWindow(glm::vec4 color)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(color.r, color.g, color.b, color.a);
}
