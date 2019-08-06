cbuffer matrices
{
  float4x4 mvp;
  float4x4 modelMat;
  float4x4 modelMatInvTrans;
}

struct vsInput
{
  float4 vColor : COLOR;
  float3 vNormal : NORMAL;
  float3 vPosition : POSITION;
  float2 vTexCoord : TEXCOORD;
  float3 vTangent : TANGENT;
  float3 vBitangent : BINORMAL;
};

struct vsOutput
{
  float4 position : SV_Position;
  float3 worldPos : COLOR0;
  float4 color : COLOR1;
  float3 normal : NORMAL;
  float3 tangent : TANGENT;
  float3 bitangent : BINORMAL;
  float2 texCoord : TEXCOORD;
};

vsOutput main(vsInput In)
{
  vsOutput Out;
  float4 pos4 = float4(In.vPosition, 1.0);
  Out.position = mul(mvp, pos4);
  Out.worldPos = (float3)mul(modelMat, pos4);
  Out.color = In.vColor;
  Out.texCoord = In.vTexCoord;
  Out.normal = normalize(mul(In.vNormal, (float3x3)modelMatInvTrans));
  Out.tangent = normalize(mul(In.vTangent, (float3x3)modelMatInvTrans));
  Out.bitangent = normalize(mul(In.vBitangent, (float3x3)modelMatInvTrans));
  return Out;
}
