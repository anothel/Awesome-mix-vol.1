#include "include/cstringt.h"

namespace awesome_ATL {

class CC : public IAtlStringMgr {
  virtual CStringData* Allocate(int nAllocLength, int nCharSize) throw() {

  }

  // Free an existing CStringData
  virtual void Free(CStringData* pData) throw() {

  }

  // Change the size of an existing CStringData
  virtual CStringData* Reallocate(CStringData* pData, int nAllocLength,
                                  int nCharSize) throw() {

                                  }

  // Get the CStringData for a Nil string
  virtual CStringData* GetNilString() throw() {}

  virtual IAtlStringMgr* Clone() throw() {}
};
}  // namespace awesome_ATL

static awesome_ATL::CC a; 

awesome_ATL::IAtlStringMgr* AfxGetStringManager() {
  return (awesome_ATL::IAtlStringMgr*)&a;
}

template< typename _CharType = char, class StringIterator = awesome_ATL::ChTraitsCRT< _CharType > >
class StrTrait : 
	public StringIterator
{
public:
	static awesome_ATL::IAtlStringMgr* GetDefaultManager() throw()
	{
		return( AfxGetStringManager() );
	}
};

typedef awesome_ATL::CStringT< char, StrTrait< char > > CString;
