cbuffer cbPerObject : register(b0)
{
    float4x4 wvp;
    float4x4 world;
};

struct SKYMAP_VS_OUTPUT    //output structure for skymap vertex shader
{
    float4 Pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

SKYMAP_VS_OUTPUT main(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{
    SKYMAP_VS_OUTPUT output;
    //Set Pos to xyww instead of xyzw, so that z will always be 1 (furthest from camera)
    output.Pos = mul(inPos, wvp).xyww;
    output.texCoord = inTexCoord;
    return output;
}
