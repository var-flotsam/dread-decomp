// CAbilityGhostAura's unit (tools/tu_map.py unit 430): the ghost aura's destructor. Drops its counted
// reference, frees its list, unlinks its callback and its listener, then CAbility's members and the base
// destructor fn_7101043934. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAbility.h"
#include "game/CAbilityGhostAura.h"
#include "game/CGameObject.h"
#include "game/navmesh/SNavMeshPath.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkCameraCtrl_ovb0049;

struct UnkGhostAuraClearedPair_l314668 {
    u64 a, b;

    void Destroy() {
        a = 0;
        b = 0;
    }
};

// A value holder: a reflected value and four pairs cleared on destruction.
struct UnkGhostAuraValueHolder_l314668 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    base::reflection::UnkReflValue value;
    UnkGhostAuraClearedPair_l314668 pairs[4];

    void Destroy() {
        for (int i = 3; i >= 0; --i)
            pairs[i].Destroy();
        vtable = &g_unk_7101a225f8[2];
        fn_71000a1c60(&value);
    }
};
static_assert(sizeof(UnkGhostAuraValueHolder_l314668) == 0x70, "holder size");

struct UnkGhostAuraMember_l314668 {
    void** vtable;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;

    void Destroy() {
        vtable = &g_unk_7101a22650[2];
        field_0x20 = 0;
        field_0x10 = 0;
        field_0x18 = 0;
    }
};

// The listener's counted link to its target.
struct UnkGhostAuraLink_l314668 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;  // 0x8
    void* owner;  // 0x10
};

struct UnkGhostAuraListener_l314668 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkGhostAuraLink_l314668* link;  // 0x18

    void Destroy() {
        vtable = &g_unk_7101a22d48[2];
        UnkGhostAuraLink_l314668* l = link;
        if (!l)
            return;
        if (void* owner = l->owner) {
            fn_710117b3f0(static_cast<UnkWeakTarget*>(owner), this);
            l = link;
            if (!l)
                return;
        }
        if (--l->ref_count == 0)
            l->Release();
    }
};

// A callback: the list it is linked into, its object and its function.
struct UnkGhostAuraCallback_l314668 {
    UnkLinkList* list;
    void* object;
    void (*function)(void*);

    void Destroy() {
        if (list) {
            fn_71002aeb9c(list, &list);
            list = nullptr;
        }
        function = nullptr;
        object = nullptr;
    }
};

// base::global::CRntVector's layout, of plain elements.
struct UnkGhostAuraList_l314668 {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    base::UnkAllocator* allocator;

    void Destroy() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

struct UnkAbilityGhostAura_l314668 {
    void** vtable;
    u64 field_0x8;
    UnkGhostAuraMember_l314668 member;  // 0x10
    u8 pad_0x38[0xe8 - 0x38];
    UnkGhostAuraValueHolder_l314668 holders[6];  // 0xe8
    u8 pad_0x388[0x390 - 0x388];
    UnkGhostAuraListener_l314668 listener;  // 0x390
    UnkGhostAuraCallback_l314668 callback;  // 0x3b0
    UnkGhostAuraList_l314668 list;  // 0x3c8
    u8 pad_0x3f0[0x440 - 0x3f0];
    UnkRefObject* ref;  // 0x440
};
static_assert(__builtin_offsetof(UnkAbilityGhostAura_l314668, list) == 0x3c8, "layout");
static_assert(__builtin_offsetof(UnkAbilityGhostAura_l314668, ref) == 0x440, "layout");

void fn_7100314668(UnkAbilityGhostAura_l314668* self) {
    self->vtable = &g_unk_7101a22a88[2];
    if (UnkRefObject* r = self->ref) {
        if (--r->ref_count == 0)
            r->vf_0x10();
    }
    self->list.Destroy();
    self->callback.Destroy();
    self->listener.Destroy();

    // CAbility's members, then its base.
    self->vtable = &g_unk_7101a223f8[2];
    for (int i = 5; i >= 0; --i)
        self->holders[i].Destroy();
    self->member.Destroy();
    fn_7101043934(reinterpret_cast<UnkCameraCtrl_ovb0049*>(self));
}
