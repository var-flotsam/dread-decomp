// CCharClassShotComponent's unit (tools/tu_map.py unit 1507): the CShotComponent constructor, the
// counterpart of src/game/CCharClassShotComponent/CShotComponent_Dtor_b52.cpp. After the char class
// base constructor (fn_71011acd10) it sets the vtable and builds the list of owned objects at +0x58
// (game_s2/UnkOwnedList.h: allocator flags 0xf) with room for ten. Names and layouts are placeholders.
#include "game/CCharClassComponent.h"
#include "game_s2/UnkOwnedList.h"
#include "types.h"

struct UnkStruct_71008bca30;
extern void* g_unk_7101a8ab40[];

struct UnkCShotComponent_csc {
    void** vtable;
    u8 pad_0x8[0x50];
    UnkOwnedList list;  // 0x58
};

// Moves the owned objects into a new array of `capacity` (each moved-from slot is emptied and
// destroyed), freeing the old one.
static inline void Reserve_csc(UnkOwnedList* list, u32 capacity) {
    if (list->capacity >= capacity)
        return;
    UnkOwnedPtr* p = static_cast<UnkOwnedPtr*>(list->allocator->vf_0x40(capacity * sizeof(UnkOwnedPtr), 8, list->flags));
    if (list->data) {
        UnkOwnedPtr* src = list->data;
        UnkOwnedPtr* dst = p;
        for (u32 i = 0; i < list->count; ++i, ++dst, ++src) {
            UnkOwned* moved = src->ptr;
            src->ptr = nullptr;
            dst->ptr = moved;
            src->~UnkOwnedPtr();
        }
        list->allocator->vf_0x48(list->data);
    }
    list->data = p;
    list->capacity = capacity;
}

void fn_71009f8ac4(UnkCShotComponent_csc* self) {
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->vtable = g_unk_7101a8ab40 + 2;
    new (&self->list) UnkOwnedList();
    Reserve_csc(&self->list, 10);
}
