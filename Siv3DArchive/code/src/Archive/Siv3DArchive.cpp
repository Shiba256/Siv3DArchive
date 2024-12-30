#include"Siv3DArchive.h"
#include"AES.h"
#include"ArchiveDetail.h"

namespace Archive {
	inline std::unique_ptr<ArchiveDetail> p_impl = nullptr;

	void Create(FilePathView path) {
		if (p_impl) {
			p_impl->Create(path);
		}
	}

	void Setting(FilePathView path, AES::Key key,AES::Mode mode) {
		if (p_impl) {
			p_impl.release();
		}
		p_impl = std::make_unique<ArchiveDetail>(key, mode);
		p_impl->setArchivePath(path);
	}

	MemoryReader Load(FilePathView path) {
		if (p_impl) {
			if (not p_impl->isRead()) {
				p_impl->ReadHeader();
			}
			return p_impl->Load(path);
		}
		else {
			return MemoryReader{};
		}
	}

	Array<FilePath> GetContents() {
		if (p_impl) {
			if (not p_impl->isRead()) {
				p_impl->ReadHeader();
			}
			return p_impl->GetContents();
		}
		return {};
	}
}


