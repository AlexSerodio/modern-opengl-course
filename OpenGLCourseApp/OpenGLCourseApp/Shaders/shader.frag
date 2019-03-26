#version 330

in vec4 vCol;
in vec2 TexCoord;

struct DirectionalLight {
	vec3 colour;
	float ambientIntensity;
};

out vec4 colour;

uniform sampler2D theTexture;
uniform DirectionalLight directionalLight;

void main()	{
	vec4 ambientColour = vec4(directionalLight.colour, 1.0f)* directionalLight.ambientIntensity;

	colour = texture(theTexture, TexCoord) * ambientColour;
}