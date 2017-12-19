Texture2D shaderTexture;
SamplerState sampleType;

cbuffer VertexConstantBuffer : register(b0)
{
	float4x4 model;
	float4x4 view;
	float4x4 proj;
};

struct VOut
{
	float4 position : SV_POSITION;
	float4 tex : TEXCOORD0;
};


VOut VShader(float4 position : POSITION, float2 tex: TEXCOORD0)
{
	VOut output;

	position.w = 1.0f;

	output.position = mul(position, model);
	output.position = mul(output.position, view);
	output.position = mul(output.position, proj);
	output.tex = tex;

	return output;
}


float4 PShader(float4 position : SV_POSITION, float2 tex: TEXCOORD0) : SV_TARGET
{
	float4 textureColor;

	textureColor = shaderTexture.Sample(sampleType, input.tex);

	return textureColor;
}
