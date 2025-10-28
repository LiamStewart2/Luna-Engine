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
    
    
    output.FragPositionLightSpace = mul(lightSpaceMatrix, worldPos);
    
    
    output.normal = mul(World, float4(Normal, 0));
    
    output.textureCoord = TextureCoordinate;
    //output.normal = Normal;
    return output;
}

float Shadow(VS_Out input)
{
    float2 projCoords = (float2) 0;
    projCoords = projCoords * 0.5f + 0.5f;

    projCoords.x = input.FragPositionLightSpace.x / input.FragPositionLightSpace.w / 2.0f + 0.5f;
    projCoords.y = -input.FragPositionLightSpace.y / input.FragPositionLightSpace.w / 2.0f + 0.5f;
    
    // if outside shadow map bounds
    if (projCoords.x < 0.0f || projCoords.x > 1.0f || projCoords.y < 0.0f || projCoords.y > 1.0f)
        return 0.0f;
    float currentDepth = input.FragPositionLightSpace.z / input.FragPositionLightSpace.w;
    float shadow = 0;
    
    float width = 0;
    float height = 0;
    shadowMap.GetDimensions(width, height);

    float2 texelSize = 1.0 / float2(width, height);
    
    for (int x = -2; x <= 2; ++x)
    {
        for (int y = -2; y <= 2; ++y)
        {
            float2 coordSample = float2(x, y);
            
            float closestDepth = shadowMap.Sample(shadowSampler, projCoords + (coordSample * texelSize)).r;
            shadow += currentDepth > closestDepth ? 1.0 : 0.0;
        }

    }
    shadow /= 25;
    
    return shadow;
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
    //input.color = (float4) Shadow(input);
    //input.color.a = 1;
    return input.color;
}