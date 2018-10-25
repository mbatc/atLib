cbuffer matrices
{
  float4x4 mvp;
}

struct vsInput
{
  float4 vColor : COLOR;
  float3 vNormal : NORMAL;
  float3 vPosition : POSITION;
  float2 vTexCoord : TEXCOORD;
};

struct vsOutput
{
  float4 position : SV_Position;
  float4 color : COLOR;
  float3 normal : NORMAL;
  float2 texCoord : TEXCOORD;
};

vsOutput main(vsInput In)
{
  vsOutput Out;
  Out.position = mul(mvp, float4(In.vPosition, 1.0));
  Out.color = In.vColor;
  Out.texCoord = In.vTexCoord;
  Out.normal = In.vNormal;
  return Out;
}
