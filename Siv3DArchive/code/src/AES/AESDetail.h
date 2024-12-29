#pragma once
#include"AES.h"

namespace AES {
	struct Manager::Detail {
	public:
		Detail(const uint8_t* key);

		void encryptBlock(uint8_t* block);

		void decryptBlock(uint8_t* block);

	private:
		std::array<uint8_t, BLOCK_SIZE* (NUM_ROUNDS + 1)> roundKeys;
		
		void keyExpansion(const uint8_t* key);

		void addRoundKey(uint8_t* block, size_t round);
		void subBytes(uint8_t* block);
		void shiftRows(uint8_t* block);
		void mixColumns(uint8_t* block);

		void invSubBytes(uint8_t* block);
		void invShiftRows(uint8_t* block);
		void invMixColumns(uint8_t* block);

		static uint8_t sBox(uint8_t value);
		static uint8_t invSBox(uint8_t value);
		static uint8_t mul(uint8_t a, uint8_t b);
	};
}

