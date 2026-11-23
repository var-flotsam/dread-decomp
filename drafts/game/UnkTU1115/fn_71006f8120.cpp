// Unit 1115 (tools/tu_map.py, no class name): an object constructor of the registry that hands copies of
// the names and ids, and the default word, to the full constructor (fn_71006f84a8). Names and layouts are
// placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

namespace base {
struct UnkStrIdVec;
}
struct UnkRegObj_b0255;
struct UnkRegistry_b0255;
void fn_71006f84a8(UnkRegObj_b0255* self, UnkRegistry_b0255* registry, const CStrId* name, u32 id,
                   const base::UnkStrIdVec* names, const CRntVector<u32>* ids, u32 field_0xc);

extern u32 g_unk_7101a6b7f0;  // the default for the object's word at +0xc

// A name in the object's array: a string id that moves by handing over its string.
struct UnkNameId_l6f8120 {
    UnkStringData* data;

    UnkNameId_l6f8120(const UnkNameId_l6f8120& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    UnkNameId_l6f8120(UnkNameId_l6f8120&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }
    ~UnkNameId_l6f8120() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

// A copy of an array handed to the constructor, freed when it goes out of scope.
template <class T>
struct UnkVecCopy_l6f8120 : CRntVector<T> {
    explicit UnkVecCopy_l6f8120(const CRntVector<T>& other) : CRntVector<T>(other) {}
    ~UnkVecCopy_l6f8120() { this->Free(); }
};

void fn_71006f8120(UnkRegObj_b0255* self, UnkRegistry_b0255* registry, const CStrId* name, u32 id,
                   const CRntVector<UnkNameId_l6f8120>* names, const CRntVector<u32>* ids) {
    UnkVecCopy_l6f8120<UnkNameId_l6f8120> names_copy(*names);
    UnkVecCopy_l6f8120<u32> ids_copy(*ids);
    fn_71006f84a8(self, registry, name, id, reinterpret_cast<const base::UnkStrIdVec*>(&names_copy), &ids_copy,
                  g_unk_7101a6b7f0);
}
