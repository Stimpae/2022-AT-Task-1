[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
}

cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	float4x4 model;
	float4x4 view;
	float4x4 projection;
};

struct VertexShaderInput
{
	float3 pos : POSITION;
	float3 tex : TEXCOORD0;
};

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 tex : TEXCOORD0;
};

PixelShaderInput vsmain(VertexShaderInput input)
{
	PixelShaderInput output;
	float4 pos = float4(input.pos, 1.0f);

	pos = mul(pos, model);
	pos = mul(pos, view);
	pos = mul(pos, projection);
	output.pos = pos;

	output.tex = input.tex;

	return output;
}