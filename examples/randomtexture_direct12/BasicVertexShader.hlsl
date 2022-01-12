#include"BasicType.hlsli"

cbuffer cbuff0 : register(b0) {
	matrix mat;
};

BasicType BasicVS(float4 pos : POSITION,float2 uv:TEXCOORD) {
	BasicType output;
	output.svpos = mul(mat, pos);
	output.uv = uv;
	return output;
}
