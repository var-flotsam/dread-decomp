// CChangeStateDoorsLogicAction's unit (tools/tu_map.py): collection functions of this unit's reflected array of
// 0x48-byte callback sets (vtable g_unk_7101a33fa0; Reserve fn_7100a3b8e0, the other copies in
// src/game/CCharClassWaterPoolComponent/fn_710090dbb4.cpp): the "set at index" slots (copy and move; copies of
// fn_710090ca8c and fn_710090cc04 in UnkCallbackSetVector_SetAt_A4_b72.cpp, new entries zeroed), and the copy
// assignment of the iterator's cached (index, element) value, which may own its pair or refer to the iterator's
// (state 2). Names and layouts are placeholders.
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "game/CChangeStateDoorsLogicAction.h"
#include "types.h"

extern void* g_unk_7101a20998[];  // the callback lists' base vtable
extern void* g_unk_7101a33fa0[];  // their vtable

struct UnkDoorSetElem_ovb0087 {
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
    u32 field_0x40;

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
    }
    __attribute__((always_inline)) void Assign(const UnkDoorSetElem_ovb0087& other) {
        const base::UnkRefHandle* src = &other.owner;
        fn_710031d208(&owner, src);
        field_0x30 = other.field_0x30;
        fn_710023b29c(List(), other.List());
        field_0x40 = other.field_0x40;
    }
};
static_assert(sizeof(UnkDoorSetElem_ovb0087) == 0x48, "layout");

struct UnkCallbackSetVector;

struct UnkDoorSetVector_ovb0087 {
    UnkDoorSetElem_ovb0087* data;
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
            fn_7100a3b8e0(reinterpret_cast<UnkCallbackSetVector*>(this), n);
            fn_7100a3b8e0(reinterpret_cast<UnkCallbackSetVector*>(this), index + 1);
            for (u32 i = count; i < index + 1; ++i)
            {
                UnkDoorSetElem_ovb0087* e = &data[i];
                __builtin_memset(e, 0, sizeof(UnkDoorSetElem_ovb0087));
                e->Construct();
            }
            count = index + 1;
        }
    }
};

void fn_7100a3b374(UnkDoorSetVector_ovb0087* self, const u32* index, const UnkDoorSetElem_ovb0087* value) {
    u32 i = *index;
    self->GrowTo(i);
    const base::UnkRefHandle* src = &value->owner;
    UnkDoorSetElem_ovb0087* e = &self->data[i];
    fn_710031d208(&e->owner, src);
    e->field_0x30 = value->field_0x30;
    fn_710023b29c(e->List(), value->List());
    e->field_0x40 = value->field_0x40;
}

// The move slot: the given entry's list is emptied (fn_7100ad3588) and its owner released after the assignment.
void fn_7100a3b4e0(UnkDoorSetVector_ovb0087* self, const u32* index, UnkDoorSetElem_ovb0087* value) {
    u32 i = *index;
    self->GrowTo(i);
    UnkDoorSetElem_ovb0087* data = self->data;
    const base::UnkRefHandle* src = &value->owner;
    UnkDoorSetElem_ovb0087* e = &data[i];
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

// The cached (index, element) value: owned (state 1) or referring to the iterator's (state 2).
struct UnkDoorIterValue_ovb0087 {
    struct Value {
        u32 index;
        UnkDoorSetElem_ovb0087 element;
    };
    struct Ref {
        u32* index;
        UnkDoorSetElem_ovb0087* element;
    };
    union {
        Value value;
        Ref ref;
    };
    u32 state;

    u32* Index() { return state == 2 ? ref.index : &value.index; }
    const u32* Index() const { return state == 2 ? ref.index : &value.index; }
    UnkDoorSetElem_ovb0087* Element() { return state == 2 ? ref.element : &value.element; }
    const UnkDoorSetElem_ovb0087* Element() const { return state == 2 ? ref.element : &value.element; }
};

UnkDoorIterValue_ovb0087* fn_7100a3acec(UnkDoorIterValue_ovb0087* self, const UnkDoorIterValue_ovb0087* other) {
    if (self != other) {
        *self->Index() = *other->Index();
        const UnkDoorSetElem_ovb0087* src = other->Element();
        self->Element()->Assign(*src);
    }
    return self;
}
