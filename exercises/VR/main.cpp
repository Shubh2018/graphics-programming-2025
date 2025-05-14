#include "VoxelRenderer.h"

int main()
{
	int voxelCountX = 100;
	int voxelCountZ = 100;

	glm::vec4 lightColor = glm::vec4(1.f, 1.f, 1.f, 1.f);
	glm::vec3 lightPosition = glm::vec3(25.f, 10.f, 25.f);

	float lacunarity = 2;
	float gain = .75f;
	float octaves = 5;
	float modifier = 1.f;

	VoxelRenderer renderer(
		voxelCountX,
		voxelCountZ,
		lightColor,
		lightPosition,
		lacunarity,
		gain,
		octaves,
		modifier
	);
	renderer.RunApplication();
	return 0;
}