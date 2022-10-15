// Copyright 2021~2022 `anothel` All rights reserved

<<<<<<< HEAD:include/amvcore.hpp
#ifndef AMVCORE_HPP_
#define AMVCORE_HPP_

#include "include/amvdefine.hpp"

namespace AMV {
=======
namespace
{
>>>>>>> ade53ee0c3a75665892909aca4de84a626eb6dd1:include/amvcore.h

    /////////////////////////////////////////////////////////////////////////////
    // Verify that a null-terminated string points to valid memory
    inline BOOL AmvIsValidString(_In_reads_z_(nMaxLength) const char *psz,
                                 _In_ size_t nMaxLength = UINT_MAX)
    {
        return (psz != NULL);
    }

    template <typename _CharType>
    inline _CharType *AmvCharNext(_In_ const _CharType *p) throw()
    {
        // Too expensive to check separately here
        AMVASSUME(p != NULL);
        return const_cast<_CharType *>(p + 1);
    }

    template <typename CharType>
    inline const CharType *AmvstrchrT(_In_z_ const CharType *p,
                                      _In_ CharType ch) throw()
    {
        AMVASSERT(p != NULL);
        if (p == NULL)
        {
            return NULL;
        }
        while (*p != 0)
        {
            if (*p == ch)
            {
                return p;
            }
            p = AmvCharNext(p);
        }
        // strchr for '\0' should succeed - the while loop terminates
        //*p == 0, but ch also == 0, so NULL terminator address is returned
        return (*p == ch) ? p : NULL;
    }
<<<<<<< HEAD:include/amvcore.hpp
    p = AmvCharNext(p);
  }
  // strchr for '\0' should succeed - the while loop terminates
  // *p == 0, but ch also == 0, so NULL terminator address is returned
  return (*p == ch) ? p : NULL;
}
}  // namespace AMV

#endif  // AMVCORE_HPP_
=======
} // namespace
>>>>>>> ade53ee0c3a75665892909aca4de84a626eb6dd1:include/amvcore.h
