// CDoorLifeComponent's unit (tools/tu_map.py unit 1376): sends the door's owner one of two ids for the
// "StayOpened" flag, shows or hides the door's parts, and stores the flag in the current scenario's
// blackboard section unless the door is locked open. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CNavMeshItemComponent.h"
#include "game/CScenario.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1376.h"
#include "game/UnkTU372.h"
#include "game/UnkTUProperty.h"
#include "types.h"
#include "game/UnkTypedValue.h"

using base::global::CStrId;

struct UnkWeakTarget;

struct UnkStayOpenedId_l8d1944 {
    u8 pad_0x0[0x30];
    u64 hash;
};
// The ids sent to the owner when the flag is clear and set.
__attribute__((visibility("hidden"))) extern UnkStayOpenedId_l8d1944* g_unk_7172731778[2];

static inline void SetSectionBool(CBlackboard* blackboard, const CStrId& section, UnkPropertyName* name, bool* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_710000d228();
            v.value = value;
            fn_7100332c80(s, name, &v);
        }
    }
}

struct UnkDoorOwner_l8d1944 {
    u8 pad_0x0[0x1b4];
    bool field_0x1b4;
};
struct UnkDoorPart_l8d1944 {
    u64 field_0x0;
    bool visible;
};
typedef UnkDoorPart_l8d1944* __attribute__((aligned(4))) UnkDoorPartPtr_l8d1944;
struct UnkDoorLock_l8d1944 {
    u8 pad_0x0[0x11c];
    bool field_0x11c;
    u8 pad_0x11d[0x128 - 0x11d];
    bool field_0x128;
};
struct UnkDoorLifeValue_l8d1944 {
    UnkPropertyName name;
    u64 field_0x8;
};

struct UnkDoorLife_l8d1944 {
    u8 pad_0x0[0x30];
    UnkDoorOwner_l8d1944* owner;
    u8 pad_0x38[0x418 - 0x38];
    bool field_0x418;
    u8 pad_0x419[0x670 - 0x419];
    UnkDoorLifeValue_l8d1944* values;
    u64* keys;
    u32 count;
    u8 pad_0x684[0x750 - 0x684];
    u32 part_count;
    UnkDoorPartPtr_l8d1944 parts[8];
    u8 pad_0x794[0x9f8 - 0x794];
    UnkDoorLock_l8d1944* lock;
};
static_assert(__builtin_offsetof(UnkDoorLife_l8d1944, parts) == 0x754, "layout");
static_assert(__builtin_offsetof(UnkDoorLife_l8d1944, lock) == 0x9f8, "layout");

bool fn_71008d0db4(UnkDoorLife_l8d1944* self);

void fn_71008d1944(UnkDoorLife_l8d1944* self, bool stay_opened) {
    bool value = stay_opened;
    void* target = fn_71002b3b84(reinterpret_cast<UnkWeakTarget*>(self->owner));
    if (!target)
        return;
    fn_71009c0f1c(target, (stay_opened ? g_unk_7172731778[1] : g_unk_7172731778[0])->hash);

    bool shown = fn_71008d0db4(self);
    for (UnkDoorPartPtr_l8d1944* p = self->parts; p != self->parts + self->part_count; ++p)
        (*p)->visible = shown && !stay_opened;

    UnkDoorLock_l8d1944* lock = self->lock;
    if ((!self->field_0x418 || lock->field_0x128) && lock->field_0x11c && self->owner->field_0x1b4)
        return;

    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    u64 index = 0;
    u32 count = self->count;
    if (count) {
        u64* keys = self->keys;
        u64 key = (u64)g_unk_7172731110.name.data;
        for (; index != count; ++index) {
            if (keys[index] == key)
                break;
        }
    }
    UnkDoorLifeValue_l8d1944* values = self->values;
    SetSectionBool(blackboard, section_id, &values[index].name, &value);
}
