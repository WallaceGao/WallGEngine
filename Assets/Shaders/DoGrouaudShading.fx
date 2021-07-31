// Description: Simple shader that does transform and lighting.

cbuffer TransformBuffer : register(b0)
{
	matrix world;
	matrix wvp;
	float3 viewPosition;
}

cbuffer LightBuffer : register(b1)
{
	float3 lightDirection;
	float4 lightAmbient;
	float4 lightDiffuse;
	float4 lightSpecular;
}

cbuffer MaterialBuffer : register(b2)
{
	float4 materialAmbient;
	float4 materialDiffuse;
	float4 materialSpecular;
	float specularPower;
}

struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float4 color : COLOR;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;

	float3 worldPosition = mul(float4(input.position, 1.0f), world).xyz;
	float3 worldNormal = mul(input.normal, (float3x3)world);
	
	float4 ambient = lightAmbient * materialAmbient;

	float3 dirToLight = -lightDirection;
	float diffuseIntensity = saturate(dot(worldNormal, dirToLight));
	float4 diffuse = diffuseIntensity * lightDiffuse * materialDiffuse;

	float3 dirToView = normalize(viewPosition - worldPosition);
	float3 reflected = normalize(reflect(lightDirection, worldNormal));
	float specularBase = saturate(dot(reflected, dirToView));
	float specularIntensity = pow(specularBase, specularPower);
	float4 specular = specularIntensity * lightSpecular * materialSpecular;

	output.position = mul(float4(input.position, 1.0f), wvp);
	output.color = ambient + diffuse + specular;

	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	return input.color;
}