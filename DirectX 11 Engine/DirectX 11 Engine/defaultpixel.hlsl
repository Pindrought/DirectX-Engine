struct Light
{
    float3 dir;
    float padding;
    float4 ambient;
    float4 diffuse;
};

struct Pointlight
{
    float4 lightColor;
    float4 pos;
    float3 att;
    float range;
};

cbuffer cbWorldLightPerFrame : register(b0) //Ambient & Directional Light
{
    Light light;
};

cbuffer cbPointLightPerFrame : register(b1) //For the point light in the scene
{
    Pointlight pointlight;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 worldPos : POSITION;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
    input.normal = normalize(input.normal);

    float4 pixelSample = objTexture.Sample(objSamplerState, input.texCoord);
    
    float4 pixelPercent = light.ambient; //default to ambient


    ////calculate length from pointlight to pos
    float3 lightToPixelVec = pointlight.pos - input.worldPos;
    float d = length(lightToPixelVec);
    if (d < pointlight.range)
    {
        lightToPixelVec /= d; //transform to unit vector
        //Calculate how much light the pixel gets by the angle
	    //in which the light strikes the pixels surface
        float lightPercentage = dot(lightToPixelVec, input.normal);

	    //If light is striking the front side of the pixel
        if (lightPercentage > 0.0f)
        {
            pixelPercent += pointlight.lightColor / (pointlight.att[0] + (pointlight.att[1] * d) + (pointlight.att[2] * (d * d))) * lightPercentage;
        }
    }

    ////Apply directional lighting
    pixelPercent += saturate(dot(light.dir, input.normal) * light.diffuse);

    pixelPercent = saturate(pixelPercent);

    pixelSample *= pixelPercent; //adjust pixel sample by pixel percent based on lighting
    return float4(pixelSample);
    //float4 diffuse = objTexture.Sample(objSamplerState, input.texCoord);
    //clip(diffuse.a - .25);
    //return diffuse;
}

//float4 main(VS_OUTPUT input) : SV_TARGET
//{
//    input.normal = normalize(input.normal);

//    float4 diffuse = objTexture.Sample(objSamplerState, input.texCoord);

//    float3 finalColor; //final color which will be stored

//    finalColor = diffuse * light.ambient; //default to the ambient color multiplied by original

//    //calculate length from pointlight to pos
//    float3 lightToPixelVec = pointlight.pos - input.worldPos;
//    float d = length(lightToPixelVec);

//    if (d < pointlight.range)
//    {
//        lightToPixelVec /= d; //transform to unit vector
//        //Calculate how much light the pixel gets by the angle
//	    //in which the light strikes the pixels surface
//        float howMuchLight = dot(lightToPixelVec, input.normal);

//	    //If light is striking the front side of the pixel
//        if (howMuchLight > 0.0f)
//        {
//		    //Add light to the finalColor of the pixel
//            finalColor += (howMuchLight * diffuse * pointlight.lightColor);
		
//		    //Calculate Light's Falloff factor
//            finalColor /= (pointlight.att[0] + (pointlight.att[1] * d) + (pointlight.att[2] * (d * d)));
//        }
//    }

//    //Apply directional lighting
//    finalColor += saturate(dot(light.dir, input.normal) * light.diffuse * diffuse);
    
//    finalColor = saturate(finalColor);

//    return float4(finalColor, diffuse.a);
//    //float4 diffuse = objTexture.Sample(objSamplerState, input.texCoord);
//    //clip(diffuse.a - .25);
//    //return diffuse;
//}