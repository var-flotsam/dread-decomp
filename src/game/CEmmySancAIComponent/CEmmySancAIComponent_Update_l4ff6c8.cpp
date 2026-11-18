// CEmmySancAIComponent's unit (tools/tu_map.py unit 793): the update step. While the chase zone at
// +0x2a728 is on, tests the owner's position against the zone's shape entries; the named entry of the
// component's list is handed on (fn_710110f744 with the zone on, else fn_710110f8ac), the key
// "OverridenTimeOutOfCameraToEndChase" is set or cleared by the test, then the base update runs.
// Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkTU108.h"
#include "game/UnkTU1176.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU365.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

struct UnkEntry;
struct UnkTracker;
struct UnkNamedEntry_7100772cb0;
struct UnkNamedList_7100772cb0;
struct UnkEmmyForestAI_A2_b70;

void fn_710110f744(void* self, void* object);
void fn_710054d16c(UnkEmmyForestAI_A2_b70* self, f32 dt);

extern CStrId g_unk_71726dd8f0 __attribute__((visibility("hidden")));

struct UnkSancZoneEntry_l4ff6c8 {
    u8 data[0x48];
};

struct UnkSancZoneShape_l4ff6c8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();

    u8 pad_0x8[0xa0];
    UnkSancZoneEntry_l4ff6c8* entries;  // 0xa8
    u32 entry_count;                    // 0xb0
};

struct UnkSancZoneTracker_l4ff6c8 {
    u8 pad_0x0[0x30];
    UnkSancZoneShape_l4ff6c8* shape;
};

struct UnkSancZoneHolder_l4ff6c8 : base::UnkRefCounted {
    void* target;
};

struct UnkSancOwner_l4ff6c8 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
};

struct UnkSancListHolder_l4ff6c8 {
    u8 pad_0x0[0x10];
    UnkNamedList_7100772cb0* list;
};

// The zone the E.M.M.I. ends its chase in: its shape is reached through the handle at +0x40, resolved
// in state 1 from the member at +0x10.
struct UnkSancChaseZone_l4ff6c8 {
    bool enabled;
    u8 pad_0x1[0xf];
    u8 source[0x28];  // 0x10
    s32 state;        // 0x38
    u8 pad_0x3c[4];
    base::UnkRefHandle handle;  // 0x40

};

struct UnkSancAI_l4ff6c8 {
    u8 pad_0x0[0x30];
    UnkSancOwner_l4ff6c8* owner;  // 0x30
    u8 pad_0x38[0x29558 - 0x38];
    UnkSancListHolder_l4ff6c8* list_holder;  // 0x29558
    u8 pad_0x29560[0x2a4f8 - 0x29560];
    UnkGuiKeyList keys;  // 0x2a4f8
    u8 pad_0x2a528[0x2a728 - 0x2a528];
    UnkSancChaseZone_l4ff6c8 zone;  // 0x2a728
};
static_assert(__builtin_offsetof(UnkSancAI_l4ff6c8, zone) == 0x2a728, "UnkSancAI_l4ff6c8 layout");

// Whether the owner's position lies on one of the shape entries of the chase zone.
static bool InChaseZone(UnkSancAI_l4ff6c8* self) {
    UnkSancChaseZone_l4ff6c8& zone = self->zone;
    if (!zone.enabled || zone.state == 0)
        return false;
    if (zone.state == 1) {
        {
            UnkRefPtr reference = fn_710023b580(zone.source);
            fn_7100234f78(&zone.handle, reinterpret_cast<const base::UnkRefHandle*>(&reference));
        }
        zone.state = 2;
    }
    auto* holder = static_cast<UnkSancZoneHolder_l4ff6c8*>(zone.handle.ptr);
    auto* tracker =
        reinterpret_cast<UnkSancZoneTracker_l4ff6c8*>(fn_710028aca8(holder ? holder->target : nullptr));
    if (!tracker)
        return false;
    UnkSancZoneShape_l4ff6c8* shape = tracker->shape;
    if (!shape || shape->vf_0x18() != 0x10 || shape->entry_count == 0)
        return false;
    for (UnkSancZoneEntry_l4ff6c8* entry = shape->entries; entry != shape->entries + shape->entry_count;
         ++entry) {
        f32 x = self->owner->x;
        f32 y = self->owner->y;
        f32 z = self->owner->z;
        CVector3D position;
        position.x = x;
        position.y = y;
        position.z = z;
        CVector2D point;
        fn_710007b430(&point, &position);
        if (fn_7100116de0(reinterpret_cast<UnkEntry*>(entry), &point, 0, 1, 1.0f))
            return true;
    }
    return false;
}

void fn_71004ff6c8(UnkSancAI_l4ff6c8* self, f32 dt) {
    bool inside = InChaseZone(self);
    if (UnkSancListHolder_l4ff6c8* holder = self->list_holder) {
        if (UnkNamedList_7100772cb0* list = holder->list) {
            if (char* entry = reinterpret_cast<char*>(fn_7100772cb0(list, &g_unk_71726dd8f0))) {
                if (self->zone.enabled)
                    fn_710110f744(self, entry + 0x58);
                else
                    fn_710110f8ac(self, entry + 0x58);
            }
        }
    }
    static CStrId s_key("OverridenTimeOutOfCameraToEndChase", true);
    if (inside)
        fn_710002fd10(&self->keys, &s_key);
    else
        fn_710002ffb4(&self->keys, &s_key);
    fn_710054d16c(reinterpret_cast<UnkEmmyForestAI_A2_b70*>(self), dt);
}
