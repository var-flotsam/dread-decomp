// A component of the CDemolitionBlockComponent unit (the one UnkDemolitionBlockCtor.cpp constructs):
// its destructor. The two arrays at +0x108 and +0xe0, the listener at +0xc0, the string-id table at
// +0x90 and the name at +0x88, then the activatable component's destructor (fn_71007b00cc). Names
// and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkStrIdTable.h"
#include "base/global/CRntVector.h"
#include "game/CAbilityGhostAura.h"
#include "game/CDemolitionBlockComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// An array element (0x40 bytes) destroyed through its vtable.
struct UnkDemolitionBlockEntry_l7afecc {
    virtual ~UnkDemolitionBlockEntry_l7afecc();
    u8 pad_0x8[0x38];
};

// The counted link a listener holds; the object it is registered with is at +0x10.
struct UnkDemolitionBlockLink_l7afecc {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;
    void* owner;
};

// A listener that unregisters from its link's owner (fn_710117b3f0) on destruction.
struct UnkDemolitionBlockListener_l7afecc {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkDemolitionBlockLink_l7afecc* link;

    ~UnkDemolitionBlockListener_l7afecc() {
        vtable = &g_unk_7101a22d48[2];
        if (UnkDemolitionBlockLink_l7afecc* l = link) {
            bool held = true;
            if (void* owner = l->owner) {
                fn_710117b3f0(static_cast<UnkWeakTarget*>(owner), this);
                l = link;
                held = l != nullptr;
            }
            if (held && --l->ref_count == 0)
                l->Release();
        }
    }
};

struct UnkDemolitionBlockTable_l7afecc : UnkStrIdTable {
    ~UnkDemolitionBlockTable_l7afecc() {
        fn_71004f11f8(this);
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }
};

struct UnkDemolitionBlock_l7afecc {
    void** vtable;
    u8 pad_0x8[0x80];
    base::UnkStrIdMember name;                                            // 0x88
    UnkDemolitionBlockTable_l7afecc table;                                // 0x90
    u8 pad_0xb8[0x8];
    UnkDemolitionBlockListener_l7afecc listener;                          // 0xc0
    base::global::CRntVector<UnkDemolitionBlockEntry_l7afecc> entries_a;  // 0xe0
    base::global::CRntVector<UnkDemolitionBlockEntry_l7afecc> entries_b;  // 0x108
};
static_assert(__builtin_offsetof(UnkDemolitionBlock_l7afecc, table) == 0x90, "layout");
static_assert(__builtin_offsetof(UnkDemolitionBlock_l7afecc, listener) == 0xc0, "layout");
static_assert(__builtin_offsetof(UnkDemolitionBlock_l7afecc, entries_b) == 0x108, "layout");

void fn_71007afecc(UnkDemolitionBlock_l7afecc* self) {
    self->vtable = g_unk_7101a70380 + 2;
    self->entries_b.Free();
    self->entries_a.Free();
    self->listener.~UnkDemolitionBlockListener_l7afecc();
    self->table.~UnkDemolitionBlockTable_l7afecc();
    using base::UnkStrIdMember;
    self->name.~UnkStrIdMember();
    fn_71007b00cc(reinterpret_cast<UnkActivatable_b44a2<0x48>*>(self));
}
