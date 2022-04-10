Texture2D Texture: register(t0);
sampler TextureSampler: register(s0);

[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
}

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 psmain(PixelShaderInput input) : SV_TARGET
{
	return Texture.Sample(TextureSampler, input.tex);
}