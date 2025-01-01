#include"ArchiveReaderDetail.h"

namespace s3a {
	constexpr size_t META_DATA_SIZE{ 2 * sizeof(size_t) + 16ull };

	ArchiveReader::ArchiveReaderDetail::ArchiveReaderDetail(FilePathView archive_path, AES::Key key, AES::Mode mode) :
		m_archive_path(archive_path),
		m_aes_key(key),
		m_aes_manager(m_aes_key, mode)
	{}

	void ArchiveReader::ArchiveReaderDetail::ReadHeader() {
		Deserializer<BinaryReader> reader{ m_archive_path };
		size_t directory_size = 0ull;
		size_t header_pos = 0ull;
		reader >> header_pos >> directory_size;
		AES::IV iv;
		reader->read(&iv, AES::BLOCK_SIZE);
		reader->setPos(header_pos);
		size_t binary_pos = 0ull;
		for ([[maybe_unused]] const auto& i : step(directory_size)) {
			std::string name_base64;
			std::string size_base64;
			reader >> name_base64 >> size_base64;
			constexpr size_t size_of_String_value_type{ sizeof(String::value_type) };
			Blob name_blob = Base64::Decode(name_base64);
			Blob decmp_name_blob = Compression::Decompress(name_blob);
			const auto decrypted_name = m_aes_manager.decrypt(decmp_name_blob, iv);
			size_t name_size = decrypted_name.size() / size_of_String_value_type;
			String name{ U"",name_size };
			std::memcpy(name.data(), decrypted_name.data(), decrypted_name.size_bytes());
			Blob size_blob = Base64::Decode(size_base64);
			Blob decmp_size_blob = Compression::Decompress(size_blob);
			const auto decrypted_size = m_aes_manager.decrypt(decmp_size_blob, iv);
			size_t size = 0ull;
			std::memcpy(&size, decrypted_size.data(), decrypted_size.size_bytes());
			m_data.emplace(name, std::pair{ binary_pos ,size });
			binary_pos += size;
		}
	}

	MemoryReader ArchiveReader::ArchiveReaderDetail::Load(FilePathView path) const {
		if (m_data.contains(path)) {
			BinaryReader reader{ m_archive_path };
			reader.setPos(META_DATA_SIZE + m_data.at(path).first);
			Blob bytes;
			for ([[maybe_unused]] const auto& i : step(m_data.at(path).second)) {
				Byte byte{};
				reader.read(&byte, sizeof(Byte));
				bytes.append(&byte, sizeof(Byte));
			}
			return MemoryReader{ Decrypt(Compression::Decompress(bytes)) };
		}
		else {
			return MemoryReader{};
		}
	}

	Blob ArchiveReader::ArchiveReaderDetail::Decrypt(const Blob& bytes) const {
		AES::IV iv;
		size_t i = 0ull;
		for (auto itr = bytes.end() - AES::BLOCK_SIZE; itr != bytes.end(); ++itr) {
			iv[i] = *itr;
			i++;
		}
		i = 0ull;
		for (auto& byte : iv) {
			if (i == m_aes_key.size())i = 0;
			byte ^= m_aes_key[i++];
		}
		Blob encrypted;
		for (auto itr = bytes.begin(); itr != bytes.end() - AES::BLOCK_SIZE; ++itr) {
			encrypted.append(&(*itr), sizeof(*itr));
		}
		return m_aes_manager.decrypt(encrypted, iv);
	}

	bool ArchiveReader::ArchiveReaderDetail::isOpen() const noexcept {
		return FileSystem::Exists(m_archive_path) && not m_data.empty();
	}

	bool ArchiveReader::ArchiveReaderDetail::isEmpty() const noexcept {
		return m_data.empty();
	}


}
