#include<Siv3D.hpp>
#include"Siv3DArchive.h"

void Main() {
	// 鍵の生成
	const auto key = AES::Key::GenerateRandomKey();

	// アーカイブの入出力先と鍵を設定
	Archive::Setting(U"archive.cmp", key);

	// engine ディレクトリのアーカイブを作成
	Archive::Create(U"engine");

	// アーカイブファイルから読み込む
	// アーカイブのパス及び鍵は Archive::Setting で指定したものが使われる
	TextReader reader{ Archive::Load(U"engine/font/fontawesome/LICENSE.txt") };

	Console << reader.readAll();
	
	while (System::Update()) {}
}
