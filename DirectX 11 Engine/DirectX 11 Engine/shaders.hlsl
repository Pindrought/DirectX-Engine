cbuffer cbPerObject
{
	float4x4 wvp;
};

Texture2D objTexture;
SamplerState objSamplerState;

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

VS_OUTPUT VS(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD)
{
	VS_OUTPUT output;

	output.pos = mul(inPos, wvp);
	output.texCoord = inTexCoord;

	return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
	return objTexture.Sample(objSamplerState, input.texCoord);
}