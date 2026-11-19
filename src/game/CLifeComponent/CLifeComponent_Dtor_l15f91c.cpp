// CLifeComponent's unit (tools/tu_map.py unit 2715): the life component's base destructor (fn_710115f91c):
// destroys and frees the entries at +0x218, releases the two sets of lists (last first), destroys the two
// strings, drops the reference at +0x48, then runs the component base destructor. Names and layouts are
// placeholders.
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "game/CLifeComponent.h"
#include "game/UnkComponent.h"
#include "types.h"

using base::global::CRntString;
using base::global::CRntVector;

// A polymorphic 0x100-byte entry, destroyed through its vtable.
struct UnkLifeEntry_l15f91c {
    virtual ~UnkLifeEntry_l15f91c();
    u8 pad_0x8[0xf8];
};
static_assert(sizeof(UnkLifeEntry_l15f91c) == 0x100, "UnkLifeEntry_l15f91c layout");

// A reference-counted object: released through its third virtual slot when the count reaches zero.
struct UnkLifeShared_l15f91c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Destroy();
    u32 refcount;  // 0x8
};

// Three lists; the first holds the addresses of slots that refer back to this set.
struct UnkLifeListSet_l15f91c {
    CRntVector<void**> slots;
    CRntVector<void*> field_0x28;
    CRntVector<void*> field_0x50;
    u8 pad_0x78[0x8];

    void Release() {
        void*** end = slots.data + slots.count;
        for (void*** it = slots.data; it != end; ++it)
            **it = nullptr;
        slots.Clear();
        field_0x28.Clear();
        field_0x50.Clear();
        field_0x50.Free();
        field_0x28.Free();
        slots.Free();
    }
};
static_assert(sizeof(UnkLifeListSet_l15f91c) == 0x80, "UnkLifeListSet_l15f91c layout");

struct UnkLifeComponent_l15f91c {
    void** vtable;
    u8 pad_0x8[0x40];
    UnkLifeShared_l15f91c* shared;  // 0x48
    u8 pad_0x50[0x30];
    CRntString root_name;  // 0x80
    CRntString field_0xb0;
    u8 pad_0xe0[0x38];
    UnkLifeListSet_l15f91c sets[2];  // 0x118
    CRntVector<UnkLifeEntry_l15f91c> entries;  // 0x218
};
static_assert(__builtin_offsetof(UnkLifeComponent_l15f91c, root_name) == 0x80, "layout");
static_assert(__builtin_offsetof(UnkLifeComponent_l15f91c, sets) == 0x118, "layout");
static_assert(__builtin_offsetof(UnkLifeComponent_l15f91c, entries) == 0x218, "layout");

void fn_710115f91c(void* object) {
    UnkLifeComponent_l15f91c* self = static_cast<UnkLifeComponent_l15f91c*>(object);
    self->vtable = &g_unk_7101cb3db0[2];
    self->entries.Free();
    self->sets[1].Release();
    self->sets[0].Release();
    fn_710007ecd4(&self->field_0xb0);
    fn_710007ecd4(&self->root_name);
    if (UnkLifeShared_l15f91c* shared = self->shared) {
        if (--shared->refcount == 0)
            shared->Destroy();
    }
    fn_71010c4194(static_cast<UnkBaseComponent*>(object));
}
