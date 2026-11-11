// CThermalDeviceComponent's unit (tools/tu_map.py unit 1601): the walks over the component's links
// (+0x548, 0x48 bytes each) that resolve each link's door lock and set it to a state, then call the
// script function named at +0x578 or +0x580 with the owner. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CActorComponent.h"
#include "game/CDoorShieldLifeComponent.h"
#include "game/UnkTU0.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkDoorLock;
UnkDoorLock* fn_71002af860(UnkWeakTarget*);
struct UnkDoorShieldStore_A4_b76;

// A counted reference to a weak handle.
struct UnkRef {
    UnkWeakHandle* ptr;
    ~UnkRef() {
        if (ptr && --ptr->ref_count == 0)
            ptr->vf_0x10();
    }
};
UnkRef fn_7100353be8(void*);

struct UnkDoorState_b0279 {
    u8 pad_0x0[0x408];
    u8 field_0x408;
    u8 pad_0x409[3];
    s32 field_0x40c;
    u8 pad_0x410[0xaef - 0x410];
    u8 field_0xaef;
};

struct UnkThermalLink_b0279 {
    u8 pad_0x0[8];
    u8 source[0x28];  // 0x8
    u32 state;        // 0x30
    u8 pad_0x34[4];
    UnkRef handle;    // 0x38
    u32 kind;         // 0x40
    u8 pad_0x44[4];
};
static_assert(sizeof(UnkThermalLink_b0279) == 0x48, "layout");

struct UnkStringEntry_b0279 {
    u8 pad_0x0[0x10];
    const char* text;  // 0x10
    u8 pad_0x18[0x2c];
    union {
        s32 readers;  // 0x44
    };
};

struct UnkThermalDevice_A5_b73;

struct UnkThermalDevice_b0279 {
    u8 pad_0x0[0x30];
    void* owner;  // 0x30
    u8 pad_0x38[0x548 - 0x38];
    UnkThermalLink_b0279* links;  // 0x548
    u32 link_count;               // 0x550
    u8 pad_0x554[0x578 - 0x554];
    UnkStringEntry_b0279* script_a;  // 0x578
    UnkStringEntry_b0279* script_b;  // 0x580
};

static inline void ResolveLink(UnkThermalLink_b0279* link) {
    if (link->state == 1) {
        {
            UnkRef fresh = fn_7100353be8(link->source);
            fn_710031d208(reinterpret_cast<base::UnkRefHandle*>(&link->handle),
                          reinterpret_cast<const base::UnkRefHandle*>(&fresh));
        }
        link->state = 2;
    }
}

static inline UnkWeakTarget* LinkTarget(UnkThermalLink_b0279* link) {
    UnkWeakHandle* h = link->handle.ptr;
    return h ? h->target : nullptr;
}

typedef s32 __attribute__((may_alias)) UnkAliasS32_b0279;

static inline void CallScript(UnkThermalDevice_b0279* self, UnkStringEntry_b0279** slot) {
    if (reinterpret_cast<u64>(*slot) == reinterpret_cast<u64>(&g_unk_7101d47108->empty))
        return;
    ++*reinterpret_cast<UnkAliasS32_b0279*>(&(*slot)->readers);
    const char* text = (*slot)->text;
    fn_7100000790(text ? text : g_unk_710162e2ed, "o", self->owner);
}

void fn_7100a925a0(UnkThermalDevice_A5_b73* component) {
    UnkThermalDevice_b0279* self = reinterpret_cast<UnkThermalDevice_b0279*>(component);
    for (u64 i = 0; i < self->link_count; ++i) {
        UnkThermalLink_b0279* link = &self->links[i];
        ResolveLink(link);
        UnkWeakTarget* target = LinkTarget(link);
        u32 saved = link->kind;
        UnkDoorState_b0279* door = reinterpret_cast<UnkDoorState_b0279*>(fn_71002af860(target));
        if (!door)
            continue;
        u32 kind = saved;
        if ((link->kind & ~1u) == 2) {
            door->field_0x408 = 0;
            door->field_0x40c = kind;
        } else if (door->field_0xaef) {
            door->field_0x408 = 0;
            door->field_0x40c = 2;
        } else {
            door->field_0x408 = 1;
            door->field_0x40c = kind;
        }
        fn_71008db4e4(reinterpret_cast<UnkDoorShieldStore_A4_b76*>(door));
    }
    CallScript(self, &self->script_b);
}

struct UnkStruct_710024fa0c;
UnkStruct_710024fa0c* fn_7100a9312c(UnkWeakTarget*);

void fn_7100a92424(UnkThermalDevice_b0279* self) {
    for (u64 i = 0; i < self->link_count; ++i) {
        UnkThermalLink_b0279* link = &self->links[i];
        ResolveLink(link);
        UnkWeakTarget* target = LinkTarget(link);
        if (UnkStruct_710024fa0c* heat = fn_7100a9312c(target))
            fn_710024fa0c(heat, false);
        UnkDoorState_b0279* door = reinterpret_cast<UnkDoorState_b0279*>(fn_71002af860(target));
        if (!door)
            continue;
        if ((link->kind & ~1u) != 2) {
            door->field_0x40c = 2;
            door->field_0x408 = 0;
        } else if (!door->field_0xaef) {
            door->field_0x40c = 1;
            door->field_0x408 = 1;
        }
        fn_71008db4e4(reinterpret_cast<UnkDoorShieldStore_A4_b76*>(door));
    }
    CallScript(self, &self->script_a);
}
