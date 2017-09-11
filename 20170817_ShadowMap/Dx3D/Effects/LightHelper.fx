struct DirectionalLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Direction;
    float pad;
};

struct Material
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;//w = SpecPower
    float4 Reflect;
};

void ComputeDirectionalLight(Material mat,
                             DirectionalLight L,
                             float3 normal,
                             float3 toEye,
                             out float4 ambient,
                             out float4 diffuse,
                             out float4 specular)
{
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float3 vecLight = -L.Direction;
    float diffuseFactor = dot(normal, vecLight);
    ambient = mat.Ambient * L.Ambient;

    [flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 reflected = reflect(L.Direction, normal);
        float specFactor = pow(dot(reflected, toEye), mat.Specular.w);

        diffuse = mat.Diffuse * L.Diffuse * diffuseFactor;
        specular = mat.Specular * L.Specular * specFactor;
    }
}