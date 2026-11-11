// CAINavigationComponent unit (tools/tu_map.py unit 1113): the constructor and the destructor. After the base constructor
// (fn_71010c4148) and the vtable: cleared fields, an empty string id at +0x50, and two bound callbacks
// at +0x68 and +0x88 (vtable, the component, a member-function pointer: fn_71006f540c and fn_71006f549c
// of CAINavigationComponent_b50.cpp, with a zero adjustment). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CAttackComponent.h"
#include "game/CComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkComponent.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkStateObject;
extern void* g_unk_7101a6b608[];  // the component's vtable
extern void* g_unk_7101a2aef8[];  // the callback's vtable
extern void* g_unk_7101a6b780[];  // the second callback's vtable

struct UnkNavComponent_b50;
bool fn_71006f540c(UnkNavComponent_b50* self, u32 value);
bool fn_71006f549c(UnkNavComponent_b50* self, u32 value);

// A counted link to the owner a callback is registered with (as CAINavigationComponent_b39.cpp's).
struct UnkNavLink_b52 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;  // 0x8
    void* owner;    // 0x10
};

// The callbacks share their first 0x20 bytes: vtable, object and member-function pointer; the link at
// +0x18 overlays the adjustment once registered.
struct UnkNavCallback_b52 {
    void** vtable;
    void* object;
    bool (*method)(UnkNavComponent_b50*, u32);
    union {
        u64 adjust;
        UnkNavLink_b52* link;
    };
};

// As src/game/CAINavigationComponent/CAINavigationComponent_b39.cpp and
// src/game/CAbilityGhostAura/UnkListenerDtors_b40.cpp declare them.
struct UnkLinkOwner_ain;
struct UnkListener_ain;

template <int Which>
static inline __attribute__((always_inline)) void Detach_b52(UnkNavCallback_b52* self, void** vtable) {
    self->vtable = vtable;
    UnkNavLink_b52* link = self->link;
    if (!link)
        return;
    if (void* owner = link->owner) {
        if (Which == 0)
            fn_7101137c84(static_cast<UnkLinkOwner_ain*>(owner), reinterpret_cast<UnkListener_ain*>(self));
        else
            fn_71010cb2fc(owner, self);
        link = self->link;
        if (!link)
            return;
    }
    if (--link->ref_count == 0)
        link->Release();
}

struct UnkNavComponent_b52 {
    void** vtable;
    u8 pad_0x8[0x38];
    u64 field_0x40;
    u32 field_0x48;
    u16 field_0x4c;
    u8 field_0x4e;
};

struct UnkNavComponentTail_b52 {
    UnkStringData* id;  // 0x50
    u64 field_0x58;
    u32 field_0x60;
    UnkNavCallback_b52 on_a;  // 0x68
    UnkNavCallback_b52 on_b;  // 0x88
    u64 field_0xa8;
    u64 field_0xb0;
};

void fn_71006f411c(UnkNavComponent_b52* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    UnkNavComponentTail_b52* tail = reinterpret_cast<UnkNavComponentTail_b52*>(reinterpret_cast<char*>(self) + 0x50);
    self->vtable = g_unk_7101a6b608 + 2;
    self->field_0x40 = 0;
    self->field_0x48 = 0;
    self->field_0x4c = 0;
    self->field_0x4e = 0;
    tail->id = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    tail->id = &g_unk_7101d47108->empty;
    tail->field_0x58 = 0;
    tail->field_0x60 = 0;
    tail->on_a.vtable = g_unk_7101a2aef8 + 2;
    tail->on_b.vtable = g_unk_7101a6b780 + 2;
    tail->on_a.method = fn_71006f540c;
    tail->on_a.adjust = 0;
    tail->on_b.method = fn_71006f549c;
    tail->on_b.adjust = 0;
    tail->field_0xa8 = 0;
    tail->field_0xb0 = 0;
    tail->on_a.object = self;
    tail->on_b.object = self;
}

void fn_71006f4228(UnkNavComponent_b52* self) {
    UnkNavComponentTail_b52* tail = reinterpret_cast<UnkNavComponentTail_b52*>(reinterpret_cast<char*>(self) + 0x50);
    self->vtable = g_unk_7101a6b608 + 2;
    Detach_b52<0>(&tail->on_b, g_unk_7101a6b780 + 2);
    Detach_b52<1>(&tail->on_a, g_unk_7101a2aef8 + 2);
    if (UnkStringPool* pool = g_unk_7101d47108) {
        UnkStringData** id = &tail->id;
        if (*id && *id != &pool->empty) {
            --(*id)->ref_count;
            if ((*id)->ref_count == 0) {
                fn_71000803a4(pool, *id);
                pool = g_unk_7101d47108;
            }
        }
        *id = &pool->empty;
    }
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
