#include"Siv3DArchive.h"
#include"AES.h"
#include"ArchiveWriterDetail.h"
#include"ArchiveReaderDetail.h"

namespace s3a {
	void Create(FilePathView archive_path, FilePathView directory_path, AES::Key key, AES::Mode mode) {
		s3a::ArchiveWriterDetail{ archive_path,key,mode }.Create(directory_path);
	}

	ArchiveReader::ArchiveReader(FilePathView archive_path, AES::Key key, AES::Mode mode) :
		p_impl(std::make_shared<ArchiveReaderDetail>(archive_path, key, mode))
	{
		if (p_impl)p_impl->ReadHeader();
	}

	MemoryReader ArchiveReader::load(FilePathView path) {
		return p_impl->Load(path);
	}

	Array<FilePath> ArchiveReader::getContents() {
		return p_impl->GetContents();
	}

	ArchiveReader::operator bool() const noexcept {
		return p_impl->isOpen();
	}

	bool ArchiveReader::isOpen() const noexcept {
		return p_impl->isOpen();
	}

	bool ArchiveReader::open(FilePathView archive_path, AES::Key key, AES::Mode mode) {
		if (p_impl)p_impl.reset();
		p_impl = std::make_shared<ArchiveReaderDetail>(archive_path, key, mode);
		if (p_impl)p_impl->ReadHeader();
		return p_impl->isOpen();
	}

	bool ArchiveReader::isEmpty() const noexcept {
		return p_impl->isEmpty();
	}


	void ArchiveReader::close() {
		if (p_impl)p_impl.reset();
	}

}

