#pragma once

#include"stdafx.h"
#include"Key.h"
#include"Mode.h"

namespace Archive {
	/// @brief pathで指定されたディレクトリのアーカイブを作成します
	/// @param path アーカイブを作成したいディレクトリのパス
	void Create(FilePathView path);

	/// @brief アーカイブファイルからコンテンツを読み取ります
	/// @param path アーカイブファイル内でのパス
	/// @return 取得したデータ
	MemoryReader Load(FilePathView path);

	/// @brief アーカイブの設定を行います。アーカイブの機能を使う際は最初に呼び出す必要があります。
	/// @param path アーカイブファイルの入出力パス
	/// @param key アーカイブファイルの暗号鍵
	/// @param mode AES暗号のモード
	void Setting(FilePathView path, AES::Key key, AES::Mode mode = AES::Mode::CTR);

	/// @brief アーカイブに含まれるファイルパスを全て返します
	/// @return アーカイブに含まれるファイルパス
	Array<FilePath> GetContents();
}
