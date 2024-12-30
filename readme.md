# Siv3D Archive

ディレクトリ構造を保持し、AES-128で暗号化されたファイルアーカイブを作成できます。また、アーカイブ内のデータを直接読み込むことも可能です。

---

## ファイルアーカイブの作成

ファイルアーカイブを作成するには、`Archive::Setting`関数を使用し、アーカイブの出力先、暗号化用の鍵、AESのモードを指定します。暗号化用の鍵には`AES::Key`を使用します。AESのモードは`AES::Mode`で指定でき、現在`CBC`と`CTR`の2種類が実装されています。省略時はデフォルトで`CTR`モードが使用されます。

```C++
// 鍵をランダムに生成
const auto key = AES::Key::GenerateRandomKey();

// 設定（AESのモードは省略可能。デフォルトはCTRモード）
Archive::Setting(U"Archive.cmp", key);
```

`Create(FilePathView path)`を使用して、指定したディレクトリをアーカイブに変換します。pathにはアーカイブを作成したいディレクトリのパスを指定します。

```C++
// アーカイブの作成
Create(U"engine");
```

---

## ファイルアーカイブからの読み込み

ファイルアーカイブを使用する際は`Archive::Setting`関数でアーカイブの入力先、暗号化用の鍵、AESのモードを指定します。

その後、`Load(FilePathView path)`でアーカイブからデータを直接読み込むことができます。このとき、アーカイブのカレントディレクトリは、`Create`関数を実行した際のディレクトリと同じになります。

`Load`関数は`MemoryReader`を返却するので、`TextReader`や`Texture`などのコンストラクタに`MemoryReader`を渡すことで直接読み込むことができます。

```C++
// アーカイブの読み込み
Archive::Load(U"engine/font/fontawesome/LICENSE.txt");

// TextReaderを使ってファイルアーカイブから読み込む
TextReader text_reader{ Archive::Load(U"engine/font/fontawesome/LICENSE.txt") };
```

---
