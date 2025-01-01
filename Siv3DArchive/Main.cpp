#include<Siv3D.hpp>
#include"Siv3DArchive.h"

void Main() {
	// 鍵の生成
	const String key_str = AES::Key::GenerateRandomKey().base64Str();

	Console << U"key " << key_str;

	const AES::Key key = AES::Key::FromBase64Str(key_str);

	// アーカイブの入出力先と鍵を設定
	Archive::Setting(U"archive.cmp", key, AES::Mode::ECB);

	// engine ディレクトリのアーカイブを作成
	Archive::Create(U"engine");

	// アーカイブファイルから読み込む
	// アーカイブのパス及び鍵は Archive::Setting で指定したものが使われる
	TextReader reader{ Archive::Load(U"engine/font/fontawesome/LICENSE.txt") };

	Console << reader.readAll();
	
	while (System::Update()) {}
}
