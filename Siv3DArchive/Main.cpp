#include<Siv3D.hpp>
#include"Siv3DArchive.h"

void Main() {
	// 鍵の生成
	const String key_str = AES::Key::GenerateRandomKey().base64Str();

	Console << U"key " << key_str;

	const AES::Key key = AES::Key::FromBase64Str(key_str);

	// engine ディレクトリのアーカイブを作成
	s3a::Create(U"archive.cmp",U"engine", key, AES::Mode::ECB);

	// アーカイブファイルから読み込む
	s3a::ArchiveReader archive{ U"archive.cmp",key,AES::Mode::ECB };
	TextReader reader{ archive.load(U"engine/font/fontawesome/LICENSE.txt") };

	Console << reader.readAll();
	
	while (System::Update()) {}
}
