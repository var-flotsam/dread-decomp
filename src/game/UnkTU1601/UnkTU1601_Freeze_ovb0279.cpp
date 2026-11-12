// CThermalDeviceComponent's unit (tools/tu_map.py unit 1601): freezing or thawing the component
// (fn_7100a928ec). Freezing puts every link's door lock into its frozen state (fn_71008db4e4) and
// clears the two render parameters (fn_710021a0b4); thawing reads the usable flag from the current
// scenario's blackboard section (the name looked up in the key list at +0x4d8, count +0x4e0, values at
// +0x4d0, as in CElevatorUsableComponent_b48.cpp) and runs fn_7100a925a0 or fn_7100a92424 by it, then
// sets the two render parameters. Names and layouts are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "base/math/CVector4D.h"
#include "game/CBlackboard.h"
#include "game/CDoorShieldLifeComponent.h"
#include "game/CScenario.h"
#include "game/CUsableComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU1601.h"
#include "game/UnkTU315.h"
#include "game/UnkTU367.h"
#include "game/UnkTUProperty.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector4D;

struct UnkDoorLock;
struct UnkDoorShieldStore_A4_b76;
extern const u64 g_unk_710164b5f0 __attribute__((visibility("hidden")));
extern const u64 g_unk_710164b5f8 __attribute__((visibility("hidden")));
extern const CVector4D g_unk_7172751c18;
extern const CVector4D g_unk_7101a92ec0;

struct UnkThermalDevice_A5_b73;
struct UnkThermalDevice_b0279;
void fn_7100a92424(UnkThermalDevice_b0279* self);

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

struct UnkThermalValue_b0279 {
    UnkPropertyName name;
    u64 field_0x8;
};

struct UnkThermalDevice_b0279 {
    u8 pad_0x0[0x4d0];
    UnkThermalValue_b0279* values;  // 0x4d0
    u64* keys;                      // 0x4d8
    u32 key_count;                  // 0x4e0
    u8 pad_0x4e4[0x548 - 0x4e4];
    UnkThermalLink_b0279* links;  // 0x548
    u32 link_count;               // 0x550
};

bool fn_7100a928ec(UnkThermalDevice_b0279* self, bool frozen) {
    if (frozen) {
        for (u64 i = 0; i < self->link_count; ++i) {
            UnkThermalLink_b0279* link = &self->links[i];
            if (link->state == 1) {
                {
                    UnkRef fresh = fn_7100353be8(link->source);
                    fn_710031d208(reinterpret_cast<base::UnkRefHandle*>(&link->handle),
                                  reinterpret_cast<const base::UnkRefHandle*>(&fresh));
                }
                link->state = 2;
            }
            UnkWeakHandle* h = link->handle.ptr;
            UnkDoorState_b0279* door =
                reinterpret_cast<UnkDoorState_b0279*>(fn_71002af860(h ? h->target : nullptr));
            if (!door)
                continue;
            door->field_0x408 = 0;
            door->field_0x40c = 2;
            fn_71008db4e4(reinterpret_cast<UnkDoorShieldStore_A4_b76*>(door));
        }
        {
            UnkRenderManager* render = g_unk_7101d47300;
            base::math::UnkVector4D zero;
            fn_710021a0b4(render, &g_unk_710164b5f0, &zero);
        }
        {
            base::math::UnkVector4D zero;
            fn_710021a0b4(g_unk_7101d47300, &g_unk_710164b5f8, &zero);
        }
        return false;
    }
    bool usable = false;
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    {
        CStrId section_id = fn_71011a2fbc(manager->field_0x608);
        u64 index = 0;
        u32 count = self->key_count;
        if (count) {
            u64* keys = self->keys;
            u64 key = (u64)reinterpret_cast<UnkProperty*>(g_unk_7172753148)->name.data;
            for (; index != count; ++index) {
                if (keys[index] == key)
                    break;
            }
        }
        UnkThermalValue_b0279* values = self->values;
        if (CBlackboard::CSection* s = fn_7101047270(blackboard, section_id))
            fn_710006d11c(s, reinterpret_cast<const CStrId*>(&values[index].name), &usable);
    }
    if (usable)
        fn_7100a925a0(reinterpret_cast<UnkThermalDevice_A5_b73*>(self));
    else
        fn_7100a92424(self);
    fn_710021a0b4(g_unk_7101d47300, &g_unk_710164b5f0, &g_unk_7172751c18);
    fn_710021a0b4(g_unk_7101d47300, &g_unk_710164b5f8, &g_unk_7101a92ec0);
    return false;
}
