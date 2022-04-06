[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
}

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 color : COLOR0;
};

float4 psmain(PixelShaderInput input) : SV_TARGET
{
	return float4(input.color, 1.0f);
}