	#version 410

	layout (location = 0) in vec3 pos;

	out vec4 vColor;

	uniform vec4 color;
	uniform mat4 model;
	uniform mat4 ViewProjectionMatrix;

	void main()
	{
		gl_Position = ViewProjectionMatrix * model * vec4(pos, 1.0f); 
		vColor = color;
	}