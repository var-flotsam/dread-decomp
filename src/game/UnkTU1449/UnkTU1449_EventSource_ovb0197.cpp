// Unit 1449 of tools/tu_map.py (no name hint): the destructor and deleting destructor of an event source
// (vtable g_unk_7101a85bd8: a weak-link listener at +0x258, three callback-slot groups at +0xe0, +0x150 and
// +0x1c0, a record at +0x30, then the base with the reflected value at +0x18; the same shape as
// src/game/UnkUnit1452/UnkEventHolder_Dtor_A4_b74.cpp), and the constructor of a string-id vector from an
// array of ids. Names and layouts are placeholders.
#include "base/UnkCloneRecords.h"
#include "base/UnkVector.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/UnkTU1449.h"
#include "game_s2/UnkWeakHandle.h"

#include "base/global/CStrId.h"
#include "types.h"

struct UnkLink_b0197 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;           // 0x8
    void* owner;             // 0x10
};

struct UnkListener_b0197 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkLink_b0197* link;  // 0x18
};

namespace {

struct UnkEventBase_b0197 {
    void* vtable;
    u8 pad_0x8[0x10];
    base::reflection::UnkReflValue field_0x18;

    void DestroyBase(void** vt) {
        vtable = vt + 2;
        fn_71000a1c60(&field_0x18);
    }
};

struct UnkSlots_b0197 : UnkEventBase_b0197 {
    void* slots[8];

    void Destroy(void** vt) {
        slots[7] = nullptr;
        slots[6] = nullptr;
        slots[5] = nullptr;
        slots[4] = nullptr;
        slots[3] = nullptr;
        slots[0] = nullptr;
        slots[1] = nullptr;
        slots[2] = nullptr;
        DestroyBase(vt);
    }
};

struct UnkRecord_b0197 {
    void* vtable;
    u8 pad_0x8[0x8];
    void* field_0x10;
    void* field_0x18;
    u32 field_0x20;

    void Destroy(void** vt) {
        vtable = vt + 2;
        field_0x10 = nullptr;
        field_0x18 = nullptr;
        field_0x20 = 0;
    }
};

}  // namespace

struct UnkEventHolder_b0197 : UnkEventBase_b0197 {
    UnkRecord_b0197 field_0x30;
    u8 pad_0x58[0x88];
    UnkSlots_b0197 field_0xe0;
    UnkSlots_b0197 field_0x150;
    UnkSlots_b0197 field_0x1c0;
    u8 pad_0x230[0x28];
    UnkListener_b0197 listener_0x258;
};
static_assert(__builtin_offsetof(UnkEventHolder_b0197, listener_0x258) == 0x258, "layout");

static inline __attribute__((always_inline)) void Destroy_b0197(UnkEventHolder_b0197* self) {
    self->vtable = g_unk_7101a85bd8 + 2;
    UnkListener_b0197* listener = &self->listener_0x258;
    listener->vtable = g_unk_7101a22d48 + 2;
    UnkLink_b0197* link = listener->link;
    if (link) {
        if (void* owner = link->owner) {
            fn_710117b3f0((UnkWeakTarget*)owner, listener);
            link = listener->link;
        }
        if (link && --link->ref_count == 0)
            link->Release();
    }
    self->field_0x1c0.Destroy(g_unk_7101a85d58);
    self->field_0x150.Destroy(g_unk_7101a85d58);
    self->field_0xe0.Destroy(g_unk_7101a85d58);
    self->field_0x30.Destroy(g_unk_7101a85db0);
    self->DestroyBase(g_unk_7101a85c80);
}

void fn_710097e4d8(UnkEventHolder_b0197* self) {
    Destroy_b0197(self);
}

void fn_710097e5ec(UnkEventHolder_b0197* self) {
    Destroy_b0197(self);
    fn_7100083c00(self);
}


// The string-id vector built from an array of ids (an initializer list: pointer and size), a
// growth step, an allocator (the default one when none is given) and the allocation flags.
void fn_710097bb7c(base::UnkStrIdVec* self, const base::global::CStrId* ids, u64 size, u32 step,
                   base::UnkAllocator* allocator, u32 flags) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = step;
    self->field_0x14 = step != 0;
    self->flags = flags;
    self->allocator = allocator ? allocator : fn_7100083990();
    self->Reserve(size);
    for (const base::global::CStrId *p = ids, *end = ids + size; p != end; ++p) {
        base::global::CStrId id = *p;
        self->PushBack(id);
    }
}
