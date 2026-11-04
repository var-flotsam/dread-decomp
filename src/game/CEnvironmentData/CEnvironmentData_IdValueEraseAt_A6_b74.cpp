// Reflected collection erase slots (0x128) of CEnvironmentData::SFogTransition's unit, for arrays
// (base::UnkVector) of polymorphic {vtable, string id, value} elements (base::UnkIdValueElem,
// include/base/UnkCloneHelpers.h): destroy the entry an index iterator names through its vtable,
// then copy each later entry down one and destroy the copied-from one. b73's erase template
// (src/base/reflection/UnkCollectionEraseAt_A1_b73.cpp). One copy per element class; the value is
// 4 bytes except in 0x7101cab2c0's and 0x7101cab2e8's classes (8). Names and layouts are placeholders.
#include <new>

#include "UnkClone.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkVector.h"
#include "types.h"

extern char g_unk_7101cab248[];
extern char g_unk_7101cab270[];
extern char g_unk_7101cab2c0[];
extern char g_unk_7101cab2e8[];
extern char g_unk_7101cab310[];
extern char g_unk_7101cab360[];
extern char g_unk_7101cab388[];
extern char g_unk_7101cab3b0[];
extern char g_unk_7101cab3d8[];

template <char* Vtable, u64 Size>
struct UnkIdValueEraseTag_A6_b74;

template <char* Vtable, u64 Size>
struct base::UnkIdValueTraits<UnkIdValueEraseTag_A6_b74<Vtable, Size>> {
    static char* vtable() { return Vtable; }
};

template <char* Vtable, u64 Size>
using UnkIdValueEraseElem_A6_b74 =
    base::UnkIdValueElem<UnkIdValueEraseTag_A6_b74<Vtable, Size>, UnkCloneElem<UnkIdValueEraseTag_A6_b74<Vtable, Size>, Size>>;

template <char* Vtable, u64 Size>
using UnkIdValueEraseVec_A6_b74 = base::UnkVector<UnkIdValueEraseElem_A6_b74<Vtable, Size>>;

template <char* Vtable, u64 Size>
static inline __attribute__((always_inline)) void IdValueEraseAt_A6_b74(UnkIdValueEraseVec_A6_b74<Vtable, Size>* self,
                                                                        const u32* it) {
    using T = UnkIdValueEraseElem_A6_b74<Vtable, Size>;
    T* base = reinterpret_cast<T*>(self);
    u64 offset = (u64)*it * sizeof(T);
    T* p = reinterpret_cast<T*>(reinterpret_cast<char*>(base) + offset);
    u32 index = (u32)(p - base);
    if (index >= self->count)
        return;
    self->data[index].Destroy();
    for (u32 i = index; i < self->count - 1; ++i) {
        T* src = &self->data[i] + 1;
        new (src - 1) T(*src);
        (&self->data[i] + 1)->Destroy();
    }
    self->count = self->count - 1;
}

#define IDVALUE_ERASE_A6_B74(fn, vt, size)                                    \
    void fn(UnkIdValueEraseVec_A6_b74<vt, size>* self, const u32* it) {       \
        IdValueEraseAt_A6_b74<vt, size>(self, it);                            \
    }

IDVALUE_ERASE_A6_B74(fn_7100f5f5ac, g_unk_7101cab248, 4)
IDVALUE_ERASE_A6_B74(fn_7100f61c88, g_unk_7101cab270, 4)
IDVALUE_ERASE_A6_B74(fn_7100f667fc, g_unk_7101cab2c0, 8)
IDVALUE_ERASE_A6_B74(fn_7100f68c7c, g_unk_7101cab2e8, 8)
IDVALUE_ERASE_A6_B74(fn_7100f6ad50, g_unk_7101cab310, 4)
IDVALUE_ERASE_A6_B74(fn_7100f7063c, g_unk_7101cab360, 4)
IDVALUE_ERASE_A6_B74(fn_7100f727a8, g_unk_7101cab388, 4)
IDVALUE_ERASE_A6_B74(fn_7100f74f68, g_unk_7101cab3b0, 4)
IDVALUE_ERASE_A6_B74(fn_7100f77188, g_unk_7101cab3d8, 4)
