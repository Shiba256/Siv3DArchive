#include"Key.h"

namespace AES {
	Key::Key(std::array<Byte, KEY_SIZE>&& data):
		m_data(std::move(data))
	{}

	Key::Key(std::array<uint8, KEY_SIZE>&& data){
		std::memcpy(m_data.data(), data.data(), data.size());
	}

	Key::Key(std::initializer_list<Byte> lists):
		m_data()
	{
		if (lists.size() != 16ull)throw Error{ U"AES::Key Error!\nThe size of \"std::initializer_list<Byte>\" is assumed to be 16" };
		std::copy(lists.begin(), lists.end(), m_data.begin());
	}
	
	Key::Key(std::initializer_list<uint8> lists) :
		m_data()
	{
		if (lists.size() != 16ull)throw Error{ U"AES::Key Error!\nThe size of \"std::initializer_list<uint8>\" is assumed to be 16" };
		size_t i = 0ull;
		for (auto itr = lists.begin(); itr != lists.end();++itr) {
			m_data[i++] = Byte{ *itr };
		}
	}


	Key& Key::operator =(const std::array<Byte, KEY_SIZE>& other) {
		m_data = other;
		return *this;
	}

	Key& Key::operator =(std::array<Byte, KEY_SIZE>&& other) noexcept {
		m_data = std::move(other);
		return *this;
	}

	Key& Key::operator =(const Key& other) {
		m_data = other.m_data;
		return *this;
	}

	Key& Key::operator =(Key&& other) noexcept {
		m_data = std::move(other.m_data);
		return *this;
	}

	std::string Key::base64() const {
		std::string result;
		Base64::Encode(m_data.data(), this->size_bytes(), result);
		return result;
	}

	String Key::base64Str() const {
		String result;
		Base64::Encode(m_data.data(), this->size_bytes(), result);
		return result;
	}

	void Key::base64(std::string& dst) const {
		Base64::Encode(m_data.data(), this->size_bytes(), dst);
	}

	void Key::base64(String& dst) const {
		Base64::Encode(m_data.data(), this->size_bytes(), dst);
	}

	Key Key::GenerateRandomKey() {
		Key key;
		for (auto& byte : key) {
			byte = Byte(RandomUint8());
		}
		return key;
	}
}
