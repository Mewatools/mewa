#include"VectorType.hlsli"



float4 BasicPS(BasicType input) : SV_TARGET
{
    float4 vColor = input.col;
    float2 fsUVT = input.uv;
    float inside = sign(vColor.a - 0.5); // + to draw outside, - to draw inside

    float2 dx = ddx_fine(fsUVT);
    float2 dy = ddy_fine(fsUVT);
      float fx = (2.0*fsUVT.x)*dx.x - dx.y;
      float fy = (2.0*fsUVT.x)*dy.x - dy.y;
      float sdf = (fsUVT.x*fsUVT.x - fsUVT.y)/sqrt(fx*fx + fy*fy);
     float alpha = smoothstep(inside, -inside, sdf);


    return float4(vColor.rgb, alpha);
	//return float4(tex.Sample(smp,input.uv)) * input.col;
}
