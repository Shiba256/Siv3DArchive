#pragma once
#include"AES.h"

namespace AES {
	class Key;
	enum class Mode;
}

namespace Archive {
	class ArchiveDetail {
	public:
		ArchiveDetail(AES::Key key, AES::Mode mode);

		void Create(FilePathView path) const;

		void ReadHeader();

		MemoryReader Load(FilePathView path) const;

		void setArchivePath(FilePathView path);

		constexpr bool isRead() const noexcept {
			return static_cast<bool>(header_size);
		}
	private:
		HashTable<FilePath, std::pair<size_t, size_t>> data;
		size_t header_size = 0ull;
		FilePathView archive_path = U"";
		AES::Key aes_key;
		AES::IV aes_iv;
		AES::Manager aes_manager;

		Blob Encrypt(FilePathView before) const;

		Blob Decrypt(const Array<Byte>& bytes) const;

		AES::IV GenerateIV() const;
	};
}
