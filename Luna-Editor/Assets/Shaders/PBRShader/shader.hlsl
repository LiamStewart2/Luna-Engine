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

Texture2D albedoMap : register(t0);
Texture2D roughnessMap : register(t1);
Texture2D shadowMap : register(t2);
Texture2D normalMap : register(t3);
Texture2D metallicMap : register(t4);
Texture2D aoMap : register(t5);

SamplerState albedoSampler : register(s0);
SamplerState roughnessSampler : register(s1);
SamplerState shadowSampler : register(s2);
SamplerState normalSampler : register(s3);
SamplerState metallicSampler : register(s4);
SamplerState aoSampler : register(s5);

float PI = 3.14159265359;

struct VS_Out
{
    float4 position : SV_POSITION;
    float2 textureCoord : TEXCOORD;
    float4 color : COLOR;
    float3 normal : NORMAL0;
    float3 worldSpacePosition : POSITION0;
    float4 FragPositionLightSpace : POSITION1;
    
    float3 Tangent : TANGENT0;
    float3 Binormal : BINORMAL0;
};

VS_Out VS_main(float3 Position : POSITION, float2 TextureCoordinate : TEXTURECOORD, float3 Normal : NORMAL, float3 Tangent : TANGENT, float3 Bitangent : BITANGENT)
{
    VS_Out output = (VS_Out) 0;
    
    float4 worldPos = mul(World, float4(Position, 1.0f));
    float4 viewPos = mul(View, worldPos);
    output.position = mul(Projection, viewPos);
    output.worldSpacePosition = worldPos;
    output.color = AmbientColour;
    
    
    output.FragPositionLightSpace = mul(lightSpaceMatrix, worldPos);
    //output.position = mul(lightSpaceMatrix, worldPos);
    
    
    
    output.textureCoord = TextureCoordinate;
    
    
    output.normal = mul(World, float4(Normal, 0));
    output.Tangent = normalize(mul(World, float4(Tangent, 0)));
    output.Binormal = normalize(mul(World, float4(Bitangent, 0)));
    
    return output;
}

float3 NormalMapping(VS_Out input)
{
    float4 normalMapSample = normalMap.Sample(normalSampler, input.textureCoord);
    normalMapSample = (normalMapSample * 2.0f) - 1.0f;
    
    float3 N = (normalMapSample.x * input.Tangent) + (normalMapSample.y * input.Binormal) + (normalMapSample.z * input.normal);
    return normalize(N);
}

float Shadow(VS_Out input)
{
    float2 projCoords = (float2) 0;

    projCoords.x = (input.FragPositionLightSpace.x / input.FragPositionLightSpace.w) / 2.0f + 0.5f;
    projCoords.y = (-input.FragPositionLightSpace.y / input.FragPositionLightSpace.w) / 2.0f + 0.5f;

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

float DistributionGGX(float3 N, float3 H, float roughness)
{
    return 0.0f;
}
float GeometrySchlickGGX(float NdotV, float roughness)
{
    return 0.0f;
}
float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    return 0.0f;
}
float3 FresnelSchlick(float cosTheta, float3 F0)
{
    return 0.0f;
}

float4 PS_main(VS_Out input) : SV_TARGET
{
    float3 albedo = albedoMap.Sample(albedoSampler, input.textureCoord);
    float roughness = 1 - roughnessMap.Sample(roughnessSampler, input.textureCoord).r; // Inverted because testing with glossy materials
    float metallic = metallicMap.Sample(metallicSampler, input.textureCoord).r;
    float AO = aoMap.Sample(aoSampler, input.textureCoord).r;
    
    float3 lightPosition = -LightDirection; // pretty sure we need negative light direction, as we actually are wanting the light position
    
    float3 N = NormalMapping(input);
    float3 V = normalize(CameraPosition - input.worldSpacePosition);
    
    float3 F0 = 0.04f;
    F0 = lerp(F0, albedo, 0);
    
    float3 Lo = (float3)0.0f;
    
    float3 L = normalize(lightPosition - input.worldSpacePosition);
    float H = normalize(V + L); // the halfway vector
    float distance = length(lightPosition - input.worldSpacePosition);
    float attenuation = 1.0 / (distance * distance);
    float3 radiance = LightColour * attenuation;
    
    
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    float3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);
    
    float3 kS = F;
    float3 kD = (float3) 1.0f - kS;
    kD *= 1.0 - metallic;
    
    float3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    float3 specular = numerator / denominator;
    
    float NdotL = max(dot(N, L), 0.0);
    Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    
    
    float3 ambient = (float3) 0.03 * albedo * AO;
    float3 color = ambient + Lo;
    
    color = color / (color + (float3) 1.0);
    color = pow(color, (float3) 1.0 / 2.2);
    
    return float4(color.rgb, 1.0);
}