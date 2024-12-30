# Siv3D Archive

�f�B���N�g���\����ێ����AAES-128�ňÍ������ꂽ�t�@�C���A�[�J�C�u���쐬�ł��܂��B�܂��A�A�[�J�C�u���̃f�[�^�𒼐ړǂݍ��ނ��Ƃ��\�ł��B

---

## �t�@�C���A�[�J�C�u�̍쐬

�t�@�C���A�[�J�C�u���쐬����ɂ́A`Archive::Setting`�֐����g�p���A�A�[�J�C�u�̏o�͐�A�Í����p�̌��AAES�̃��[�h���w�肵�܂��B�Í����p�̌��ɂ�`AES::Key`���g�p���܂��BAES�̃��[�h��`AES::Mode`�Ŏw��ł��A����`CBC`��`CTR`��2��ނ���������Ă��܂��B�ȗ����̓f�t�H���g��`CTR`���[�h���g�p����܂��B

```C++
// ���������_���ɐ���
const auto key = AES::Key::GenerateRandomKey();

// �ݒ�iAES�̃��[�h�͏ȗ��\�B�f�t�H���g��CTR���[�h�j
Archive::Setting(U"Archive.cmp", key);
```

`Create(FilePathView path)`���g�p���āA�w�肵���f�B���N�g�����A�[�J�C�u�ɕϊ����܂��Bpath�ɂ̓A�[�J�C�u���쐬�������f�B���N�g���̃p�X���w�肵�܂��B

```C++
// �A�[�J�C�u�̍쐬
Create(U"engine");
```

---

## �t�@�C���A�[�J�C�u����̓ǂݍ���

�t�@�C���A�[�J�C�u���g�p����ۂ�`Archive::Setting`�֐��ŃA�[�J�C�u�̓��͐�A�Í����p�̌��AAES�̃��[�h���w�肵�܂��B

���̌�A`Load(FilePathView path)`�ŃA�[�J�C�u����f�[�^�𒼐ړǂݍ��ނ��Ƃ��ł��܂��B���̂Ƃ��A�A�[�J�C�u�̃J�����g�f�B���N�g���́A`Create`�֐������s�����ۂ̃f�B���N�g���Ɠ����ɂȂ�܂��B

`Load`�֐���`MemoryReader`��ԋp����̂ŁA`TextReader`��`Texture`�Ȃǂ̃R���X�g���N�^��`MemoryReader`��n�����ƂŒ��ړǂݍ��ނ��Ƃ��ł��܂��B

```C++
// �A�[�J�C�u�̓ǂݍ���
Archive::Load(U"engine/font/fontawesome/LICENSE.txt");

// TextReader���g���ăt�@�C���A�[�J�C�u����ǂݍ���
TextReader text_reader{ Archive::Load(U"engine/font/fontawesome/LICENSE.txt") };
```

---
