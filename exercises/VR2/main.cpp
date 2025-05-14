#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/mat4x4.hpp"

#include "Voxel.h"
#include "Shader.h"
#include "GLWindow.h"
#include "Camera.h"

#include "ituGL/asset/Texture2DLoader.h"

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

GLWindow* mainWindow;
std::vector<std::shared_ptr<Voxel>> meshList;
std::vector<std::shared_ptr<Shader>> shaderList;
Camera* camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader
std::string vShader = "shaders\\vertex.shader";

// Fragment Shader
std::string fShader = "shaders\\fragment.shader";

const int voxelCountX = 100;
//const int voxelCountY = 10;
const int voxelCountZ = 100;

GLuint indices[] = {
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

GLfloat vertices[] = {
	-.5f,  .5f, -.5f,
	 .5f,  .5f, -.5f,
	-.5f, -.5f, -.5f,
	 .5f, -.5f, -.5f,

	-.5f,  .5f,  .5f,
	 .5f,  .5f,  .5f,
	-.5f, -.5f,  .5f,
	 .5f, -.5f,  .5f
};

void CreateMesh(GLfloat* vertices, GLuint* indices, glm::vec3 position)
{
	std::shared_ptr<Voxel> obj = std::make_shared<Voxel>();
	obj->CreateVoxel(vertices, indices, 24, 36);
	obj->SetPosition(position);
	meshList.push_back(obj);
}

void CreateShader()
{
	std::shared_ptr<Shader> shader = std::make_shared<Shader>();
	shader->CreateFromFile(vShader, fShader);
	shaderList.push_back(shader);
}

std::vector<float> CreateHeightMap(unsigned int width, unsigned int height, glm::ivec2 coords)
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

int main()
{
	mainWindow = new GLWindow(800, 600);
	mainWindow->Initialize();

	//CreateMesh(vertices, indices);
	CreateShader();

	camera = new Camera(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -90.0f, 5.0f, 0.5f);

	std::cout << glGetString(GL_VERSION) << std::endl << glGetString(GL_VENDOR) << std::endl << glGetString(GL_RENDERER);

	glm::mat4 projection(1.0f);
	projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow->GetBufferWidth() / (GLfloat)mainWindow->GetBufferHeight(), 0.1f, 100.0f);

	GLuint uniformModel = shaderList[0]->GetUniformLocation("model");
	GLuint viewProjectionUniform = shaderList[0]->GetUniformLocation("ViewProjectionMatrix");
	GLuint uniformColor = shaderList[0]->GetUniformLocation("color");

	std::vector<float> heights = CreateHeightMap(voxelCountX, voxelCountZ, glm::ivec2(0, 0));
	std::vector<glm::ivec3> positions;

	std::cout << std::endl << heights.size();

	for (int x = 0; x < voxelCountX; x++)
	{
		for (int z = 0; z < voxelCountZ; z++)
		{
			//std::cout << heights[x * voxelCountZ + z] * 10.f << std::endl;
			//
			positions.push_back(glm::ivec3(static_cast<GLint>(x), static_cast<GLint>(abs(heights[x * voxelCountZ + z]) * 10.f), static_cast<GLint>(z)));
			//CreateMesh(vertices, indices, glm::ivec3(static_cast<GLint>(x), , static_cast<GLint>(z)));
		}
	}

	for (int i = 0; i < positions.size(); i++)
	{
		for (int j = 0; j < positions[i].y; j++)
		{
			//std::cout << std::endl << "Creating Mesh: " << positions[i].x << " " << j << " " << positions[i].z;
			CreateMesh(vertices, indices, glm::ivec3(positions[i].x, j, positions[i].z));
		}
	}

	std::cout << std::endl << meshList.size();

	// Loop until window is closed
	while (!mainWindow->GetShouldClose())
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		// Get + Handle user input events
		glfwPollEvents();

		camera->KeyControl(mainWindow->GetKeys(), deltaTime);
		camera->MouseControl(mainWindow->GetXChange(), mainWindow->GetYChange());

		// Clear Window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		shaderList[0]->UseShader();

		for (auto mesh : meshList)
		{
			glm::mat4 model(1.0f);

			glm::vec3 pos = mesh->GetPosition();

			model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));
			model = glm::scale(model, glm::vec3(1.f, 1.f, 1.f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(viewProjectionUniform, 1, GL_FALSE, glm::value_ptr(projection * camera->CalculateViewMatrix()));
			//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->CalculateViewMatrix()));
			if (mesh->GetPosition().y > 2)
			{
				glUniform4f(uniformColor, 1.f, 1.f, 1.f, 1.f);
			}

			else if (mesh->GetPosition().y >= 1)
			{
				glUniform4f(uniformColor, .7f, .4f, 0.f, 1.f);
			}

			else
			{
				glUniform4f(uniformColor, 0.f, 1.f, 0.f, 1.f);
			}

			mesh->RenderVoxel();
		}

		glUseProgram(0);

		// Swap Buffers
		mainWindow->SwapBuffers();
	}

	return 0;
}