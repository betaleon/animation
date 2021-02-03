
Texture2D g_Texture : register(t0);
Texture2D g_TextureShadowDepth : register(t1);//----�V���h�E�}�b�v
SamplerState g_SamplerState : register(s0);

void main(//in PS_IN In, out float4 outDiffuse : SV_Target
	in  float4 inPosition		: SV_POSITION,
	in  float4 inShadowPosition	: POSITION1,
	in  float4 inNormal : NORMAL0,
	in  float2 inTexCoord : TEXCOORD0,
	in  float4 inDiffuse : COLOR0,

	out float4 outDiffuse : SV_Target0
) 
{
	//���ʂ̃e�N�X�`���̐F���o��
	outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord);
	outDiffuse *= inDiffuse;	//�F=���ʂɃe�N�X�`��*���邳�̏���

	//In.ShadowPosition.xyz��In.ShadowPosition.w�Ŋ��遨���ʂ͂��̂܂�In.ShadowPosition.xyz��
	//�������̂͏��������邽�߂̒�������
	inShadowPosition.xyz /= inShadowPosition.w;	//���W�l-1.0~1.0�̒l�ɂȂ�
	//�ϊ�����In.ShadowPosition.xy���e�N�X�`�����W(0.0~1.0)�֕ϊ�����
	inShadowPosition.x = inShadowPosition.x *0.5f + 0.5f;
	inShadowPosition.y = -inShadowPosition.y *0.5f + 0.5f;//Y�͏㉺�𔽓]����̂ŕ�����-�ɂ���

	//�V���h�E�o�b�t�@(�e�N�X�`��)���炱�̃s�N�Z���̌����ڂ̌�������̋������擾����
	float depth = g_TextureShadowDepth.Sample(g_SamplerState, inShadowPosition.xy);

	//�ϊ������s�N�Z���̋���(In.ShadowPosition.z)�ƃV���h�E�o�b�t�@�̌����ڂ̋���(depth)���r����
	//�����ڂ̋������v�Z��̋���()��菬������΁A���̃s�N�Z���͉e�̒��ɂ���=�F(Diffuse,rgb)���Â��Ȃ�
	//<��������낤>
	//if���ŕ��ʂɍ���OK
	
	if (inShadowPosition.z -0.01f< depth)
	{

	}
	else
	{
		outDiffuse.rgb *= 0.5f;
	}

	//�s�N�Z�����e�ɂȂ��Ă�����F(Diffuse.rgb)���Â����� 0.5�{���炢�H

	outDiffuse.a = 1.0f;	//����

}