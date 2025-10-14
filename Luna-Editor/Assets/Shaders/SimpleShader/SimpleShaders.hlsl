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

    float3 worldNormal = normalize(mul((float3x3) World, Normal));
    output.normal = worldNormal;
    
    return output;
}

float4 PS_main(VS_Out input) : SV_TARGET
{
    float3 surfaceNormal = normalize(input.normal);
    float3 surfaceToLightDirection = normalize(-LightDirection);
    
    // AMBIENT LIGHTING
    float4 ambient = AmbientColour * AmbientIntensity;
    
    //DIFFUSE LIGHTING
    float diffuseFactor = saturate(dot(surfaceNormal, surfaceToLightDirection));
    float4 diffuse = diffuseFactor * AmbientColour * LightColour;
    
    // SPECULAR LIGHTING
    float4 specular = 0;
    if (diffuseFactor > 0.0f)
    {        
        float3 surfaceToCameraDirection = normalize(CameraPosition - input.position.rgb);
        float3 reflectedLightDirection = reflect(-surfaceToLightDirection, surfaceNormal);
    
        float specularFactor = pow(saturate(dot(reflectedLightDirection, surfaceToCameraDirection)), 16);
        specular = specularFactor * SpecularColour * SpecularIntensity;
    }
    
    input.color = ambient + diffuse + specular;
    //input.color = diffuse;
    return input.color;
}