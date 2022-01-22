#include"IconType.hlsli"
Texture2D<float4> tex:register(t0);
SamplerState smp:register(s0);


float4 IconPS(IconType input ) : SV_TARGET
{
	return float4(tex.Sample(smp,input.uv)) * input.col;
}
