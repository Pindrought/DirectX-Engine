struct Light
{
    float3 dir;
    float padding;
    float4 ambient;
    float4 diffuse;
};

cbuffer cbWorldLightPerFrame : register(b0) //Ambient & Directional Light
{
    Light light;
};


Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

struct SKYMAP_VS_OUTPUT    //output structure for skymap vertex shader
{
    float4 Pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

float4 main(SKYMAP_VS_OUTPUT input) : SV_TARGET
{
    float4 diffuse = objTexture.Sample(objSamplerState, input.texCoord);
    float3 finalColor = diffuse;
    return float4(finalColor, 1.0f);
}
