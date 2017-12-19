cbuffer VertexConstantBuffer : register(b0)
{
	float4x4 model;
	float4x4 view;
	float4x4 proj;
};

struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};


VOut VShader(float4 position : POSITION, float4 color : COLOR)
{
	VOut output;

	output.position = mul(position, model);
	output.position = mul(output.position, view);
	output.position = mul(output.position, proj);
	output.color = color;

	return output;
}


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
	return color;
}
