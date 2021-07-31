// Description: Post Processing shader.

//uniform sampler2D sceneTex;
Texture2D textureMap : register(t0);
SamplerState textureSampler : register(s0);

cbuffer SettingBuffer : register(b0)
{
	//uniform float vx_offset;
	float vxOffset;
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
     vec2 uv = gl_TexCoord[0].xy;
     vec3 tc = vec3(1.0, 0.0, 0.0);
     if (uv.x < (vx_offset-0.005))
     {
       vec3 pixcol = texture2D(sceneTex, uv).rgb;
       vec3 colors[3];
       colors[0] = vec3(0.,0.,1.);
       colors[1] = vec3(1.,1.,0.);
       colors[2] = vec3(1.,0.,0.);
       float lum = (pixcol.r+pixcol.g+pixcol.b)/3.;
       int ix = (lum < 0.5)? 0:1;
       tc = mix(colors[ix],colors[ix+1],(lum-float(ix)*0.5)/0.5);
     }
     else if (uv.x>=(vx_offset+0.005))
     {
       tc = texture2D(sceneTex, uv).rgb;
     }
    gl_FragColor = vec4(tc, 1.0);*/

    float2 uv = (input.texCoord).xy;
    float3 tc = float3(1.0f, 0.0f, 0.0f);
    if (uv.x < (vxOffset - 0.005f))
    {
        float3 pixcol = textureMap.Sample(textureSampler, input.texCoord).rgb;
        float3 colors[3];
        colors[0] = float3(0.0f, 0.0f, 1.0f);
        colors[1] = float3(1.0f, 1.0f, 0.0f);
        colors[2] = float3(1.0f, 0.0f, 0.0f);
        float lum = (pixcol.r + pixcol.g + pixcol.b) / 3;
        int ix = (lum < 0.5) ? 0 : 1;
        tc = lerp(colors[ix], colors[ix +1], (lum - (float)ix * 0.5)/0.5 );
    }
    else if (uv.x >= (vxOffset + 0.005))
    {
        tc = textureMap.Sample(textureSampler, input.texCoord).rgb;
    }
    return float4(tc, 1.0);
}
