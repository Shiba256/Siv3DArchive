#pragma once

#include"stdafx.h"
#include"Key.h"
#include"Mode.h"

namespace s3a{
	void Create(FilePathView archive_path, FilePathView directory_path, AES::Key key, AES::Mode mode = AES::Mode::CTR);
	class ArchiveReader {
	public:
		SIV3D_NODISCARD_CXX20
		ArchiveReader(FilePathView archive_path, AES::Key key, AES::Mode mode = AES::Mode::CTR);

		MemoryReader load(FilePathView path);

		Array<FilePath> getContents();

		explicit operator bool() const noexcept;

		bool isOpen() const noexcept;

		bool isEmpty() const noexcept;

		bool open(FilePathView archive_path, AES::Key key, AES::Mode mode = AES::Mode::CTR);

		void close();

	private:
		class ArchiveReaderDetail;
		std::shared_ptr<ArchiveReaderDetail> p_impl;
	};

}
