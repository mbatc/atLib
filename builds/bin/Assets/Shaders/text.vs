cbuffer matrices
{
  float4x4 mvp;
}

struct vsInput
{
  float4 vColor : COLOR;
  float3 vPosition : POSITION;
  float2 vTexCoord : TEXCOORD;
};

struct vsOutput
{
  float4 position : SV_Position;
  float2 texCoord : TEXCOORD;
  float4 color : COLOR;
};

vsOutput main(vsInput In)
{
  vsOutput Out;
  Out.position = mul(mvp, float4(In.vPosition, 1.0));
  Out.texCoord = In.vTexCoord;
  Out.color = In.vColor;
  return Out;
}
