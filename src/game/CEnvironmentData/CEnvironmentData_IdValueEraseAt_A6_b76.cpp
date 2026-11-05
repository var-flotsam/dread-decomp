// Copy of fn_7100f68c7c (src/game/CEnvironmentData/CEnvironmentData_IdValueEraseAt_A6_b74.cpp): reflected
// collection erase slot (0x128) of a base::UnkVector of polymorphic {vtable, string id, 8-byte value} elements
// (base::UnkIdValueElem, include/base/UnkCloneHelpers.h) in CEnvironmentData::SFogTransition's unit: destroy the
// entry an index iterator names through its vtable, then copy each later entry down one and destroy the
// copied-from one. Names and layouts are placeholders.
#include <new>

#include "UnkClone.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkVector.h"
#include "types.h"

extern char g_unk_7101cab298[];

template <char* Vtable, u64 Size>
struct UnkIdValueEraseTag_A6_b76;

template <char* Vtable, u64 Size>
struct base::UnkIdValueTraits<UnkIdValueEraseTag_A6_b76<Vtable, Size>> {
    static char* vtable() { return Vtable; }
};

template <char* Vtable, u64 Size>
using UnkIdValueEraseElem_A6_b76 =
    base::UnkIdValueElem<UnkIdValueEraseTag_A6_b76<Vtable, Size>, UnkCloneElem<UnkIdValueEraseTag_A6_b76<Vtable, Size>, Size>>;

template <char* Vtable, u64 Size>
using UnkIdValueEraseVec_A6_b76 = base::UnkVector<UnkIdValueEraseElem_A6_b76<Vtable, Size>>;

template <char* Vtable, u64 Size>
static inline __attribute__((always_inline)) void IdValueEraseAt_A6_b76(UnkIdValueEraseVec_A6_b76<Vtable, Size>* self,
                                                                        const u32* it) {
    using T = UnkIdValueEraseElem_A6_b76<Vtable, Size>;
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

void fn_7100f640c0(UnkIdValueEraseVec_A6_b76<g_unk_7101cab298, 8>* self, const u32* it) {
    IdValueEraseAt_A6_b76<g_unk_7101cab298, 8>(self, it);
}
