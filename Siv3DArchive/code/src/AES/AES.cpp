#include "AES.h"
#include "AESDetail.h"

namespace AES {
	Manager::Manager(const Key& key, Mode mode)
		: m_key(key), m_mode(mode), p_impl(std::make_shared<Detail>(reinterpret_cast<const uint8*>(key.data()))) {}

	Blob Manager::encrypt(const Blob& plainText, const IV& iv) const {
		auto plainArray = plainText.asArray();
		uint8 paddingSize = 16 - (plainArray.size() % 16);
		paddingSize %= 16;
		plainArray.insert(plainArray.end(), paddingSize, static_cast<Byte>(paddingSize));

		Blob padding_plainText{ plainArray };
		Blob cipherText(padding_plainText.size());
		IV workingIV = iv;

		for (size_t i = 0; i < cipherText.size(); i += 16) {
			uint8 block[16] = { 0 };

			size_t blockSize = std::min<size_t>(16, padding_plainText.size() - i);
			std::memcpy(block, padding_plainText.data() + i, blockSize);

			if (m_mode == Mode::ECB) {
				p_impl->encryptBlock(block);
			}
			else if (m_mode == Mode::CBC) {
				for (size_t j = 0; j < 16; ++j) {
					block[j] ^= static_cast<uint8>(workingIV[j]);
				}
				p_impl->encryptBlock(block);
				std::memcpy(workingIV.data(), block, 16);
			}
			else if (m_mode == Mode::CFB) {
				uint8 temp[16];
				std::memcpy(temp, workingIV.data(), 16);
				p_impl->encryptBlock(temp);

				for (size_t j = 0; j < 16; ++j) {
					block[j] ^= temp[j];
				}
				std::memcpy(workingIV.data(), block, 16);
			}
			else if (m_mode == Mode::CTR) {
				uint8 counter[16];
				std::memcpy(counter, workingIV.data(), 16);
				p_impl->encryptBlock(counter);

				for (size_t j = 0; j < 16; ++j) {
					block[j] ^= counter[j];
				}

				for (int j = 15; j >= 0; --j) {
					workingIV[j] = Byte(AsUint8(workingIV[j]) + 1);
					if (AsUint8(workingIV[j]) != 0) break;
				}
			}

			std::memcpy(cipherText.data() + i, block, 16);
		}

		return cipherText;
	}

	Blob Manager::decrypt(const Blob& cipherText, const IV& iv) const {
		Blob plainText(cipherText.size());
		IV workingIV = iv;

		for (size_t i = 0; i < cipherText.size(); i += 16) {
			uint8 block[16];
			std::memcpy(block, cipherText.data() + i, 16);

			if (m_mode == Mode::ECB) {
				p_impl->decryptBlock(block);
			}
			else if (m_mode == Mode::CBC) {
				uint8 prevIV[16];
				std::memcpy(prevIV, workingIV.data(), 16);
				p_impl->decryptBlock(block);

				for (size_t j = 0; j < 16; ++j) {
					block[j] ^= prevIV[j];
				}

				std::memcpy(workingIV.data(), cipherText.data() + i, 16);
			}
			else if (m_mode == Mode::CFB) {
				uint8 temp[16];
				std::memcpy(temp, workingIV.data(), 16);
				p_impl->encryptBlock(temp);

				for (size_t j = 0; j < 16; ++j) {
					block[j] ^= temp[j];
				}
				std::memcpy(workingIV.data(), cipherText.data() + i, 16);
			}
			else if (m_mode == Mode::CTR) {
				uint8 counter[16];
				std::memcpy(counter, workingIV.data(), 16);
				p_impl->encryptBlock(counter);

				for (size_t j = 0; j < 16; ++j) {
					block[j] ^= counter[j];
				}

				for (int j = 15; j >= 0; --j) {
					workingIV[j] = Byte(AsUint8(workingIV[j]) + 1);
					if (AsUint8(workingIV[j]) != 0) break;
				}
			}

			std::memcpy(plainText.data() + i, block, 16);
		}

		auto plainArray = plainText.asArray();
		if (!plainText.empty()) {
			uint8_t paddingValue = AsUint8(plainArray.back());
			if (0 < paddingValue && paddingValue <= 16) {
				plainArray.resize(plainArray.size() - paddingValue);
			}
		}
		return Blob(plainArray);
	}
}
