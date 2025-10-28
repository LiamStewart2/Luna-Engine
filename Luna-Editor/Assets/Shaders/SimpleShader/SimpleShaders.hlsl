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

Texture2D textureTest : register(t0);
Texture2D specularTest : register(t1);
Texture2D shadowMap : register(t2);

SamplerState samplerTest : register(s0);
SamplerState specularSampler : register(s1);
SamplerState shadowSampler : register(s2);

struct VS_Out
{
    float4 position : SV_POSITION;
    float2 textureCoord : TEXCOORD;
    float4 color : COLOR;
    float3 normal : NORMAL0;
    float3 worldSpacePosition : POSITION0;
    float4 FragPositionLightSpace : POSITION1;
};

VS_Out VS_main(float3 Position : POSITION, float2 TextureCoordinate : TEXTURECOORD, float3 Normal : NORMAL)
{   
    VS_Out output = (VS_Out)0;
    
    float4 worldPos = mul(World, float4(Position, 1.0f));
    float4 viewPos = mul(View, worldPos);
    output.position = mul(Projection, viewPos);
    output.worldSpacePosition = worldPos;
    output.color = AmbientColour;
    
    
    output.FragPositionLightSpace = mul(lightSpaceMatrix, float4(output.worldSpacePosition, 1.0));
    
    
    output.normal = mul(World, float4(Normal, 0));
    
    output.textureCoord = TextureCoordinate;
    //output.normal = Normal;
    return output;
}

float Shadow(VS_Out input)
{
    float3 projectionCoords = input.FragPositionLightSpace.xyz / input.FragPositionLightSpace.w;
    projectionCoords = projectionCoords * 0.5 + 0.5;
    float closestDepth = shadowMap.Sample(shadowSampler, projectionCoords.xy).r;
    float currentDepth = projectionCoords.z;
    
    if(currentDepth > closestDepth)
        return 0.5;
    return 0;
}

float4 PS_main(VS_Out input) : SV_TARGET
{
    float3 surfaceNormal = normalize(input.normal);
    float3 surfaceToLightDirection = normalize(-LightDirection);
    float3 lightToSurfaceDirection = normalize(LightDirection);
    float3 viewDirection = normalize(CameraPosition - input.worldSpacePosition.xyz);
    // AMBIENT LIGHTING
    float4 ambient = AmbientColour * AmbientIntensity;
    
    //DIFFUSE LIGHTING
    float diffuseFactor = saturate(dot(surfaceNormal, surfaceToLightDirection));
    float4 diffuse = diffuseFactor * AmbientColour * LightColour;
    
    // SPECULAR LIGHTING
    float4 specular = 0;     
    float3 reflectedLightDirection = reflect(lightToSurfaceDirection, surfaceNormal);
    
    float specularDot = max(0.0, dot(viewDirection, reflectedLightDirection));

    float specularFactor = pow(specularDot, 32);
    specular = specularFactor * SpecularColour * (1 - (specularTest.Sample(specularSampler, input.textureCoord)));

    float4 col = textureTest.Sample(samplerTest, input.textureCoord);
    input.color = col * ((1.0 - Shadow(input)) * (diffuse + specular) + ambient);
    //input.color = float4(specularTest.Sample(specularSampler, input.textureCoord));
    
    //input.color = float4(normalize(input.position.xyz), 1);
    //input.color = diffuse;
    //input.color = float4(specular.rgb, 1);
    
    return input.color;
}