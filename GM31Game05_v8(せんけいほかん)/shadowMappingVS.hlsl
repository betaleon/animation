// �}�g���N�X�o�b�t�@
cbuffer WorldBuffer : register(b0)
{
	matrix World;
}
cbuffer ViewBuffer : register(b1)
{
	matrix View;
}
cbuffer ProjectionBuffer : register(b2)
{
	matrix Projection;
}

//���C�g�I�u�W�F�N�g�\���̂ƃR���X�^���g�o�b�t�@
struct LIGHT
{
	bool Enable;
	bool3 Dummy;		//4�̔{���ɂ���ƌ����������̂Œ����p�A����C����ł���������VS2017������Ă���Ă���
	float4 Direction;
	float4 Diffuse;
	float4 Ambient;

	matrix View;		//���C�g�̍s��
	matrix Projection;	//����
};

cbuffer LightBuffer : register(b4)	//�R���X�^���g�o�b�t�@4�ԂƂ���
{
	LIGHT Light;			//���C�g�\����
}


void main(
	in  float4 inPosition		: POSITION0,
	in	float4 inShadowPosition : POSITION1,
	in  float4 inNormal : NORMAL0,
	in  float4 inDiffuse : COLOR0,
	in  float2 inTexCoord : TEXCOORD0,

	out float4 outPosition : SV_POSITION,
	out float4 outShadowPosition : POSITON1,
	out float4 outNormal : NORMAL0,
	out float2 outTexCoord : TEXCOORD0,
	out float4 outDiffuse : COLOR0
)
{
	matrix wvp;
	wvp = mul(World, View);					//1path�ڂ�View=Light�s�� 2path��View=Camera�s��
	wvp = mul(wvp, Projection);				//�ϊ��s��쐬
	//--�����܂Œ��--

	float4 worldNormal, normal;				//�@�������[���h�s��ŉ�]
	normal = float4(inNormal.xyz, 0.0f);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);	//�ϊ������@���𐳋K��

	//--�����܂ŃZ�b�g--
	//�T���v���ł̓s�N�Z���V�F�[�_�[����P���ɂ��邽��
	//�����v�Z�𒸓_�ł��
	float light = -dot(Light.Direction.xyz, worldNormal.xyz);
	light = saturate(light);	//���邳��0~1�ɂ���
	outDiffuse = light;
	outDiffuse.a = inDiffuse.a;

	outPosition = mul(inPosition, wvp);		//���_�����[���h�r���[�v���W�F�N�V�����ϊ����ďo��
	outNormal = worldNormal;				//���[���h�ϊ������@�����o��
	outTexCoord = inTexCoord;				//�e�N�X�`�����W���o��

	//--�����܂ŕ��ʂ̒��_�ϊ�--
	//1path�ځ@�V���h�E�}�b�v�쐬
	//�J����&�v���W�F�N�V�����s�񂪃��C�g�̕��ɂȂ��Ă���B
	//���C�g���猩����Ԃ������_�����O���邱�ƂɂȂ�(�[�x�o�b�t�@�쐬�̂�)
	//����path�ł�ShadowPosition�ւ̏o�͈͂Ӗ����Ȃ�
	//2path�ځ@�{�`��@�e��
	//�ʏ�̃J�������猩����Ԃ������_�����O����
	//����path�ł�ShadowPosition�̓��C�g���猩���ڂ̍��W��\�����Ƃ���
	//�s�N�Z���V�F�[�_�[�ŗ��p�����
	matrix lightwvp;//-----VS�ł͂������厖�@���C�g=�J�����Ƃ����ϊ��s����쐬
	lightwvp = mul(World, Light.View);//
	lightwvp = mul(lightwvp, Light.Projection);

	outShadowPosition = mul(inPosition, lightwvp);//--���C�g���猩�����_�̍��W���o��

}