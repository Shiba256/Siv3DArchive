namespace AES {
	inline const Byte& Key::operator[](const size_t index) const {
		return m_data[index];
	}

	inline Byte& Key::operator[](const size_t index) {
		return m_data[index];
	}

	[[nodiscard]]
	inline const Byte* Key::data() const noexcept {
		return m_data.data();
	}

	[[nodiscard]]
	inline Byte* Key::data() noexcept {
		return m_data.data();
	}

	inline const std::array<Byte, KEY_SIZE>& Key::asArray() const noexcept {
		return m_data;
	}

	inline bool Key::empty() const noexcept {
		return m_data.empty();
	}

	inline bool Key::isEmpty() const noexcept {
		return m_data.empty();
	}

	inline size_t Key::size() const noexcept {
		return m_data.size();
	}

	inline size_t Key::size_bytes() const noexcept {
		return sizeof(m_data);
	}


	inline size_t Key::max_size() const noexcept {
		return m_data.max_size();
	}

	inline Key::iterator Key::begin() noexcept {
		return m_data.begin();
	}

	inline Key::iterator Key::end() noexcept {
		return m_data.end();
	}

	inline Key::const_iterator Key::begin() const noexcept {
		return m_data.begin();
	}

	inline Key::const_iterator Key::end() const noexcept {
		return m_data.end();
	}

	inline Key::const_iterator Key::cbegin() const noexcept {
		return m_data.cbegin();
	}

	inline Key::const_iterator Key::cend() const noexcept {
		return m_data.cend();
	}

	inline Key::reverse_iterator Key::rbegin() noexcept {
		return m_data.rbegin();
	}

	inline Key::reverse_iterator Key::rend() noexcept {
		return m_data.rend();
	}

	inline Key::const_reverse_iterator Key::rbegin() const noexcept {
		return m_data.rbegin();
	}

	inline Key::const_reverse_iterator Key::rend() const noexcept {
		return m_data.rend();
	}

	inline Key::const_reverse_iterator Key::crbegin() const noexcept {
		return m_data.crbegin();
	}

	inline Key::const_reverse_iterator Key::crend() const noexcept {
		return m_data.crend();
	}
}
