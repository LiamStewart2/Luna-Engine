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
    float4 position : SV_Position;
};

VS_Out VS_main(float3 Position : POSITION, float2 TextureCoordinate : TEXTURECOORD, float3 Normal : NORMAL)
{
    VS_Out output = (VS_Out) 0;
    
    float4 worldPos = mul(World, float4(Position, 1.0f));
    output.position = mul(lightSpaceMatrix, worldPos);

    return output;
}

float4 PS_main(VS_Out input) : SV_TARGET
{
    float ndcDepth = input.position.z / input.position.w;

    return float4(ndcDepth, ndcDepth, ndcDepth, 1.0f);
}