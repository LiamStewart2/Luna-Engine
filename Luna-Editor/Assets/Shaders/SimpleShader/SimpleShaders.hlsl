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
    float3 worldSpacePosition : POSITION0;
};

VS_Out VS_main(float3 Position : POSITION, float2 TextureCoordinate : TEXTURECOORD, float3 Normal : NORMAL)
{   
    VS_Out output = (VS_Out)0;
    
    float4 worldPos = mul(World, float4(Position, 1.0f));
    float4 viewPos = mul(View, worldPos);
    output.position = mul(Projection, viewPos);
    output.worldSpacePosition = worldPos;
    output.color = AmbientColour;
    
    output.normal = mul(World, float4(Normal, 0));
    //output.normal = Normal;
    return output;
}

float4 PS_main(VS_Out input) : SV_TARGET
{
    float3 surfaceNormal = normalize(input.normal);
    float3 surfaceToLightDirection = normalize(-LightDirection);
    float3 lightToSurfaceDirection = normalize(LightDirection);
    
    // AMBIENT LIGHTING
    float4 ambient = AmbientColour * AmbientIntensity;
    
    //DIFFUSE LIGHTING
    float diffuseFactor = saturate(dot(surfaceNormal, surfaceToLightDirection));
    float4 diffuse = diffuseFactor * AmbientColour * LightColour;
    
    // SPECULAR LIGHTING
    float4 specular = 0;     
    float3 surfaceToCameraDirection = normalize(CameraPosition - input.worldSpacePosition.xyz);
    float3 reflectedLightDirection = (reflect(lightToSurfaceDirection, surfaceNormal));
    
    float specularDot = dot(reflectedLightDirection, surfaceToCameraDirection);
    if(specularDot > 0)
    {
        float specularFactor = pow(specularDot, 32);
        specular = specularFactor * SpecularColour * SpecularIntensity;
    }
    
    input.color = ambient + diffuse + specular;
    //input.color = float4(normalize(input.position.xyz), 1);
    //input.color = diffuse;
    //input.color = specular;
    return input.color;
}