cbuffer Constants : register(b0)
{
    matrix worldViewProjection;
    matrix world;
    float4 lightDirection;
    float4 tint;
    float4 options;
};

Texture2D diffuseTexture : register(t0);
SamplerState diffuseSampler : register(s0);

struct VSInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    float4 color : COLOR;
};

PSInput VSMain(VSInput input)
{
    PSInput output;
    output.position = mul(float4(input.position, 1.0f), worldViewProjection);
    output.normal = normalize(mul(float4(input.normal, 0.0f), world).xyz);
    output.texcoord = input.texcoord;
    output.color = input.color * tint;
    return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    float light = saturate(dot(normalize(input.normal), normalize(-lightDirection.xyz))) * 0.75f + 0.25f;
    float4 baseColor = input.color;
    if (options.x > 0.5f)
    {
        baseColor *= diffuseTexture.Sample(diffuseSampler, input.texcoord);
    }
    baseColor.rgb *= light;
    return baseColor;
}
