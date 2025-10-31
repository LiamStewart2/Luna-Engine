cbuffer ConstantBuffer : register(b0)
{
    float3 LightDirection;
    float AmbientIntensity;
    
    float4 LightColour;
    
    float4 AmbientColour;
    float4 SpecularColour;
    
    float3 CameraPosition;
    float SpecularIntensity;
    
    column_major float4x4 Projection;
    column_major float4x4 View;
    column_major float4x4 World;
    column_major float4x4 lightSpaceMatrix;
}

struct VS_Out
{
    float4 position : SV_POSITION;
    float3 texCoord : TEXCOORD0;
};

TextureCube skyboxTexture : register(t0);
SamplerState skyboxSampler : register(s0);

VS_Out VS_main(float3 Position : POSITION, float2 TextureCoordinate : TEXTURECOORD, float3 Normal : NORMAL)
{   
    VS_Out output;
    float4 worldPos = mul(World, float4(Position, 1.0f));
    float4 viewPos = mul(View, worldPos);

    output.position = mul(Projection, viewPos);
    output.texCoord = normalize(worldPos.xyz - CameraPosition);
    return output;
}

float4 PS_main(VS_Out input) : SV_TARGET
{
    float4 color = skyboxTexture.Sample(skyboxSampler, input.texCoord);
    return float4(1, 1, 1, 1);
}