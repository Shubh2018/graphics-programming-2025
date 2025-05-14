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

	void SetPosition(glm::ivec3 pos ) { m_position = glm::ivec3(pos); }
	void SetColor(glm::vec4 color) { m_color = glm::vec4(color); }

	glm::ivec3 GetPosition() { return m_position; }
	glm::vec4 GetColor() { return m_color; }

	~Voxel();

private:
	GLuint m_VAO, m_VBO, m_IBO;
	GLsizei m_indexCount, m_vertexCount;

	glm::vec4 m_color;
	glm::ivec3 m_position;

	std::vector<GLfloat> m_vertices;
	std::vector<GLint> m_indices;
};