#include "include/amvdefine.h"

/////////////////////////////////////////////////////////////////////////////
// Verify that a null-terminated string points to valid memory
inline BOOL AmvIsValidString(_In_reads_z_(nMaxLength) LPCSTR psz,
                             _In_ size_t nMaxLength = UINT_MAX) {
  return (psz != NULL);
}
