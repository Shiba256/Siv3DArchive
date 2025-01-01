#include"ArchiveWriterDetail.h"

namespace s3a {
	constexpr size_t META_DATA_SIZE{ 2 * sizeof(size_t) + +AES::BLOCK_SIZE };

	ArchiveWriterDetail::ArchiveWriterDetail(FilePathView archive_path,AES::Key key, AES::Mode mode) :
		m_archive_path(archive_path),
		m_aes_key(key),
		m_aes_manager(m_aes_key, mode)
	{}

	void ArchiveWriterDetail::Create(FilePathView path) const {
		Serializer<BinaryWriter> writer{ m_archive_path };
		const auto directory_contents = FileSystem::DirectoryContents(path).filter(FileSystem::IsFile);
		const auto aes_iv = GenerateIV();

		writer << size_t(0ull) << directory_contents.size();
		writer->write(aes_iv.data(), aes_iv.size_bytes());

		Array<std::pair<FilePath, size_t>> header_data;
		size_t header_pos = META_DATA_SIZE;

		for (const auto& content : directory_contents) {
			const auto content_path = FileSystem::RelativePath(content);
			Blob blob{ content_path };
			const auto encrypt_binary = Encrypt(blob);
			const auto cmp_encrypt_binary = Compression::Compress(encrypt_binary, Compression::MaxLevel);
			writer->write(cmp_encrypt_binary.data(), cmp_encrypt_binary.size_bytes());
			header_data << std::pair{ content_path, cmp_encrypt_binary.size_bytes() };
			header_pos += cmp_encrypt_binary.size_bytes();
		}

		const auto temp = writer->getPos();
		writer->setPos(0ull);
		writer << header_pos;
		writer->setPos(temp);

		for (const auto& data : header_data) {
			const auto& content = data.first;
			const auto& file_size = data.second;
			const auto content_path = FileSystem::RelativePath(content);
			Blob content_path_blob{ content_path.data(),content_path.size_bytes() };
			Blob file_size_blob{ &file_size,sizeof(file_size) };
			const auto encrypted_content_path = m_aes_manager.encrypt(content_path_blob, aes_iv);
			const auto encrypted_file_size = m_aes_manager.encrypt(file_size_blob, aes_iv);
			const auto cmp_encrypted_content_path = Compression::Compress(encrypted_content_path, Compression::MaxLevel);
			const auto cmp_encrypted_file_size = Compression::Compress(encrypted_file_size, Compression::MaxLevel);

			writer << cmp_encrypted_content_path.base64() << cmp_encrypted_file_size.base64();
		}
	}

	Blob ArchiveWriterDetail::Encrypt(const Blob& plain) const {
		AES::IV iv = GenerateIV();
		Blob encrypted = m_aes_manager.encrypt(plain, iv);
		size_t i = 0ull;
		for (auto& byte : iv) {
			if (i == m_aes_key.size())i = 0;
			byte ^= m_aes_key[i++];
		}
		for (const auto& byte : iv) {
			encrypted.append(&byte, sizeof(byte));
		}
		return encrypted;
	}


	AES::IV ArchiveWriterDetail::GenerateIV() const {
		return AES::IV::GenerateRandomKey();
	}
}
