#version 450 core
out vec4 FragColor;
in vec4 vertCoords;
void main()
{
	float brightness = 0.25f;
	vec2 center = vec2(0.0f, 0.0f);
	float radius = 0.56695f;
	float dist = distance(vertCoords.xy, center);
	if(dist < radius)
	{
		brightness += 0.5f * (1.0f - dist / radius);
	}
	FragColor = vec4(0.0f, 0.0f, brightness, 1.0f);
}
