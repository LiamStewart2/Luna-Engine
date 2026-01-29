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

struct VS_Out
{
    float4 position                 : SV_POSITION;
    
    float2 textureCoord             : TEXCOORD;
    float3 normal                   : TEXCOORD1;
    float3 worldSpacePosition       : TEXCOORD2;
    float4 FragPositionLightSpace   : TEXCOORD3;
    
    float3 Tangent                  : TEXCOORD4;
    float3 Binormal                 : TEXCOORD5;
};

VS_Out VS_main(float3 Position : POSITION, float2 TextureCoordinate : TEXTURECOORD, float3 Normal : NORMAL, float3 Tangent : TANGENT, float3 Bitangent : BITANGENT)
{
    VS_Out output = (VS_Out) 0;
    
    float4 worldPos = mul(World, float4(Position, 1.0f));
    float4 viewPos = mul(View, worldPos);
    output.position = mul(Projection, viewPos);
    output.worldSpacePosition = worldPos;
    
    
    output.FragPositionLightSpace = mul(lightSpaceMatrix, worldPos);
    
    output.textureCoord = TextureCoordinate;
    
    
    float3x3 worldmat3 = (float3x3) World;
    output.normal = normalize(mul(worldmat3, Normal));
    output.Tangent = normalize(mul(worldmat3, Tangent));
    output.Binormal = normalize(mul(worldmat3, Bitangent));
    
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

// Approximates the surfaces roughness of microfacets
float DistributionGGX(float3 N, float3 H, float roughness)
{
    float PI = 3.14159265359;
    
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / max(denom, 0.0001); // Add epsilon to prevent division by zero
}

// Approximates how the microfacets block the light from the perspective of the camera
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    
    float denominator = NdotV * (1.0 - k) + k;
    return NdotV / denominator;
}
float GeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    
    return ggx1 * ggx2;
}


float3 FresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0f - F0) * pow(clamp(1.0f - cosTheta, 0.0f, 1.0f), 5.0f);
}

float4 PS_main(VS_Out input) : SV_TARGET
{
    float3 albedo = albedoMap.Sample(albedoSampler, input.textureCoord);
    albedo = pow(albedo, 2.2);
    float roughness = roughnessMap.Sample(roughnessSampler, input.textureCoord).r; // Inverted because testing with glossy materials
    roughness = clamp(roughness, 0.04, 1.0);
    float metallic = metallicMap.Sample(metallicSampler, input.textureCoord).r;
    float AO = aoMap.Sample(aoSampler, input.textureCoord).r;
    
    float PI = 3.14159265359;
    
    float3 N = NormalMapping(input);
    float3 V = normalize(CameraPosition - input.worldSpacePosition);
    
    float3 F0 = 0.04;
    F0 = lerp(F0, albedo, metallic);
    
    float3 Lo = (float3)0.0f;
    
    // Calculate per-light radiance (in future will support multiple lights)
    float3 L = normalize(-LightDirection);
    float3 H = normalize(V + L); // the halfway vector
    float3 radiance = LightColour;
    
    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    float3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

    float3 numerator = NDF * G * F;
    float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0001);
    float3 specular = numerator / denominator;
    
    float3 kS = F; // specular is equal to the fresnel
    float3 kD = (float3) 1.0f - kS; // specular + diffuse = 1, so diffuse = 1 - specular
    kD *= 1.0 - metallic; // pure metals have no diffuse factor
    
    
    float3 diffuse = kD * albedo;
    
    Lo += (diffuse + specular) * radiance * max(dot(N, L), 0.0);

    
    float3 ambient = (float3) 0.03 * albedo * AO;
    float3 color = ambient + Lo;
    
    color = color / (color + (float3) 1.0); // HDR tonemapping
    color = pow(color, (float3) 1.0 / 2.2); // gamma correct
    
    float shadow = Shadow(input);
    color *= 1.0 - shadow;
    
    return float4(color.rgb, 1.0);
}