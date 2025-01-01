# Siv3D Archive

ディレクトリ構造を保持し、AES-128で暗号化されたファイルアーカイブを作成できます。また、アーカイブ内のデータを直接読み込むことも可能です。

---

## ファイルアーカイブの作成

ファイルアーカイブを作成するには、`s3a::Create`関数を使用し、暗号化用の鍵には`AES::Key`を使用します。AESのモードは`AES::Mode`で指定でき、`CBC`,`CTR`,`CFB`,`ECB`の4種類が実装されています。省略時はデフォルトで`CTR`モードが使用されます。
鍵はBase64でエンコード/デコードできるので文字列として扱いたい時に使用します。
```C++
// 鍵をランダムに生成
const auto key = AES::Key::GenerateRandomKey();

// 設定（AESのモードは省略可能。デフォルトはCTRモード）
s3a::Create(U"Archive.cmp",U"engine", key);
```

---

## ファイルアーカイブからの読み込み
`s3a::ArchiveReader::load`関数でアーカイブからデータを直接読み込むことができます。このとき、アーカイブのカレントディレクトリは、`Create`関数を実行した際のディレクトリと同じになります。

`load`関数は`MemoryReader`を返却するので、`TextReader`や`Texture`などのコンストラクタに`MemoryReader`を渡すことで直接読み込むことができます。

```C++
// アーカイブファイルの作成
s3a::ArchiveReader archive{ U"archive.cmp",key };

// テキストの読み込み
TextReader reader{ archive.load(U"engine/font/fontawesome/LICENSE.txt") };
```

---
