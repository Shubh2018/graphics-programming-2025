#include <iostream>
#include "VoxelRenderer.h"
#include "imgui.h"

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

VoxelRenderer::VoxelRenderer()
{
	m_voxelCountX = 50;
	m_voxelCountZ = 50;
	m_lightColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
	m_lightPosition = glm::vec3(5.f, 6.f, 8.f);

	m_lacunarity = 1.9f;
	m_gain = 0.5f;
	m_octaves = 8;
	m_modifier = 1.f;

	Initialize();
}

VoxelRenderer::VoxelRenderer(int voxelCountX, int voxelCountZ, glm::vec4 lightColor, glm::vec3 lightPosition, float lacunarity, float gain, float octaves, float modifiers)
{
	m_voxelCountX = voxelCountX;
	m_voxelCountZ = voxelCountZ;
	m_lightColor = lightColor;
	m_lightPosition = lightPosition;

	m_lacunarity = lacunarity;
	m_gain = gain;
	m_octaves = octaves;
	m_modifier = modifiers;

	Initialize();
}

void VoxelRenderer::Initialize()
{
	m_vertices = new GLfloat[]
	{
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
		//-.5f,  .5f, -.5f,
		// .5f,  .5f, -.5f,
		//-.5f, -.5f, -.5f,
		// .5f, -.5f, -.5f,

		//-.5f,  .5f,  .5f,
		// .5f,  .5f,  .5f,
		//-.5f, -.5f,  .5f,
		// .5f, -.5f,  .5f 
	};

	m_indices = new GLuint[]
	{
		0, 1, 3,
		0, 3, 2,

		1, 7, 3,
		1, 5, 7,

		0, 6, 4,
		0, 2, 6,

		4, 7, 5,
		4, 6, 7,

		4, 5, 1,
		4, 1, 0,

		6, 3, 7,
		6, 2, 3
	};

	m_vertexShaderPath = "shaders\\vertex.shader";
	m_fragmentShaderPath = "shaders\\fragment.shader";

	m_deltaTime = 0.f;
	m_lastTime = 0.f;

	InitializeWindow();
	GenerateVoxelList();
	InitializeLighting();
}

void VoxelRenderer::InitializeWindow()
{
	m_mainWindow = std::make_unique<GLWindow>(1024, 768);
	m_mainWindow->Initialize();

	//CreateMesh(m_vertices, indices);
	CreateShader();

	m_camera = std::make_unique<Camera>(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 45.0f, 0.0f, 5.0f, 0.5f);

	//std::cout << glGetString(GL_VERSION) << std::endl << glGetString(GL_VENDOR) << std::endl << glGetString(GL_RENDERER);

	m_projection = glm::mat4(1.0f);
	m_projection = glm::perspective(glm::radians(45.0f), (GLfloat)m_mainWindow->GetBufferWidth() / (GLfloat)m_mainWindow->GetBufferHeight(), 0.1f, 100.0f);
}

void VoxelRenderer::GenerateVoxelList()
{
	std::vector<float> heights = CreateHeightMap(m_voxelCountX, m_voxelCountZ, glm::ivec2(0, 0));
	std::vector<glm::ivec3> positions;

	for (int x = 0; x < m_voxelCountX; x++)
	{
		for (int z = 0; z < m_voxelCountZ; z++)
		{
			//std::cout << heights[x * voxelCountZ + z] * 10.f << std::endl;
			//
			positions.push_back(glm::ivec3(static_cast<GLint>(x), static_cast<GLint>(abs(heights[x * m_voxelCountZ + z]) * 10.f), static_cast<GLint>(z)));
			//CreateMesh(m_vertices, indices, glm::ivec3(static_cast<GLint>(x), , static_cast<GLint>(z)));
		}
	}
	
	if (m_meshList.size() > 0)
		m_meshList.clear();

	for (int i = 0; i < positions.size(); i++)
	{
		for (int j = 0; j < positions[i].y; j++)
		{
			glm::vec4 color(0.0f);

			if (j > 2)
			{
				color = glm::vec4(1.f, 1.f, 1.f, 1.f);
			}

			else if (j >= 1)
			{
				color = glm::vec4(.7f, .4f, 0.f, 1.f);
			}

			else
			{
				color = glm::vec4(0.f, 1.f, 0.f, 1.f);
			}

			//std::cout << std::endl << "Creating Mesh: " << positions[i].x << " " << j << " " << positions[i].z;
			CreateMesh(m_vertices, m_indices, glm::ivec3(positions[i].x, j, positions[i].z), color);
		}
	}
}

