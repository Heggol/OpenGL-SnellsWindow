#version 330 core
out vec4 FragColor;
in vec4 vertCoords;
uniform float aspectRatio;

void main()
{
	float brightness = 0.25f;
	vec2 center = vec2(0.0f, 0.0f);
	float radius = 0.5f;
	float dist = distance(vertCoords.xy, center);
	if(dist < radius)
	{
		brightness += 0.5f * (1.0f - dist / radius);
	}
	FragColor = vec4(0.0f, 0.0f, brightness, 1.0f);
}