#pragma once

#include"constants.h"
#include"Key.h"
#include"Mode.h"

#include"stdafx.h"

namespace AES {
	using IV = Key;

	class Manager {
	public:
		Manager(const Key& key, Mode mode);

		Blob encrypt(const Blob& plainText, const IV& iv) const;

		Blob decrypt(const Blob& cipherText, const IV& iv) const;

	private:
		struct Detail;
		Mode m_mode;
		Key m_key;
		std::shared_ptr<Detail> p_impl;
	};
}
