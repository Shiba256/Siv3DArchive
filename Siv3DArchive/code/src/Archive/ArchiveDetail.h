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

		bool isRead() const noexcept;

		Array<FilePath> GetContents() const {
			Array<FilePath> contents;
			for (const auto& content : m_data) {
				contents << content.first;
			}
			return contents;
		}
	private:
		HashTable<FilePath, std::pair<size_t, size_t>> m_data;
		FilePathView m_archive_path;
		AES::Key m_aes_key;
		AES::IV m_aes_iv;
		AES::Manager m_aes_manager;

		Blob Encrypt(const Blob& plain) const;

		Blob Decrypt(const Blob& encrypt) const;

		AES::IV GenerateIV() const;
	};
}
