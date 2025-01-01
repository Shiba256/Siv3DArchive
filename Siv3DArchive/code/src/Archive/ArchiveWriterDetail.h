#pragma once

#include"AES.h"

#include"stdafx.h"

namespace s3a {
	class ArchiveWriterDetail {
	public:
		ArchiveWriterDetail(FilePathView archive_path, AES::Key key, AES::Mode mode);

		void Create(FilePathView path) const;

	private:
		FilePathView m_archive_path;
		AES::Key m_aes_key;
		AES::Manager m_aes_manager;

		Blob Encrypt(const Blob& plain) const;

		AES::IV GenerateIV() const;
	};
}
