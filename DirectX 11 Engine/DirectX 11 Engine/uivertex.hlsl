cbuffer cbPerUI : register(b0)
{
    float4 uipos;
};

struct UI_VS_OUTPUT    //output structure for skymap vertex shader
{
    float4 Pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

UI_VS_OUTPUT main(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD)
{
    UI_VS_OUTPUT output;
    output.Pos = inPos.xyzz + uipos;
    output.texCoord = inTexCoord;
    return output;
}
