// CCharClassTunnelTrapMorphballComponent's unit (tools/tu_map.py unit 1560): fn_7100a443c8 (run by the
// message handlers in CTunnelTrapMorphballComponent_Msg_b49a4.cpp and by fn_7100a44350) requests the
// animation 0x6a17390fc912b900 from the owner's animation (+0x220) and hooks the callback fn_7100a45150
// (link at +0x50, handle at +0x68) up to it, as
// src/game/CBigkranXMagmaRainAttack/CBigkranXMagmaRainAttack_Start_A2_b72.cpp; when the definition's flag
// (+0xd0, +0x58) is set, enables the second collider name of g_unk_717274a6e0 and disables the first; then
// marks the owner with g_unk_710164b520 (fn_7100d7c828) and runs the component's vf_0x180 on the game
// manager's blackboard (+0x968). fn_7100a44788 handles the state 1 message: the colliders swapped back with
// the owner's collision mask set (fn_71008659c4), the owner's action switcher sent 0xb15b31b835c17820, the
// breakable parts released as src/game/CCharClassBreakableScenarioComponent/CBreakableScenarioComponent_b36.cpp
// does, the player's forced movement told "ForcedMovementAreaComponent" (a function-local static id), and
// the target link dropped; false when the player has no forced movement object. fn_7100a435b0 is the
// component's destructor body (the parts, the id table at +0x80, the id at +0x78 and the target link at
// +0x50 destroyed, then the component base's fn_71010c4194). Names and layouts are placeholders.
#include "base/UnkStrIdTable.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkActor.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTimelineComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710117b3e8(UnkWeakTarget* target, void* link);
void fn_7100a45150();

// The two collider names (hidden statics of the unit).
extern CStrId g_unk_717274a6e0[2] __attribute__((visibility("hidden")));
extern const u64 g_unk_710164b520;  // reached through the GOT

struct UnkTunnelTrapLink_ovb0136 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkTunnelTrapDef_ovb0136 {
    u8 pad_0x0[0x58];
    bool field_0x58;
};

struct UnkTunnelTrapMorphball;

struct UnkTunnelTrapOwner_ovb0136 {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x78];
    void* field_0x2a0;
};

struct UnkTunnelTrapComponent_ovb0136 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkTunnelTrapOwner_ovb0136* owner;  // 0x30
    u8 pad_0x38[0x18];
    UnkTunnelTrapLink_ovb0136 link;  // 0x50
    UnkWeakHandle* handle;           // 0x68
    u8 pad_0x70[0x60];
    UnkTunnelTrapDef_ovb0136* def;  // 0xd0

    void vf_0x180(void* blackboard, bool flag) {
        reinterpret_cast<void (*)(UnkTunnelTrapComponent_ovb0136*, void*, bool)>(vtable[0x180 / 8])(this, blackboard,
                                                                                                    flag);
    }
};

