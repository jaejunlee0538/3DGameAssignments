#include "LightHelper.fx"

DirectionalLight gDirectionalLights[3];
int gNumDirectionalLights;

Material gMaterial;
float4 gEyePosW : ViewPosition;

float4x4 gWorldMatrix : World;
float4x4 gViewMatrix : View;
float4x4 gProjMatrix : Projection;

struct VS_INPUT
{
    float4 positionL : POSITION;
    float3 normalL : NORMAL;
};

struct VS_OUTPUT
{
    float4 position : POSITION;
    float4 positionW : TEXCOORD1;
    float3 normalW : TEXCOORD2;
};

struct PS_INPUT
{
    float4 positionW : TEXCOORD1;
    float3 normalW : TEXCOORD2;
};

VS_OUTPUT VS_Main(VS_INPUT Input)
{
    VS_OUTPUT Output;

    float4 posW = mul(Input.positionL, gWorldMatrix);
    Output.position = mul(posW, gViewMatrix);
    Output.position = mul(Output.position, gProjMatrix);
    Output.positionW = posW;
    Output.normalW = mul(Input.normalL, (float3x3) gWorldMatrix);
    return Output;
}

float4 PS_Main(PS_INPUT Input) : COLOR
{
    float3 normal = normalize(Input.normalW);
    float3 toEye = normalize(gEyePosW - Input.positionW);
    float4 litColor = float4(0, 0, 0, 1);
    if (gNumDirectionalLights > 0)
    {
        float4 ambient = float4(0, 0, 0, 0);
        float4 diffuse = float4(0, 0, 0, 0);
        float4 specular = float4(0, 0, 0, 0);
        [flatten]
        for (int i = 0; i < gNumDirectionalLights; ++i)
        {
            float4 A, D, S;
            ComputeDirectionalLight(gMaterial, gDirectionalLights[i], normal, toEye, A, D, S);   
            ambient += A;
            diffuse += D;
            specular += S;
        }

        litColor = ambient + diffuse + specular;
    }
    return litColor;
}

technique BasicLight{
    pass P0
    {
        VertexShader = compile vs_3_0 VS_Main();
        PixelShader = compile ps_3_0 PS_Main();
    }
}