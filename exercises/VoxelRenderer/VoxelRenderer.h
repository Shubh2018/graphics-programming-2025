#pragma once

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

class VoxelRenderer
{
public:
	VoxelRenderer();
	VoxelRenderer(int voxelCountX, int voxelCountZ, glm::vec4 lightColor, glm::vec3 lightPosition, float lacunarity, float grain, float octaves, float modifiers);

private:
	std::unique_ptr<GLWindow> m_mainWindow;
	std::vector<std::shared_ptr<Voxel>> m_meshList;
	std::shared_ptr<Shader> m_shader;
	std::unique_ptr<Camera> m_camera;

	GLfloat m_deltaTime = 0.0f;
	GLfloat m_lastTime = 0.0f;

	std::string m_vertexShaderPath;
	std::string m_fragmentShaderPath;

	GLuint m_uniformModel;
	GLuint m_viewProjectionUniform;
	GLuint m_uniformColor;

	GLuint m_lightPositionUniform;
	GLuint m_lightColorUniform;
	GLuint m_viewPositionUniform;

	glm::mat4 m_projection;

	int m_voxelCountX = 50;
	//const int voxelCountY = 10;
	int m_voxelCountZ = 50;

	GLuint* m_indices = new GLuint[1];
	GLfloat* m_vertices = new GLfloat[1];

	glm::vec3 m_lightPosition;
	glm::vec4 m_lightColor;

	float m_lacunarity;
	float m_gain;
	float m_octaves;
	float m_modifier;

private:
	void Initialize();
	void GenerateVoxelList();
	void CreateMesh(GLfloat* vertices, GLuint* indices, glm::vec3 position, glm::vec4 color);
	void CreateShader();
	void InitializeWindow();

	void RenderVoxels();

	void InitializeLighting();

	std::vector<float> CreateHeightMap(unsigned int width, unsigned int height, glm::ivec2 coords);

public:
	void RunApplication();

};