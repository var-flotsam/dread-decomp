#include <new>

#include "UnkClone.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkVector.h"
#include "types.h"

extern char g_unk_7101cab220[];
extern char g_unk_7101cab338[];

template <char* Vtable, u64 Size>
struct UnkIdValueEraseTag_b0196;

template <char* Vtable, u64 Size>
struct base::UnkIdValueTraits<UnkIdValueEraseTag_b0196<Vtable, Size>> {
    static char* vtable() { return Vtable; }
};

template <char* Vtable, u64 Size>
using UnkIdValueEraseElem_b0196 =
    base::UnkIdValueElem<UnkIdValueEraseTag_b0196<Vtable, Size>, UnkCloneElem<UnkIdValueEraseTag_b0196<Vtable, Size>, Size>>;

template <char* Vtable, u64 Size>
using UnkIdValueEraseVec_b0196 = base::UnkVector<UnkIdValueEraseElem_b0196<Vtable, Size>>;

template <char* Vtable, u64 Size>
static inline __attribute__((always_inline)) void IdValueEraseAt_b0196(UnkIdValueEraseVec_b0196<Vtable, Size>* self,
                                                                       const u32* it) {
    using T = UnkIdValueEraseElem_b0196<Vtable, Size>;
    T* base = reinterpret_cast<T*>(self);
    u64 offset = (u64)*it * sizeof(T);
    T* p = reinterpret_cast<T*>(reinterpret_cast<char*>(base) + offset);
    u32 index = (u32)(p - base);
    if (index >= self->count)
        return;
    T* d = self->data;
    u64 lo = (u32)(p - base);
    d[lo].Destroy();
    for (u32 i = index; i < self->count - 1; ++i) {
        T* src = &self->data[i] + 1;
        new (src - 1) T(*src);
        (&self->data[i] + 1)->Destroy();
    }
    self->count = self->count - 1;
}

#define IDVALUE_ERASE_B0196(fn, vt, size)                                     \
    void fn(UnkIdValueEraseVec_b0196<vt, size>* self, const u32* it) {        \
        IdValueEraseAt_b0196<vt, size>(self, it);                             \
    }

IDVALUE_ERASE_B0196(fn_7100f5d270, g_unk_7101cab220, 16)