void fn_7100a443c8(UnkTunnelTrapMorphball* trap) {
    auto* self = reinterpret_cast<UnkTunnelTrapComponent_ovb0136*>(trap);
    if (void* anim = self->owner->field_0x220) {
        base::reflection::refl_EmitU32(anim, 0x6a17390fc912b900, 0xf4061ff25a2163d6, 0, &g_unk_710164b958,
                                       &g_unk_7172769740);
        UnkTunnelTrapLink_ovb0136* link = &self->link;
        link->context = self;
        link->callback = fn_7100a45150;
        if (UnkWeakHandle* handle = self->handle) {
            if (UnkWeakTarget* target = handle->target)
                fn_710117b3f0(target, link);
            if (UnkWeakHandle* p = self->handle) {
                if (--p->ref_count == 0)
                    p->vf_0x10();
                self->handle = nullptr;
            }
        }
        asm("" : "+r"(self));
        fn_7100316018(static_cast<UnkWeakTarget*>(anim), &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
    }
    if (self->def->field_0x58) {
        if (auto* collision = static_cast<UnkCollisionContainer*>(self->owner->field_0x2a0)) {
            fn_7100d35a98(reinterpret_cast<UnkEntry*>(fn_71008640a4(collision, &g_unk_717274a6e0[1])), true);
            fn_7100d35a98(reinterpret_cast<UnkEntry*>(fn_71008640a4(
                              static_cast<UnkCollisionContainer*>(self->owner->field_0x2a0), &g_unk_717274a6e0[0])),
                          false);
        }
    }
    fn_7100d7c828(reinterpret_cast<UnkActor*>(self->owner), &g_unk_710164b520, false);
    self->vf_0x180(&g_unk_7101d47310->field_0x968, true);
}

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
struct UnkOwner;
UnkOwner* fn_7100defcd4(UnkGameManager*);
struct UnkEnergyWavePlayer2_ovb22;
bool fn_7100df5c20(UnkGameManager*, UnkEnergyWavePlayer2_ovb22*);
struct UnkForcedMovementTarget_A5_b68;
void fn_71010db850(UnkForcedMovementTarget_A5_b68* p, void* q);
void fn_7100872858(void* p, void* q);
void fn_71008659c4(UnkCollisionContainer*, u32, u32, void*);
void* fn_71002b3b84(UnkWeakTarget*);
void fn_71009c0f1c(void*, u64);
void fn_71009c0974(void*, bool);
struct UnkBreakablePart;
struct UnkBreakableTarget;
UnkBreakableTarget** fn_71002a76a4(UnkBreakablePart* part);
void* fn_7100751520(void* key);
void fn_710076351c(void* obj);

// The id handed to the player's +0x1d8 object (a hidden static of the unit).
extern u8 g_unk_717274a6f0[] __attribute__((visibility("hidden")));

struct UnkTunnelTrapPart_ovb0136 {
    void** vtable;
    u8 pad_0x8[0x38];
    bool vf_0x20() { return reinterpret_cast<bool (*)(UnkTunnelTrapPart_ovb0136*)>(vtable[0x20 / 8])(this); }
};

struct UnkTunnelTrapTarget_ovb0136 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

struct UnkTunnelTrapPlayer_ovb0136 {
    u8 pad_0x0[0x1d8];
    void* field_0x1d8;
    u8 pad_0x1e0[0x20];
    UnkForcedMovementTarget_A5_b68* field_0x200;
};

struct UnkTunnelTrapOwner2_ovb0136 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* field_0x2a0;
};

struct UnkTunnelTrapComponent2_ovb0136 {
    u8 pad_0x0[0x30];
    UnkTunnelTrapOwner2_ovb0136* owner;  // 0x30
    u8 pad_0x38[0x8];
    u8 field_0x40[0x10];
    u8 link[0x18];          // 0x50
    UnkWeakHandle* handle;  // 0x68
    u8 pad_0x70[0x38];
    UnkTunnelTrapPart_ovb0136* parts;  // 0xa8
    u32 part_count;                    // 0xb0
};

static inline UnkTunnelTrapPlayer_ovb0136* Player() {
    return reinterpret_cast<UnkTunnelTrapPlayer_ovb0136*>(fn_7100dec788(g_unk_7101d47310));
}

