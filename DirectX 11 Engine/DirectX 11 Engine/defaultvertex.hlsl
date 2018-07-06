cbuffer cbPerObject : register(b0)
{
    float4x4 wvp;
    float4x4 world;
};

Texture2D objTexture;
SamplerState objSamplerState;

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 worldPos : POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
};

VS_OUTPUT main(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{
    VS_OUTPUT output;

    output.pos = mul(inPos, wvp);
    output.worldPos = mul(inPos, world); //needed to calculate distance from light to pos
    output.texCoord = inTexCoord;
    output.normal = mul(normal, world);
    
    return output;
}