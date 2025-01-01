#pragma once

#include"constants.h"

#include"stdafx.h"

namespace AES {

	class Key {
	public:
		using base_type = std::array<Byte, KEY_SIZE>;
		using iterator = base_type::iterator;
		using const_iterator = base_type::const_iterator;
		using reverse_iterator = base_type::reverse_iterator;
		using const_reverse_iterator = base_type::const_reverse_iterator;

		SIV3D_NODISCARD_CXX20
		Key() = default;

		SIV3D_NODISCARD_CXX20
		Key(const Key&) = default;

		SIV3D_NODISCARD_CXX20
		Key(Key&&) = default;

		SIV3D_NODISCARD_CXX20
		explicit Key(std::array<Byte,KEY_SIZE>&& data);

		SIV3D_NODISCARD_CXX20
		explicit Key(std::array<uint8, KEY_SIZE>&& data);

		SIV3D_NODISCARD_CXX20
		Key(std::initializer_list<Byte> lists);

		SIV3D_NODISCARD_CXX20
		Key(std::initializer_list<uint8> lists);

		Key& operator =(const std::array<Byte, KEY_SIZE>& other);

		Key& operator =(std::array<Byte, KEY_SIZE>&& other) noexcept;

		Key& operator =(const Key& other);

		Key& operator =(Key&& other) noexcept;

		[[nodiscard]]
		bool operator ==(const Key& rhs) noexcept {
			return (this->asArray() == rhs.asArray());
		}

		[[nodiscard]]
		bool operator !=(const Key& rhs) noexcept {
			return (this->asArray() != rhs.asArray());
		}

		[[nodiscard]]
		const Byte& operator[](const size_t index) const;

		[[nodiscard]]
		Byte& operator[](const size_t index);

		[[nodiscard]]
		const Byte* data() const noexcept;

		[[nodiscard]]
		Byte* data() noexcept;

		const std::array<Byte, KEY_SIZE>& asArray() const noexcept;

		[[nodiscard]]
		bool empty() const noexcept;

		[[nodiscard]]
		bool isEmpty() const noexcept;

		[[nodiscard]]
		constexpr size_t size() const noexcept;

		[[nodiscard]]
		constexpr size_t size_bytes() const noexcept;

		[[nodiscard]]
		size_t max_size() const noexcept;

		[[nodiscard]]
		iterator begin() noexcept;

		[[nodiscard]]
		iterator end() noexcept;

		[[nodiscard]]
		const_iterator begin() const noexcept;

		[[nodiscard]]
		const_iterator end() const noexcept;

		[[nodiscard]]
		const_iterator cbegin() const noexcept;

		[[nodiscard]]
		const_iterator cend() const noexcept;

		[[nodiscard]]
		reverse_iterator rbegin() noexcept;

		[[nodiscard]]
		reverse_iterator rend() noexcept;

		[[nodiscard]]
		const_reverse_iterator rbegin() const noexcept;

		[[nodiscard]]
		const_reverse_iterator rend() const noexcept;

		[[nodiscard]]
		const_reverse_iterator crbegin() const noexcept;

		[[nodiscard]]
		const_reverse_iterator crend() const noexcept;

		[[nodiscard]]
		std::string base64() const;

		[[nodiscard]]
		String base64Str() const;

		void base64(std::string& dst) const;

		void base64(String& dst) const;

		static Key GenerateRandomKey();

		static Key FromBase64Str(const String& str) {
			const auto blob = Base64::Decode(str);
			Key key{};
			if (blob.size() != AES::KEY_SIZE) {
				throw Error{ U"Failed Parse String to AES::Key\nThe Key size is assumed to be 16 bytes" };
			}
			for (auto i : step(16ull)) {
				key[i] = blob[i];
			}
			return key;
		}

		static Key FromBase64(const std::string& str){
			const auto blob = Base64::Decode(str);
			Key key{};
			if (blob.size() != AES::KEY_SIZE) {
				throw Error{ U"Failed Parse String to AES::Key\nThe Key size is assumed to be 16 bytes" };
			}
			for (auto i : step(16ull)) {
				key[i] = blob[i];
			}
			return key;
		}

	private:
		base_type m_data;
	};

}

#include"./ipp/Key.ipp"
