cbuffer cbPerUI : register(b0)
{
    float4 maincolor;
    float4 bordercolor;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

struct UI_VS_OUTPUT    //output structure for ui vertex shader
{
    float4 Pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

float4 main(UI_VS_OUTPUT input) : SV_TARGET
{
    float4 diffuse = objTexture.Sample(objSamplerState, input.texCoord);
    if (diffuse.r == 0) //black (main color)
    {
        return maincolor;
    }
    else
    {
        return bordercolor;
    }
    //    float3 finalColor = diffuse;
    //return float4(finalColor, 0.5f);
}
