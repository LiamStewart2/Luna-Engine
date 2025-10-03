cbuffer ConstantBuffer : register(b0)
{
    column_major float4x4 Projection;
    column_major float4x4 View;
    column_major float4x4 World;
}

struct VS_Out
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VS_Out VS_main(float3 Position : POSITION, float4 Color : COLOR)
{   
    VS_Out output = (VS_Out)0;

    float4 Pos4 = float4(Position, 1.0f);
    
    float4 worldPos = mul(World, Pos4);
    float4 viewPos = mul(View, worldPos);
    output.position = mul(Projection, viewPos);
    
    output.color = Color;
    
    return output;
}

float4 PS_main(VS_Out input) : SV_TARGET
{
    return input.color;
}