# Siv3D Archive

�f�B���N�g���\����ێ����AAES-128�ňÍ������ꂽ�t�@�C���A�[�J�C�u���쐬�ł��܂��B�܂��A�A�[�J�C�u���̃f�[�^�𒼐ړǂݍ��ނ��Ƃ��\�ł��B

---

## �t�@�C���A�[�J�C�u�̍쐬

�t�@�C���A�[�J�C�u���쐬����ɂ́A`s3a::Create`�֐����g�p���A�Í����p�̌��ɂ�`AES::Key`���g�p���܂��BAES�̃��[�h��`AES::Mode`�Ŏw��ł��A`CBC`,`CTR`,`CFB`,`ECB`��4��ނ���������Ă��܂��B�ȗ����̓f�t�H���g��`CTR`���[�h���g�p����܂��B
����Base64�ŃG���R�[�h/�f�R�[�h�ł���̂ŕ�����Ƃ��Ĉ����������Ɏg�p���܂��B
```C++
// ���������_���ɐ���
const auto key = AES::Key::GenerateRandomKey();

// �ݒ�iAES�̃��[�h�͏ȗ��\�B�f�t�H���g��CTR���[�h�j
s3a::Create(U"Archive.cmp",U"engine", key);
```

---

## �t�@�C���A�[�J�C�u����̓ǂݍ���
`s3a::ArchiveReader::load`�֐��ŃA�[�J�C�u����f�[�^�𒼐ړǂݍ��ނ��Ƃ��ł��܂��B���̂Ƃ��A�A�[�J�C�u�̃J�����g�f�B���N�g���́A`Create`�֐������s�����ۂ̃f�B���N�g���Ɠ����ɂȂ�܂��B

`load`�֐���`MemoryReader`��ԋp����̂ŁA`TextReader`��`Texture`�Ȃǂ̃R���X�g���N�^��`MemoryReader`��n�����ƂŒ��ړǂݍ��ނ��Ƃ��ł��܂��B

```C++
// �A�[�J�C�u�t�@�C���̍쐬
s3a::ArchiveReader archive{ U"archive.cmp",key };

// �e�L�X�g�̓ǂݍ���
TextReader reader{ archive.load(U"engine/font/fontawesome/LICENSE.txt") };
```

---
