#pragma once

// This is used to store pointers, and they can be fetched by corresponding identifiers (as indexes)

const int TLSPTR_INIT_INDEXES = 32;

class TlsPtr {
private:
	unsigned int m_ThreadId;
	unsigned int* m_TlsIndexes;
	unsigned int m_TlsIndexesCount;
public:
	TlsPtr();
	~TlsPtr();
	bool Store(unsigned int index, void* ptr);
	void* Fetch(unsigned int index);
};