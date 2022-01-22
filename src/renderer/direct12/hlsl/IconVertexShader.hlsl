#include"IconType.hlsli"

cbuffer cbuff0 : register(b0) {
	matrix mat;
};

IconType IconVS(float2 pos : POSITION, float4 col:COLOR, float2 uv:TEXCOORD) {
	IconType output;
	output.svpos = mul(mat, float4(pos.x, pos.y, 0.0, 1.0) );
	output.uv = uv;
	output.col = col;
	return output;
}
