// Description: Post Processing shader.

//uniform sampler2D sceneBuffer;
//uniform sampler2D noiseTex;
//uniform sampler2D maskTex;
Texture2D sceneBuffer : register(t0);
Texture2D noiseTex : register(t1);
SamplerState textureSampler : register(s0);



cbuffer SettingBuffer : register(b0)
{
    //uniform float elapsedTime; // seconds
    //uniform float luminanceThreshold; // 0.2
    //uniform float colorAmplification; // 4.0
    //uniform float effectCoverage; // 0.5
    float elapsedTime;
    float luminanceThreshold;
    float colorAmplification;
    float effectCoverage;
}

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
	// gl_Position = ftransform();
	output.position = float4(input.position, 1.0f);
	//gl_TexCoord[0] = gl_MultiTexCoord0;
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    /*
        vec4 finalColor;
    // Set effectCoverage to 1.0 for normal use.  
    if (gl_TexCoord[0].x < effectCoverage) 
    {
      vec2 uv;           
      uv.x = 0.4*sin(elapsedTime*50.0);                                 
      uv.y = 0.4*cos(elapsedTime*50.0);                                 
      float m = texture2D(maskTex, gl_TexCoord[0].st).r;
      vec3 n = texture2D(noiseTex, 
                   (gl_TexCoord[0].st*3.5) + uv).rgb;
      vec3 c = texture2D(sceneBuffer, gl_TexCoord[0].st 
                                 + (n.xy*0.005)).rgb;
    
      float lum = dot(vec3(0.30, 0.59, 0.11), c);
      if (lum < luminanceThreshold)
        c *= colorAmplification; 
    
      vec3 visionColor = vec3(0.1, 0.95, 0.2);
      finalColor.rgb = (c + (n*0.2)) * visionColor * m;
     }
     else
     {
      finalColor = texture2D(sceneBuffer, 
                     gl_TexCoord[0].st);
     }
    gl_FragColor.rgb = finalColor.rgb;
    gl_FragColor.a = 1.0;*/

    float4 finalColor;
    if (input.texCoord.x < effectCoverage)
    {
        float2 uv;
        uv.x = 0.4 * sin(elapsedTime * 50.0);
        uv.y = 0.4 * cos(elapsedTime * 50.0);
        float3 n = noiseTex.Sample(textureSampler,((input.texCoord).xy * 3.5) + uv).rgb;
        float3 c = sceneBuffer.Sample(textureSampler, (input.texCoord).xy + (n.xy * 0.005)).rgb;
        float lum = dot(float3(0.30, 0.59, 0.11), c);
        if (lum < luminanceThreshold)c *= colorAmplification;
        float3 visionColor = float3(0.1, 0.95, 0.2);
        finalColor.rgb = (c + (n * 0.2))* visionColor;
    }
    else
    {
        finalColor = sceneBuffer.Sample(textureSampler, (input.texCoord).xy);
    }
    return finalColor;
}
