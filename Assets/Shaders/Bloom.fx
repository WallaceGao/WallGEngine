// Description: Bloom shader.

Texture2D baseTextureMap : register(t0);
Texture2D bloomTextureMap : register(t1);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float2 texCoord : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
	output.position = float4(input.position, 1.0f);
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 baseColor = baseTextureMap.Sample(textureSampler, input.texCoord);
	float4 bloomColor = bloomTextureMap.Sample(textureSampler, input.texCoord);
	return baseColor + bloomColor;
}