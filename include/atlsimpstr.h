namespace ATL {
struct CStringData;

__interface IAtlStringMgr {
 public:
  // Allocate a new CStringData
  _Ret_maybenull_ _Post_writable_byte_size_(
      sizeof(CStringData) + nAllocLength * nCharSize) CStringData*
  Allocate(_In_ int nAllocLength, _In_ int nCharSize) throw();
  // Free an existing CStringData
  void Free(_Inout_ CStringData * pData) throw();
  // Change the size of an existing CStringData
  virtual _Ret_maybenull_ _Post_writable_byte_size_(
      sizeof(CStringData) + nAllocLength * nCharSize) CStringData*
  Reallocate(_Inout_ CStringData * pData, _In_ int nAllocLength,
             _In_ int nCharSize) throw();
  // Get the CStringData for a Nil string
  CStringData* GetNilString() throw();
  IAtlStringMgr* Clone() throw();
};

struct CStringData {
  IAtlStringMgr* pStringMgr;  // String manager for this CStringData
  int nDataLength;   // Length of currently used data in XCHARs (not including
                     // terminating null)
  int nAllocLength;  // Length of allocated data in XCHARs (not including
                     // terminating null)
  long nRefs;        // Reference count: negative == locked
  // XCHAR data[nAllocLength+1]  // A CStringData is always followed in memory
  // by the actual array of character data

  void* data() throw() { return (this + 1); }

  void AddRef() throw() {
    ATLASSERT(nRefs > 0);
    _InterlockedIncrement(&nRefs);
  }
  bool IsLocked() const throw() { return nRefs < 0; }
  bool IsShared() const throw() { return (nRefs > 1); }
  void Lock() throw() {
    ATLASSERT(nRefs <= 1);
    nRefs--;  // Locked buffers can't be shared, so no interlocked operation
              // necessary
    if (nRefs == 0) {
      nRefs = -1;
    }
  }
  void Release() throw() {
    ATLASSERT(nRefs != 0);

    if (_InterlockedDecrement(&nRefs) <= 0) {
      pStringMgr->Free(this);
    }
  }
  void Unlock() throw() {
    ATLASSERT(IsLocked());

    if (IsLocked()) {
      nRefs++;  // Locked buffers can't be shared, so no interlocked operation
                // necessary
      if (nRefs == 0) {
        nRefs = 1;
      }
    }
  }
};

}  // namespace ATL