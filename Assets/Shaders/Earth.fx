// Description: Simple shader that does transform and lighting.

// b stand for buffer
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

cbuffer SettingsBuffer : register(b3)
{
	float specularMapWeight;
	float bumpMapWeight;
	float normalMapWeight;
}

// t2 = b2 same with other number 
// t = texture
// s = sampler
Texture2D textureMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D displacementMap : register(t2);
Texture2D normalMap : register(t3);
Texture2D cloudMap : register(t4);
Texture2D nightMap : register(t5);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float3 normal : TEXCOORD0;
	float3 tangent : TEXCOORD1;
	float3 binormal : TEXCOORD2;
	float3 dirToView : TEXCOORD3;
	float2 texCoord : TEXCOORD4;
};

struct VSCloud_OUTPUT
{
	float4 position : SV_Position;
	float3 normal : TEXCOORD0;
	float3 dirToView : TEXCOORD3;
	float2 texCoord : TEXCOORD4;
};

// VS runs 3 time for triangle
VS_OUTPUT VS(VS_INPUT input)
{
	float displacement = displacementMap.SampleLevel(textureSampler, input.texCoord, 0).x;
	float3 position = input.position + (input.normal * displacement * bumpMapWeight);

	float3 worldPosition = mul(float4(position, 1.0f), world).xyz;
	float3 worldNormal = mul(input.normal, (float3x3)world);
	float3 worldTangent = mul(input.tangent, (float3x3)world);
	float3 worldBinormal = normalize(cross(worldNormal, worldTangent));

	VS_OUTPUT output;
	output.position = mul(float4(position, 1.0f), wvp);
	output.normal = worldNormal;
	output.tangent = worldTangent;
	output.binormal = worldBinormal;
	output.dirToView = normalize(viewPosition - worldPosition);
	output.texCoord = input.texCoord;
	return output;
}

// PS runs for each pixel
float4 PS(VS_OUTPUT input) : SV_Target
{

	float3 dirToView = normalize(input.dirToView);

	// Get normal from texture and convert from [0, 1] to [-1 , 1]
	float3 sampledNormal= normalMap.Sample(textureSampler, input.texCoord).xyz; // from 0 to 1
	float3 unpackedNormal = (sampledNormal * 2) - 1;

	// Fix normals from rasterizer and construct the tangent space matrix
	float3 n = normalize(input.normal);
	float3 t = normalize(input.tangent);
	float3 b = normalize(input.binormal);
	
	float3x3 tbnw = float3x3(t, b, n);
	
	// True normal to use for lighting
	float3 normal = n;
	if (normalMapWeight != 0.0f)
	{
		normal = mul(unpackedNormal, tbnw);
	}
	
	
	float4 ambient = lightAmbient * materialAmbient;

	//diffuse take all lights that reflect only once's lights
	float3 dirToLight = normalize(-lightDirection);
	float timeOfDayFactor = -dot(normal, dirToLight);
	float diffuseIntensity = saturate(dot(normal, dirToLight));
	float4 diffuse = diffuseIntensity * lightDiffuse * materialDiffuse;

	float3 reflected = normalize(reflect(lightDirection, normal));
	float specularBase = saturate(dot(reflected, dirToView));
	float specularIntensity = pow(specularBase, specularPower);
	float4 specular = specularIntensity * lightSpecular * materialSpecular;

	//textureSampler take the which type of Filter in Sample use
	float4 diffuseMapColor = textureMap.Sample(textureSampler, input.texCoord);
	float4 specularMapColor = specularMap.Sample(textureSampler, input.texCoord);
	float4 nightMapColor = nightMap.Sample(textureSampler, input.texCoord);

	if (timeOfDayFactor < 0.0)
	{
		//do normal lighting;
		//specular = 1.0 mean everything will reflect
		return (ambient + diffuse)* diffuseMapColor + specular * (specularMapWeight != 0.0f ? specularMapColor.r : 1.0f);
	}
	else
	{
		//start using percentage of night map;
		return (ambient + diffuse) * diffuseMapColor + nightMapColor * timeOfDayFactor + specular * (specularMapWeight != 0.0f ? specularMapColor.r : 1.0f);
	}

}

VSCloud_OUTPUT VSCloud(VS_INPUT input)
{
	float3 position = input.position + (input.normal * bumpMapWeight);

	float3 worldPosition = mul(float4(position, 1.0f), world).xyz;
	float3 worldNormal = mul(input.normal, (float3x3)world);


	VSCloud_OUTPUT output;
	output.position = mul(float4(position, 1.0f), wvp);
	output.normal = worldNormal;
	output.dirToView = normalize(viewPosition - worldPosition);
	output.texCoord = input.texCoord;
	return output;
}

float4 PSCloud(VSCloud_OUTPUT input) : SV_Target
{
	float3 dirToView = normalize(input.dirToView);

	float3 normal = normalize(input.normal);

	float4 ambient = lightAmbient * materialAmbient;

	float3 dirToLight = -lightDirection;
	float diffuseIntensity = saturate(dot(normal, dirToLight));
	float4 diffuse = diffuseIntensity * lightDiffuse * materialDiffuse;

	float4 cloudMapColor = cloudMap.Sample(textureSampler, input.texCoord);

	//specular = 1.0 mean everything will reflect
	return (ambient + diffuse) * cloudMapColor;
}