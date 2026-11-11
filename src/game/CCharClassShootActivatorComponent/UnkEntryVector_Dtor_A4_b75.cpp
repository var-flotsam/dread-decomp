// CCharClassShootActivatorComponent unit: the reflected destructor slot (+0x40) of the array of 0x48-byte entries,
// identical to the CBeamBoxComponent unit's fn_71008ab0b8
// (src/game/CCharClassShootActivatorComponent/UnkEntryVector_Clear_A4_b73.cpp, whose layouts are copied here):
// destroys every entry, then frees the buffer. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/UnkTU341.h"
#include "types.h"


struct UnkClearEntry_A4_b75 {
    void* vtable;
    UnkErasedVector list;  // 0x8
    u32 field_0x30;
    base::UnkRefHandle owner;  // 0x38
    base::global::CStrId id;   // 0x40

    __attribute__((always_inline)) void Destroy() {
        id.Reset();
        vtable = reinterpret_cast<char*>(g_unk_7101a33fa0) + 0x10;
        if (base::UnkRefCounted* p = owner.ptr) {
            if (--p->ref_count == 0)
                delete p;
        }
        vtable = reinterpret_cast<char*>(g_unk_7101a20998) + 0x10;
        reinterpret_cast<UnkOwnedErasedVector*>(&list)->~UnkOwnedErasedVector();
    }
};
static_assert(sizeof(UnkClearEntry_A4_b75) == 0x48, "layout");

struct UnkClearVector_A4_b75 {
    UnkClearEntry_A4_b75* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void Clear() {
        for (u64 i = 0; i < count; ++i)
            data[i].Destroy();
        count = 0;
    }

    __attribute__((always_inline)) void Destroy() {
        if (data) {
            Clear();
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

void fn_71008904b0(UnkClearVector_A4_b75* self) {
    self->Destroy();
}
