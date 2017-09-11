extern float4x4 matrixPalette[35];
int numBoneInfluences;
float4x4 matViewProj;
float3 lightPos;
texture texDiffuse;
sampler2D diffuseSampler = sampler_state
{
    Texture = (texDiffuse);
};

struct VS_INPUT_SKIN
{
    float4 position : POSITION0;
    float3 normal : NORMAL;
    float2 tex0 : TEXCOORD0;
    float4 weights : BLENDWEIGHT0;
    int4 boneIndices : BLENDINDICES0;
};

struct VS_OUTPUT
{
    float4 position : POSITION0;
    float2 tex0 : TEXCOORD0;
    float shade : TEXCOORD1;
};

VS_OUTPUT vs_Skinning(VS_INPUT_SKIN Input)
{
    VS_OUTPUT Output = (VS_OUTPUT) 0;
    float4 p = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float3 norm = float3(0.0f, 0.0f, 0.0f);
    float lastWeight = 0.0f;
    int n = numBoneInfluences - 1;
    Input.normal = normalize(Input.normal);
	
    for (int i = 0; i < n; ++i)
    {
        lastWeight += Input.weights[i];
        p += Input.weights[i] * mul(Input.position, matrixPalette[Input.boneIndices[i]]);
        norm += Input.weights[i] * mul(Input.normal, matrixPalette[Input.boneIndices[i]]);
    }

    lastWeight = 1.0f - lastWeight;
    p += lastWeight * mul(Input.position, matrixPalette[Input.boneIndices[n]]);
    norm += lastWeight * mul(Input.normal, matrixPalette[Input.boneIndices[n]]);
    p.w = 1.0f;

    Output.position = mul(p, matViewProj);
    Output.tex0 = Input.tex0;

    norm = normalize(norm);

    Output.shade = max(dot(norm, normalize(lightPos - p)), 0.2f);
    return Output;
}

float4 ps_Skinning(VS_OUTPUT Input) : COLOR0
{
    float4 diffuse = Input.shade * tex2D(diffuseSampler, Input.tex0);
    return diffuse;
}

technique SkinningTech
{
    pass Pass0
    {
        VertexShader = compile vs_3_0 vs_Skinning();
        PixelShader = compile ps_3_0 ps_Skinning();
    }
}