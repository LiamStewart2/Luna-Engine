cbuffer ConstantBuffer : register(b0)
{
    float4x4 Projection;
    float4x4 View;
    float4x4 World;
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
    output.position = mul(Pos4, World);
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);
    
    output.color = Color;
    
    return output;
}

float4 PS_main(VS_Out input) : SV_TARGET
{
    return input.color;
}