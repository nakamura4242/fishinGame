#include "SceneBase.h"
#include "DxLib.h"

class SceneChange :public SceneBase
{
public:
	SceneChange();				//	�R���X�g���N�^
	~SceneChange();				//	�f�X�g���N�^

	SceneBase* Update(float _deltaTime)override;	//	�X�V
	void Draw()override;			//	�`��
	void Sound()override;			//	���y
	void Load()override;			//	������
	void DebugKey();
	
	void FeedIn();
	void FeedOut();

private:
	class Player* m_player;			//	�v���C���[�N���X�ւ̃|�C���^�����o�ϐ�
	class Target* m_target[11];			//
	class Camera* m_camera;			//	�J�����N���X�ւ̃|�C���^�����o�ϐ�
	class Mark* m_mark;				//	�}�[�N�N���X�ւ̃|�C���^�����o�ϐ�
	class UI* m_score_ui[10];		//  UI�N���X�ւ̃|�C���^�����o�ϐ�
	class UI* m_hit_ui[10];			//	�q�b�g����UI�N���X�ւ̃|�C���^�����o�ϐ�
	class PlayEffect* m_effect;     //  �G�t�F�N�g�v���[���[

};

