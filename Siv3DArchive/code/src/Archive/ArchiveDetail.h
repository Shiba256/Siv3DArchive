#pragma once

#include"AES.h"

#include"stdafx.h"
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
			return static_cast<bool>(m_header_size);
		}
	private:
		HashTable<FilePath, std::pair<size_t, size_t>> m_data;
		size_t m_header_size = 0ull;
		FilePathView m_archive_path = U"";
		AES::Key m_aes_key;
		AES::IV m_aes_iv;
		AES::Manager m_aes_manager;

		Blob Encrypt(FilePathView before) const;

		Blob Decrypt(const Array<Byte>& bytes) const;

		AES::IV GenerateIV() const;
	};
}
