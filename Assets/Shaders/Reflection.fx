// Reference: https://www.rastertek.com/dx11tut27.html
// Description: Reflection

// b stand for buffer
cbuffer TransformBuffer : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
}

//add reflection
cbuffer ReflectionBuffer : register(b1)
{
	matrix reflectionMatrix;
}

// t2 = b2 same with other number 
// t = texture
// s = sampler


//reflection variable
Texture2D shaderTexture :register(t0);
Texture2D reflectionTexture :register(t1);
SamplerState SampleType : register(s0);

struct VS_INPUT
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float2 texCoord : TEXCOORD0;
	float4 reflectionPosition : TEXCOORD1;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
	matrix reflectProjectWorld;


	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);

	// Store the texture coordinates for the pixel shader.
	output.texCoord = input.texCoord;

	// Create the reflection projection world matrix.
	reflectProjectWorld = mul(reflectionMatrix, projection);
	reflectProjectWorld = mul(world, reflectProjectWorld);

	// Calculate the input position against the reflectProjectWorld matrix.
	output.reflectionPosition = mul(input.position, reflectProjectWorld);

	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 textureColor;
	float2 reflectTexCoord;
	float4 reflectionColor;
	float4 color;

	// Sample the texture pixel at this location.
	textureColor = shaderTexture.Sample(SampleType, input.texCoord);

	// Calculate the projected reflection texture coordinates.
	reflectTexCoord.x = input.reflectionPosition.x / input.reflectionPosition.w / 2.0f + 0.5f;
	reflectTexCoord.y = -input.reflectionPosition.y / input.reflectionPosition.w / 2.0f + 0.5f;

	// Sample the texture pixel from the reflection texture using the projected texture coordinates.
	reflectionColor = reflectionTexture.Sample(SampleType, reflectTexCoord);

	// Do a linear interpolation between the two textures for a blend effect.
	color = lerp(textureColor, reflectionColor, 0.5f);

	return color;

}