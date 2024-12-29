#include"ArchiveDetail.h"

#include"AES.h"

namespace Archive {
	ArchiveDetail::ArchiveDetail(AES::Key key, AES::Mode mode) :
			aes_key(key),
			aes_iv(GenerateIV()),
			aes_manager(aes_key, mode)
		{}

	void ArchiveDetail::Create(FilePathView path) const{
			Serializer<BinaryWriter> writer{ archive_path };
			const auto directory_contents = FileSystem::DirectoryContents(path);
			writer << directory_contents.filter(FileSystem::IsFile).size();
			writer->write(aes_iv.data(), aes_iv.size_bytes());
			for (const auto& content : directory_contents) {
				const auto content_path = FileSystem::RelativePath(content);
				if (FileSystem::IsFile(content_path)) {
#ifdef _ARCHIVE_DEBUG_
					Console << U"encrypt " << content_path;
#endif // _ARCHIVE_DEBUG_
					auto file_size = FileSystem::FileSize(content_path);
					if (file_size) {
						file_size += AES::KEY_SIZE;
						file_size += (AES::KEY_SIZE - file_size % AES::KEY_SIZE) % AES::KEY_SIZE;
					}
					Blob content_path_blob{ content_path.data(),content_path.size_bytes() };
					Blob file_size_blob{ &file_size,sizeof(file_size) };
					const auto encrypted_content_path = aes_manager.encrypt(content_path_blob, aes_iv);
					const auto encrypted_file_size = aes_manager.encrypt(file_size_blob, aes_iv);
					writer << encrypted_content_path.base64() << encrypted_file_size.base64();
				}
			}
			for (const auto& content : directory_contents) {
				const auto content_path = FileSystem::RelativePath(content);
				if (FileSystem::IsFile(content_path)) {
					const auto encrypt_binary = Encrypt(content_path);
					for (const auto& byte : encrypt_binary) {
						writer->write(byte);
					}
				}
			}
		}

	void ArchiveDetail::ReadHeader() {
			Deserializer<BinaryReader> reader{ archive_path };
			size_t directory_size = 0ull;
			reader >> directory_size;
			AES::IV iv;
			reader->read(&iv, AES::BLOCK_SIZE);
			size_t binary_pos = 0ull;
			for ([[maybe_unused]] const auto& i : step(directory_size)) {
				std::string name_base64;
				std::string size_base64;
				reader >> name_base64 >> size_base64;

				constexpr size_t size_of_String_value_type{ sizeof(String::value_type) };

				Blob name_blob = Base64::Decode(name_base64);
				const auto decrypted_name = aes_manager.decrypt(name_blob, iv);
				size_t name_size = decrypted_name.size() / size_of_String_value_type;
				String name{ U"",name_size };
				std::memcpy(name.data(), decrypted_name.data(), decrypted_name.size_bytes());

				Blob size_blob = Base64::Decode(size_base64);
				const auto decrypted_size = aes_manager.decrypt(size_blob, iv);
				size_t size = 0ull;
				std::memcpy(&size, decrypted_size.data(), decrypted_size.size_bytes());
#ifdef _ARCHIVE_DEBUG_
				Console << U"ReadHeader " << name;
#endif // _ARCHIVE_DEBUG_
				data.emplace(name, std::pair{ binary_pos ,size });
				binary_pos += size;
			}
			header_size = reader->getPos();
		}

	MemoryReader ArchiveDetail::Load(FilePathView path) const {
			if (data.contains(path)) {
				BinaryReader reader{ archive_path };
				reader.setPos(header_size + data.at(path).first);
				Array<Byte> bytes;
				for ([[maybe_unused]] const auto& i : step(data.at(path).second)) {
					Byte byte{};
					reader.read(&byte, sizeof(Byte));
					bytes << byte;
				}
				return MemoryReader{ Decrypt(bytes) };
			}
			else {
				return MemoryReader{};
			}
		}

	void ArchiveDetail::setArchivePath(FilePathView path) {
			archive_path = path;
		}

	Blob ArchiveDetail::Encrypt(FilePathView before) const {
			BinaryReader reader{ before };
			Blob plain;
			Byte reader_byte;
			while (reader.read(&reader_byte, sizeof(reader_byte))) {
				plain.append(&reader_byte, sizeof(reader_byte));
			}
			AES::IV iv = GenerateIV();
			Blob encrypted = aes_manager.encrypt(plain, iv);
			size_t i = 0ull;
			for (auto& byte : iv) {
				if (i == aes_key.size())i = 0;
				byte ^= aes_key[i++];
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
				if (i == aes_key.size())i = 0;
				byte ^= aes_key[i++];
			}
			Blob encrypted;
			for (auto itr = bytes.begin(); itr != bytes.end() - AES::BLOCK_SIZE; ++itr) {
				encrypted.append(&(*itr), sizeof(*itr));
			}
			return aes_manager.decrypt(encrypted, iv);
		}

	AES::IV ArchiveDetail::GenerateIV() const {
		return AES::IV::GenerateRandomKey();
	}
}