void VoxelRenderer::CreateMesh(GLfloat* vertices, GLuint* indices, glm::vec3 position, glm::vec4 color)
{
	std::shared_ptr<Voxel> obj = std::make_shared<Voxel>();
	obj->CreateVoxel(vertices, indices, 216, 36);
	obj->SetPosition(position);
	obj->SetColor(color);
	m_meshList.push_back(obj);
}

void VoxelRenderer::CreateShader()
{
	m_shader = std::make_shared<Shader>();
	m_shader->CreateFromFile(m_vertexShaderPath, m_fragmentShaderPath);

	m_shader->UseShader();

	m_uniformModel = m_shader->GetUniformLocation("model");
	m_viewProjectionUniform = m_shader->GetUniformLocation("ViewProjectionMatrix");
	m_uniformColor = m_shader->GetUniformLocation("meshColor");

	m_lightPositionUniform = m_shader->GetUniformLocation("lightPos");
	m_lightColorUniform = m_shader->GetUniformLocation("lightColor");
	m_viewPositionUniform = m_shader->GetUniformLocation("viewPos");

	glUseProgram(0);
}

void VoxelRenderer::InitializeLighting()
{
	m_shader->UseShader();

	glUniform3f(m_lightPositionUniform, m_lightPosition.x, m_lightPosition.y, m_lightPosition.z); 
	glUniform4f(m_lightColorUniform, m_lightColor.r, m_lightColor.g, m_lightColor.b, m_lightColor.a);
	glUniform3f(m_viewPositionUniform, m_camera->GetPosition().x, m_camera->GetPosition().y, m_camera->GetPosition().z);
	
	glUseProgram(0);
}

std::vector<float> VoxelRenderer::CreateHeightMap(unsigned int countX, unsigned int countZ, glm::ivec2 coords)
{
	std::vector<float> pixels(countZ * countX);
	for (unsigned int j = 0; j < countZ; ++j)
	{
		for (unsigned int i = 0; i < countX; ++i)
		{
			float x = static_cast<float>(i) / (countX - 1) + coords.x;
			float y = static_cast<float>(j) / (countZ - 1) + coords.y;
			pixels[j * countX + i] = stb_perlin_fbm_noise3(x, 0.f, y, m_lacunarity, m_gain, m_octaves) * m_modifier;
		}
	}

	return pixels;
}

void VoxelRenderer::RenderVoxels()
{
	for (auto mesh : m_meshList)
	{
		glm::mat4 model(1.0f);

		glm::vec3 pos = mesh->GetPosition();
		glm::vec4 color = mesh->GetColor();

		model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));
		model = glm::scale(model, glm::vec3(1.f, 1.f, 1.f));
		glUniformMatrix4fv(m_uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(m_viewProjectionUniform, 1, GL_FALSE, glm::value_ptr(m_projection * m_camera->CalculateViewMatrix()));
		glUniform4f(m_uniformColor, color.r, color.g, color.b, color.a);

		mesh->RenderVoxel();
	}
}

void VoxelRenderer::RunApplication()
{
	// Loop until window is closed
	while (!m_mainWindow->GetShouldClose())
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLfloat now = glfwGetTime();
		m_deltaTime = now - m_lastTime;
		m_lastTime = now;

		// Get + Handle user input events
		glfwPollEvents();

		m_camera->KeyControl(m_mainWindow->GetKeys(), m_deltaTime);
		m_camera->MouseControl(m_mainWindow->GetXChange(), m_mainWindow->GetYChange());

		// Clear Window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		m_shader->UseShader();

		RenderVoxels();

		glUseProgram(0);

		// Swap Buffers
		m_mainWindow->SwapBuffers();

		//RenderGUI();
	}
}