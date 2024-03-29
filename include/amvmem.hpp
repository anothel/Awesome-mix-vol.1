// Copyright 2021~2022 `anothel` All rights reserved

#ifndef AMVMEM_HPP_
#define AMVMEM_HPP_

#include <malloc.h>

#include <cstdlib>

#include "include/amvdefine.hpp"
#include "salieri-src/salieri.h"

namespace AMV
{

    template <typename N>
    inline N AmvAlignUp(_In_ N n, _In_ ULONG nAlign) throw()
    {
        return (N((n + (nAlign - 1)) & ~(N(nAlign) - 1)));
    }

<<<<<<< HEAD:include/amvmem.hpp
#endif  // AMVMEM_HPP_
=======
    __interface __declspec(uuid("654F7EF5-CFDF-4df9-A450-6C6A13C622C0"))
        IAmvMemMgr
    {
    public:
        virtual void *Allocate(_In_ size_t nBytes) throw() = 0;

        virtual void Free(_Inout_opt_ void *p) throw() = 0;

        virtual void *Reallocate(_Inout_updates_bytes_opt_(nBytes) void *p,
                                 _In_ size_t nBytes) throw() = 0;

        virtual size_t GetSize(_In_ void *p) throw() = 0;
    };

    class CAmvHeap : public IAmvMemMgr
    {
    public:
        CAmvHeap() throw() {}

        // IAmvMemMgr
        virtual void *Allocate(_In_ size_t nBytes) throw()
        {
            return (malloc(nBytes));
        }

        virtual void Free(_In_opt_ void *p) throw()
        {
            if (p != NULL)
            {
                free(p);
            }
        }

        virtual void *Reallocate(_In_opt_ void *p, _In_ size_t nBytes) throw()
        {
            if (p == NULL)
            {
                return (Allocate(nBytes));
            }

            if (nBytes == 0)
            {
                Free(p);
                return NULL;
            }

            return (realloc(p, nBytes));
        }

        virtual size_t GetSize(_Inout_ void *p) throw()
        {
            return (malloc_usable_size(p));
        }
    };
} // namespace AMV

#endif // AMVMEM_H_
>>>>>>> ade53ee0c3a75665892909aca4de84a626eb6dd1:include/amvmem.h
