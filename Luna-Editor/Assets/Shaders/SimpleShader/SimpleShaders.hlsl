cbuffer ConstantBuffer : register(b0)
{
    
    
    float3 LightDirection;
    float AmbientIntensity;
    
    float4 LightColour;
    
    float4 AmbientColour;
    float4 SpecularColour;
    column_major float4x4 Projection;
    column_major float4x4 View;
    column_major float4x4 World;
    
}

struct VS_Out
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL0;
    float3 PosW : POSITION0;
};

VS_Out VS_main(float3 Position : POSITION, float2 TextureCoordinate : TEXTURECOORD, float3 Normal : NORMAL)
{   
    VS_Out output = (VS_Out)0;

    float4 Pos4 = float4(Position, 1.0f);
    
    float4 worldPos = mul(World, Pos4);
    float4 viewPos = mul(View, worldPos);
    output.position = mul(Projection, viewPos);
    
    output.color = AmbientColour;

    float3 worldNormal = mul(World, float4(Normal.xyz, 1));
    float3 normNormal = normalize(worldNormal);
    output.normal = normNormal;
    return output;
}

float4 PS_main(VS_Out input) : SV_TARGET
{
    // AMBIENT LIGHTING
    float4 ambient = AmbientColour * AmbientIntensity;
    
    //DIFFUSE LIGHTING
    float3 LightDifference = -LightDirection;
    float d = dot(normalize(input.normal), LightDifference);
    
    float4 diffuse = saturate(d) * (AmbientColour * LightColour);
    
    // SPECULAR LIGHTING
    
    
    input.color = ambient + diffuse;
    
    return input.color;
}