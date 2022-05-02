#ifndef _SCENEMGR_H_
#define _SCENEMGR_H_

#include "DxLib.h"

class SceneMgr
{
public:
	SceneMgr();			//	�R���X�g���N�^
	~SceneMgr();		//	�f�X�g���N�^

	void Update(float _deltaTime);		//	�X�V
	void Draw();						//	�`��
	void Sound();						//�@���y

	void SetScene(class SceneBase* _Scene);		//	�V�[�����Z�b�g����				

private:
	class SceneBase* m_scene;					//	�V�[���x�[�X�ւ̃|�C���^�����o�ϐ�
};

#endif // _SCENEMGR_H_
