// CCharClassWaterPoolComponent unit: the reflected "set at index" slots (+0x140 copy, +0x148 move) of the array of
// 0x48-byte callback sets that src/game/CCharClassWaterPoolComponent/fn_710090dbb4.cpp handles. A growable array
// is first grown to hold the index (Reserve to the next step multiple, then to index + 1) and the new entries
// default-constructed (an empty callback list, scale -1.0f); then the entry at the index is assigned from the given
// one. Names and layouts are placeholders.
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CCharClassWaterPoolComponent.h"
#include "game/UnkTU341.h"
#include "types.h"

struct UnkSetElem_A4_b72 {
    void* vtable;
    UnkErasedValue_0x10* list_data;  // 0x8, an UnkErasedVector written as flat members
    u32 list_count;
    u32 list_capacity;
    u32 list_step;
    bool list_grow;
    u32 list_flags;
    base::UnkAllocator* list_allocator;
    u32 field_0x30;
    base::UnkRefHandle owner;  // 0x38
    f32 field_0x40;

    UnkErasedVector* List() { return reinterpret_cast<UnkErasedVector*>(&list_data); }
    const UnkErasedVector* List() const { return reinterpret_cast<const UnkErasedVector*>(&list_data); }

    __attribute__((always_inline)) void Construct() {
        vtable = reinterpret_cast<char*>(g_unk_7101a20998) + 0x10;
        list_data = nullptr;
        list_count = 0;
        list_capacity = 0;
        list_step = 10;
        list_grow = true;
        list_flags = 0xf;
        list_allocator = fn_7100083990();
        field_0x30 = 0;
        vtable = reinterpret_cast<char*>(g_unk_7101a33fa0) + 0x10;
        owner.ptr = nullptr;
        field_0x40 = -1.0f;
    }
};
static_assert(sizeof(UnkSetElem_A4_b72) == 0x48, "layout");

struct UnkCallbackSetVector;

struct UnkSetVector_A4_b72 {
    UnkSetElem_A4_b72* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void GrowTo(u32 index) {
        if (growable && count <= index) {
            u32 n = capacity;
            if (n <= index && step != 0)
                n = (index / step + 1) * step;
            fn_710090d00c(reinterpret_cast<UnkCallbackSetVector*>(this), n);
            fn_710090d00c(reinterpret_cast<UnkCallbackSetVector*>(this), index + 1);
            for (u32 i = count; i < index + 1; ++i)
            {
                UnkSetElem_A4_b72* e = &data[i];
                __builtin_memset(e, 0, sizeof(UnkSetElem_A4_b72));
                e->Construct();
            }
            count = index + 1;
        }
    }
};

void fn_710090ca8c(UnkSetVector_A4_b72* self, const u32* index, const UnkSetElem_A4_b72* value) {
    u32 i = *index;
    self->GrowTo(i);
    const base::UnkRefHandle* src = &value->owner;
    UnkSetElem_A4_b72* e = &self->data[i];
    fn_710031d208(&e->owner, src);
    e->field_0x30 = value->field_0x30;
    fn_710023b29c(e->List(), value->List());
    e->field_0x40 = value->field_0x40;
}

// The move slot: the given entry's list is emptied (fn_7100ad3588) and its owner released after the assignment.
void fn_710090cc04(UnkSetVector_A4_b72* self, const u32* index, UnkSetElem_A4_b72* value) {
    u32 i = *index;
    self->GrowTo(i);
    UnkSetElem_A4_b72* data = self->data;
    const base::UnkRefHandle* src = &value->owner;
    UnkSetElem_A4_b72* e = &data[i];
    fn_710031d208(&e->owner, src);
    UnkErasedVector* list = value->List();
    e->field_0x30 = value->field_0x30;
    fn_710023b29c(e->List(), list);
    value->field_0x30 = 0;
    fn_7100ad3588(list);
    if (base::UnkRefCounted* p = value->owner.ptr) {
        if (--p->ref_count == 0)
            delete p;
        value->owner.ptr = nullptr;
    }
    data[i].field_0x40 = value->field_0x40;
}
