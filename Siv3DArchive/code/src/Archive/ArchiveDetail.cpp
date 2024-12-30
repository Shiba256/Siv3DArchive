#include"ArchiveDetail.h"

namespace Archive {
	ArchiveDetail::ArchiveDetail(AES::Key key, AES::Mode mode) :
			m_aes_key(key),
			m_aes_iv(GenerateIV()),
			m_aes_manager(m_aes_key, mode)
		{}

	void ArchiveDetail::Create(FilePathView path) const{
			Serializer<BinaryWriter> writer{ m_archive_path };
			const auto directory_contents = FileSystem::DirectoryContents(path);
			writer << size_t(0ull);
			writer << directory_contents.filter(FileSystem::IsFile).size();
			writer->write(m_aes_iv.data(), m_aes_iv.size_bytes());

			Array<std::pair<FilePath, size_t>> header_data;
			size_t header_pos = sizeof(size_t) * 2 + 16ull;
			for (const auto& content : directory_contents) {
				const auto content_path = FileSystem::RelativePath(content);
				if (FileSystem::IsFile(content_path)) {
					const auto encrypt_binary = Encrypt(content_path);
					const auto cmp_encrypt_binary = Compression::Compress(encrypt_binary, Compression::MaxLevel);
					writer->write(cmp_encrypt_binary.data(), cmp_encrypt_binary.size_bytes());
					header_data << std::pair{ content_path, cmp_encrypt_binary.size_bytes() };
					header_pos += cmp_encrypt_binary.size_bytes();
				}
			}
			const auto temp = writer->getPos();
			writer->setPos(0ull);
			writer << header_pos;
			writer->setPos(temp);
			for (const auto& data : header_data) {
				const auto& content = data.first;
				const auto& file_size = data.second;
				const auto content_path = FileSystem::RelativePath(content);
				if (FileSystem::IsFile(content_path)) {
					Blob content_path_blob{ content_path.data(),content_path.size_bytes() };
					Blob file_size_blob{ &file_size,sizeof(file_size) };
					const auto encrypted_content_path = m_aes_manager.encrypt(content_path_blob, m_aes_iv);
					const auto encrypted_file_size = m_aes_manager.encrypt(file_size_blob, m_aes_iv);
					const auto cmp_encrypted_content_path = Compression::Compress(encrypted_content_path, Compression::MaxLevel);
					const auto cmp_encrypted_file_size = Compression::Compress(encrypted_file_size, Compression::MaxLevel);

					writer << cmp_encrypted_content_path.base64() << cmp_encrypted_file_size.base64();
				}
			}
		}

	void ArchiveDetail::ReadHeader() {
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
			m_header_size = reader->getPos() - header_pos;
		}

	MemoryReader ArchiveDetail::Load(FilePathView path) const {
		if (m_data.contains(path)) {
			BinaryReader reader{ m_archive_path };
			reader.setPos(sizeof(size_t) * 2 + 16ull + m_data.at(path).first);
			Blob bytes;
			for ([[maybe_unused]] const auto& i : step(m_data.at(path).second)) {
				Byte byte{};
				reader.read(&byte, sizeof(Byte));
				bytes.append(&byte, sizeof(Byte));
			}
			const auto decmp_bytes_array = Compression::Decompress(bytes).asArray();
			return MemoryReader{ Decrypt(decmp_bytes_array) };
		}
		else {
			return MemoryReader{};
		}
	}

	void ArchiveDetail::setArchivePath(FilePathView path) {
		m_archive_path = path;
	}

	Blob ArchiveDetail::Encrypt(FilePathView before) const {
			BinaryReader reader{ before };
			Blob plain;
			Byte reader_byte;
			while (reader.read(&reader_byte, sizeof(reader_byte))) {
				plain.append(&reader_byte, sizeof(reader_byte));
			}
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

	Blob ArchiveDetail::Decrypt(const Array<Byte>& bytes) const {
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

	AES::IV ArchiveDetail::GenerateIV() const {
		return AES::IV::GenerateRandomKey();
	}
}
