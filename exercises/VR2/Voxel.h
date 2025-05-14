#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"

#include <vector>

class Voxel
{
public:
	Voxel();

	void CreateVoxel(GLfloat* vertices, GLuint* indices, GLuint numOfVertices, GLuint numOfIndices);
	void RenderVoxel();
	void ClearVoxel();
	void SetPosition(glm::ivec3(pos)) { m_position = glm::ivec3(pos); }
	glm::ivec3 GetPosition() { return m_position; }

	~Voxel();

private:
	GLuint m_VAO, m_VBO, m_IBO;
	GLsizei m_indexCount, m_vertexCount;

	glm::ivec3 m_position;

	std::vector<GLfloat> m_vertices;
	std::vector<GLint> m_indices;
};