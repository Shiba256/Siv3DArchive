#pragma once

#include"AES.h"
#include"Siv3DArchive.h"

#include"stdafx.h"

namespace s3a {
	class ArchiveReader::ArchiveReaderDetail {
	public:
		ArchiveReaderDetail(FilePathView archive_path, AES::Key key, AES::Mode mode);

		void ReadHeader();

		MemoryReader Load(FilePathView path) const;

		bool isOpen() const noexcept;

		bool isEmpty() const noexcept;

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
		AES::Manager m_aes_manager;

		Blob Decrypt(const Blob& encrypt) const;
	};
}
