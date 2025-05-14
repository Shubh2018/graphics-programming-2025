#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"

#include <vector>

class Voxel
{
public:
	Voxel();

	void CreateVoxel();
	void RenderVoxel();
	void ClearVoxel();
	void SetPosition(glm::ivec3(pos)) { m_position = glm::ivec3(pos); }
	glm::ivec3 GetPosition() { return m_position; }

	~Voxel();

private:
	GLuint m_VAO, m_VBO, m_IBO;
	GLint m_indexCount, m_vertexCount;

	glm::ivec3 m_position;

	std::vector<GLfloat> m_vertices =
	{
	   -.5f,  .5f, -.5f,
		.5f,  .5f, -.5f,
	   -.5f, -.5f, -.5f,
		.5f, -.5f, -.5f,

	   -.5f,  .5f,  .5f,
		.5f,  .5f,  .5f,
	   -.5f, -.5f,  .5f,
		.5f, -.5f,  .5f
	};

	std::vector<GLint> 	m_indices =
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
};