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
    float2 texCoord : TEXCOORD0;
    float3 worldDir : TEXCOORD1;
};

VS_Out VS_main(float3 Position : POSITION, float2 TextureCoordinate : TEXTURECOORD, float3 Normal : NORMAL)
{
    VS_Out output;
    
    float4 worldPos = mul(World, float4(Position, 1.0f));
    float4 viewPos = mul(View, worldPos);
    output.position = mul(Projection, viewPos);
    
    output.position = output.position.xyww;
    
    output.worldDir = normalize(worldPos.xyz - CameraPosition);

    return output;
}

float4 PS_main(VS_Out input) : SV_TARGET
{
    float3 dir = normalize(input.worldDir);
    
    float3 skyColor = float3(0.6f, 0.7f, 0.9f);
    float3 floorColor = float3(0.05f, 0.1f, 0.4f); 
    
    float t = saturate(dir.y * 0.5f + 0.5f);

    float3 posColor = lerp(skyColor, floorColor, t);
    
    float sun = saturate(dot(dir, -LightDirection));
    posColor += LightColour.rgb * pow(sun, 64.0f) * 0.3f;
    
    //return float4(pow(sun, 64.0f), pow(sun, 64.0f), pow(sun, 64.0f), 1.0f);
    return float4(posColor, 1.0f);
}