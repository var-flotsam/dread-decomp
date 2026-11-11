// CCharClassShootActivatorComponent's unit (fn_7100894c50) and CBeamBoxComponent's (fn_71008aaab4, identical):
// reflected collection erase (slot 0x128) for a base::UnkVector of 0x48-byte callback entries (a callback list
// with the vtables g_unk_7101a33fa0 over g_unk_7101a20998, then a string id). The entry an index iterator names
// is destroyed inline, each later entry is moved down one (the list through fn_71007b1928, the id inline) and
// the moved-from one destroyed. The index goes through a byte offset from the vector itself
// (build/b67/b67a6.md). Names are placeholders.
#include "base/UnkCloneRecords.h"
#include "base/UnkVector.h"
#include "game/CDemolitionBlockComponent.h"
#include "types.h"

extern void* g_unk_7101a20998[];
extern void* g_unk_7101a33fa0[];

// The entry's destructor chain, written out with always_inline so the erase inlines it as the original does
// (the out-of-line copy is base::UnkCallbackEntry's, fn_71008933d4).
struct UnkCallbackListBase_A5_b69 {
    char* vtable;
    base::UnkVector<base::UnkCallable> callables;
    __attribute__((always_inline)) ~UnkCallbackListBase_A5_b69() {
        vtable = reinterpret_cast<char*>(g_unk_7101a20998) + 0x10;
        if (callables.data) {
            for (u64 i = 0; i < callables.count; ++i)
                callables.data[i].~UnkCallable();
            callables.count = 0;
            callables.allocator->vf_0x48(callables.data);
            callables.capacity = 0;
        }
    }
};
struct UnkCallbackList_A5_b69 : UnkCallbackListBase_A5_b69 {
    u64 field_0x30;
    base::UnkRefPtrMember owner;
    __attribute__((always_inline)) ~UnkCallbackList_A5_b69() {
        this->vtable = reinterpret_cast<char*>(g_unk_7101a33fa0) + 0x10;
    }
};
struct UnkCallbackEntry_A5_b69 {
    UnkCallbackList_A5_b69 list;
    base::UnkStrIdMember id;
    __attribute__((always_inline)) ~UnkCallbackEntry_A5_b69() {}
};
static_assert(sizeof(UnkCallbackEntry_A5_b69) == 0x48, "UnkCallbackEntry_A5_b69 layout");

static inline __attribute__((always_inline)) void MoveEntry_A5_b69(UnkCallbackEntry_A5_b69* dst,
                                                                   UnkCallbackEntry_A5_b69* src) {
    fn_71007b1928(dst, src);
    dst->id.data = src->id.data;
    src->id.data = &g_unk_7101d47108->empty;
}

static inline __attribute__((always_inline)) void EraseAt_A5_b69(base::UnkVector<UnkCallbackEntry_A5_b69>* self,
                                                                 u32 index) {
    if (index >= self->count)
        return;
    self->data[index].~UnkCallbackEntry_A5_b69();
    for (u64 i = index; i < self->count - 1; ++i) {
        UnkCallbackEntry_A5_b69* src = &self->data[i] + 1;
        MoveEntry_A5_b69(src - 1, src);
        (&self->data[i] + 1)->~UnkCallbackEntry_A5_b69();
    }
    self->count = self->count - 1;
}

static inline __attribute__((always_inline)) void EraseAtIter_A5_b69(base::UnkVector<UnkCallbackEntry_A5_b69>* self,
                                                                     const u32* it) {
    UnkCallbackEntry_A5_b69* base = reinterpret_cast<UnkCallbackEntry_A5_b69*>(self);
    u64 offset = (u64)*it * sizeof(UnkCallbackEntry_A5_b69);
    UnkCallbackEntry_A5_b69* p =
        reinterpret_cast<UnkCallbackEntry_A5_b69*>(reinterpret_cast<char*>(base) + offset);
    EraseAt_A5_b69(self, (u32)(p - base));
}

void fn_7100894c50(base::UnkVector<UnkCallbackEntry_A5_b69>* self, const u32* it) {
    EraseAtIter_A5_b69(self, it);
}

void fn_71008aaab4(base::UnkVector<UnkCallbackEntry_A5_b69>* self, const u32* it) {
    EraseAtIter_A5_b69(self, it);
}