bool fn_7100a44788(UnkTunnelTrapComponent2_ovb0136* self, const s32* state) {
    if (*state != 1)
        return true;
    if (UnkCollisionContainer* collision = self->owner->field_0x2a0) {
        fn_71008659c4(collision, 0x10000, 0x10, self->field_0x40);
        fn_7100d35a98(reinterpret_cast<UnkEntry*>(fn_71008640a4(collision, &g_unk_717274a6e0[1])), true);
        fn_7100d35a98(reinterpret_cast<UnkEntry*>(fn_71008640a4(collision, &g_unk_717274a6e0[0])), false);
    }
    if (void* switcher = fn_71002b3b84(reinterpret_cast<UnkWeakTarget*>(self->owner))) {
        fn_71009c0f1c(switcher, 0xb15b31b835c17820);
        fn_71009c0974(switcher, true);
    }
    for (UnkTunnelTrapPart_ovb0136* it = self->parts, *end = it + self->part_count; it != end; ++it) {
        if (it->vf_0x20())
            continue;
        auto* target = reinterpret_cast<UnkTunnelTrapTarget_ovb0136*>(
            *fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(it)));
        if (void* obj = fn_7100751520(target ? target->field_0x10 : nullptr))
            fn_710076351c(obj);
    }
    UnkForcedMovementTarget_A5_b68* forced = Player()->field_0x200;
    if (!forced)
        return false;
    static CStrId s_name("ForcedMovementAreaComponent", true);
    fn_71010db850(forced, &s_name);
    UnkTunnelTrapPlayer_ovb0136* player = Player();
    if (!fn_7100df5c20(g_unk_7101d47310, reinterpret_cast<UnkEnergyWavePlayer2_ovb22*>(player))) {
        auto* other = reinterpret_cast<UnkTunnelTrapPlayer_ovb0136*>(fn_7100defcd4(g_unk_7101d47310));
        if (other->field_0x200)
            fn_71010db850(other->field_0x200, &s_name);
    }
    if ((player = Player())) {
        if (player->field_0x1d8)
            fn_7100872858(player->field_0x1d8, g_unk_717274a6f0);
    }
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, self->link);
        ResetRef(&self->handle);
    }
    return true;
}

extern void* g_unk_7101a8e700[];
extern void* g_unk_7101a22d48[];

// A part of the trap: a polymorphic 0x40-byte record destroyed through its vtable.
struct UnkTunnelTrapPart2_ovb0136 {
    virtual ~UnkTunnelTrapPart2_ovb0136();
    u8 pad_0x8[0x38];
};

// The id at +0x78: released in two steps (the original reloads `data` and the count after the decrement).
struct UnkTunnelTrapId_ovb0136 {
    UnkStringData* data;

    __attribute__((always_inline)) ~UnkTunnelTrapId_ovb0136() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

// The link registered with the target (vtable g_unk_7101a22d48): unregisters itself and drops the target's
// handle when destroyed.
struct UnkTunnelTrapListener_ovb0136 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkWeakHandle* handle;  // 0x18

    __attribute__((always_inline)) ~UnkTunnelTrapListener_ovb0136() {
        vtable = g_unk_7101a22d48 + 2;
        if (UnkWeakHandle* h = handle) {
            if (UnkWeakTarget* target = h->target)
                fn_710117b3f0(target, this);
            if (UnkWeakHandle* p = handle) {
                if (--p->ref_count == 0)
                    p->vf_0x10();
            }
        }
    }
};

// The two id arrays (base/UnkStrIdTable.h): reset, then both arrays freed.
struct UnkTunnelTrapTable_ovb0136 {
    UnkStrIdTable table;

    __attribute__((always_inline)) ~UnkTunnelTrapTable_ovb0136() {
        fn_71004f11f8(&table);
        if (table.keys)
            table.allocator->vf_0x48(table.keys);
        if (table.values)
            table.allocator->vf_0x48(table.values);
    }
};

struct UnkTunnelTrapMorphballComponent_ovb0136 {
    void** vtable;
    u8 pad_0x8[0x38];
    void** vtable_0x40;
    u8 pad_0x48[0x8];
    UnkTunnelTrapListener_ovb0136 listener;  // 0x50
    u8 pad_0x70[0x8];
    UnkTunnelTrapId_ovb0136 id;                             // 0x78
    UnkTunnelTrapTable_ovb0136 table;                       // 0x80
    base::UnkVector<UnkTunnelTrapPart2_ovb0136> parts;      // 0xa8

    __attribute__((always_inline)) ~UnkTunnelTrapMorphballComponent_ovb0136() {
        vtable = g_unk_7101a8e700 + 2;
        vtable_0x40 = g_unk_7101a8e700 + 0x35;
        parts.Free();
    }
};

void fn_7100a435b0(UnkTunnelTrapMorphballComponent_ovb0136* self) {
    self->~UnkTunnelTrapMorphballComponent_ovb0136();
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
