// Description: Simple shader that does transform and lighting.

// b stand for buffer
cbuffer TransformBuffer : register(b0)
{
	matrix world;
	matrix wvp[2];
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
	int useShadow;
	int useSkinning;
}

cbuffer BoneTransformBuffer : register(b4)
{
	matrix boneTransforms[256];
}

// t2 = b2 same with other number 
// t = texture
// s = sampler
Texture2D textureMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D displacementMap : register(t2);
Texture2D normalMap : register(t3);
Texture2D shadowMap : register(t4);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord : TEXCOORD;
	int4 blendIndices : BLENDINDICES;
	float4 blendWeights : BLENDWEIGHT;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float3 normal : TEXCOORD0;
	float3 tangent : TEXCOORD1;
	float3 binormal : TEXCOORD2;
	float3 dirToView : TEXCOORD3;
	float2 texCoord : TEXCOORD4;
	//light ndc position
	float4 position2 : TEXCOORD5;
};

static matrix Identity =
{
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1
};

matrix GetBoneTransform(int4 indices, float4 weights)
{
	//if no weights return Identity
	if (length(weights) <= 0.0f)
		return Identity;
	//if there are weights use each one's index time's weights
	// TODO:: ask what is the indices[0]
	matrix transform;
	transform  = boneTransforms[indices[0]] * weights[0];
	transform += boneTransforms[indices[1]] * weights[1];
	transform += boneTransforms[indices[2]] * weights[2];
	transform += boneTransforms[indices[3]] * weights[3];
	return transform;
}

VS_OUTPUT VS(VS_INPUT input)
{
	float displacement = displacementMap.SampleLevel(textureSampler, input.texCoord, 0).x;
	float3 position = input.position + (input.normal * displacement * bumpMapWeight);

	matrix toWorld = world;
	matrix toNDC = wvp[0];
	matrix toLightNDC = wvp[1];

	if (useSkinning != 0)
	{
		matrix boneTransform = GetBoneTransform(input.blendIndices, input.blendWeights);
		toWorld = mul(boneTransform, world);
		toNDC = mul(boneTransform, wvp[0]);
	}

	float3 worldPosition = mul(float4(position, 1.0f), toWorld).xyz;
	float3 worldNormal = mul(input.normal, (float3x3)toWorld);
	float3 worldTangent = mul(input.tangent, (float3x3)toWorld);
	float3 worldBinormal = normalize(cross(worldNormal, worldTangent));

	VS_OUTPUT output;
	output.position = mul(float4(position, 1.0f), toNDC);
	output.normal = worldNormal;
	output.tangent = worldTangent;
	output.binormal = worldBinormal;
	output.dirToView = normalize(viewPosition - worldPosition);
	output.texCoord = input.texCoord;
	output.position2 = mul(float4(position, 1.0f), toLightNDC);
	return output;
}

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
	float3 dirToLight = -lightDirection;
	float diffuseIntensity = saturate(dot(normal, dirToLight));
	float4 diffuse = diffuseIntensity * lightDiffuse * materialDiffuse;

	float3 reflected = normalize(reflect(lightDirection, normal));
	float specularBase = saturate(dot(reflected, dirToView));
	float specularIntensity = pow(specularBase, specularPower);
	float4 specular = specularIntensity * lightSpecular * materialSpecular;

	float4 diffuseMapColor = textureMap.Sample(textureSampler, input.texCoord);
	float4 specularMapColor = specularMap.Sample(textureSampler, input.texCoord);


	float realDepth = 1.0 - input.position2.z / input.position2.w;
	float2 shadowUV = input.position2.xy / input.position2.w;
	shadowUV = (shadowUV + 1.0f) * 0.5f;
	shadowUV.y = 1.0f - shadowUV.y;

	float recordedDepth = shadowMap.Sample(textureSampler, shadowUV).r;

	float4 finalColor = (ambient + diffuse) * diffuseMapColor + specular * (specularMapWeight != 0.0f ? specularMapColor.r : 1.0f);

	if (useShadow != 0)
	{
		if (saturate(shadowUV.x) == shadowUV.x &&
			saturate(shadowUV.y) == shadowUV.y &&
			realDepth + 0.00001f < recordedDepth)
		{
			finalColor = ambient * diffuseMapColor;
		}
	}

	return finalColor;
}