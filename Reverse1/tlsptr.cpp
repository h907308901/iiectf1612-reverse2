#include "all.h"
#define ASSERT_CURTHREAD() if (m_ThreadId != GetCurrentThreadId()) fatal("TLS cross-thread access")

TlsPtr::TlsPtr() {
	m_ThreadId = GetCurrentThreadId();
	m_TlsIndexesCount = TLSPTR_INIT_INDEXES;
	m_TlsIndexes = new unsigned int[m_TlsIndexesCount];
	for (unsigned int i = 0; i < m_TlsIndexesCount; i++)
		m_TlsIndexes[i] = TlsAlloc();
}

TlsPtr::~TlsPtr() {
	ASSERT_CURTHREAD();
	for (unsigned int i = 0; i < m_TlsIndexesCount; i++)
		TlsFree(m_TlsIndexes[i]);
	delete[] m_TlsIndexes;
}

bool TlsPtr::Store(unsigned int index, void* ptr) {
	ASSERT_CURTHREAD();
	if (index >= m_TlsIndexesCount) {
		if (index >= 0x10000)
			fatal("TLS index boundary error");
		unsigned int* m_TlsIndexesOld = m_TlsIndexes;
		unsigned int m_TlsIndexesCountOld = m_TlsIndexesCount;
		while (index >= m_TlsIndexesCount)
			m_TlsIndexesCount *= 2;
		m_TlsIndexes = new unsigned int[m_TlsIndexesCount];
		memcpy(m_TlsIndexes, m_TlsIndexesOld, m_TlsIndexesCountOld * sizeof(void*));
		for (unsigned int i = m_TlsIndexesCountOld; i < m_TlsIndexesCount; i++)
			m_TlsIndexes[i] = TlsAlloc();
		delete[] m_TlsIndexesOld;
	}
	return TlsSetValue(m_TlsIndexes[index], EncodePointer(ptr)) != 0;
}

void* TlsPtr::Fetch(unsigned int index) {
	ASSERT_CURTHREAD();
	if (index >= m_TlsIndexesCount)
		fatal("TLS index boundary error");
	return DecodePointer(TlsGetValue(m_TlsIndexes[index]));
}