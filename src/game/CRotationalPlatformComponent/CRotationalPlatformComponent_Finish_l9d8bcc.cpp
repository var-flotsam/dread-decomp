// CRotationalPlatformComponent's unit (tools/tu_map.py unit 1498): the rotation's end (fn_71009d8bcc). The
// player detector of the rotation's direction is switched off, the owner's object is told the
// "finished" id, the closed collision is switched off and the opened one on, the state flags reset, the
// "Used" property set in the current scenario's blackboard section, and the owner's timeline gets two
// ids set. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkStrIdTable.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CAutoolAIComponent.h"
#include "game/CBlackboard.h"
#include "game/CNavMeshItemComponent.h"
#include "game/CScenario.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU372.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CStrId;

struct UnkWeakTarget;

// The unit's data (its statics, merged into one block).
struct UnkRotationalPlatformTU {
    UnkTUHeader header;
    u64 field_0x18;
    CStrId player_detector_right;  // 0x20
    CStrId player_detector_left;   // 0x28
    CStrId collision_opened;       // 0x30
    CStrId collision_closed;       // 0x38
    CStrId initial;                // 0x40
    CStrId finished;               // 0x48
    UnkReflType type;              // 0x50
};

extern UnkRotationalPlatformTU g_unk_7172740970 __attribute__((visibility("hidden")));
extern UnkProperty g_unk_71727408f0;
extern const u64 g_unk_710164b458;
extern const u64 g_unk_710164b460;

struct UnkRotationalPlatformOwner_l9d8bcc {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkRotationalPlatform_l9d8bcc {
    u8 pad_0x0[0x30];
    UnkRotationalPlatformOwner_l9d8bcc* owner;  // 0x30
    u8 pad_0x38[0x50];
    bool field_0x88;
    bool field_0x89;
    u8 pad_0x8a[0x2];
    f32 field_0x8c;
    u8 pad_0x90[0x18];
    UnkStrIdTable properties;  // 0xa8
    u8 pad_0xd0[0x18];
    s32 direction;  // 0xe8
};
static_assert(__builtin_offsetof(UnkRotationalPlatform_l9d8bcc, properties) == 0xa8, "layout");
static_assert(__builtin_offsetof(UnkRotationalPlatform_l9d8bcc, direction) == 0xe8, "layout");

static inline void SetSwitchedOn(UnkCollisionObject* object, bool on) {
    if (object)
        fn_7100d35a98(reinterpret_cast<UnkEntry*>(object), on);
}

// Stores the bool at `value` under `name` in the blackboard section `section` names (unless the
// blackboard is locked).
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

void fn_71009d8bcc(void* object) {
    auto* self = static_cast<UnkRotationalPlatform_l9d8bcc*>(object);
    UnkRotationalPlatformTU& tu = g_unk_7172740970;
    UnkCollisionContainer* collision = self->owner->collision;
    if (self->direction == 1)
        SetSwitchedOn(fn_71008640a4(collision, &tu.player_detector_left), false);
    else
        SetSwitchedOn(fn_71008640a4(collision, &tu.player_detector_right), false);

    if (void* target = fn_71002b3b84(reinterpret_cast<UnkWeakTarget*>(self->owner)))
        fn_71009c0f1c(target, tu.finished.data->Key());

    SetSwitchedOn(fn_71008640a4(collision, &tu.collision_closed), false);
    SetSwitchedOn(fn_71008640a4(collision, &tu.collision_opened), true);

    self->field_0x88 = true;
    self->field_0x89 = false;
    self->field_0x8c = 0.0f;

    UnkGameManager* manager = g_unk_7101d47310;
    if (manager->field_0x608) {
        CBlackboard* blackboard = &manager->field_0x968;
        CStrId section_id = fn_71011a2fbc(manager->field_0x608);
        u64 index = 0;
        u32 count = self->properties.count;
        if (count) {
            CStrId* keys = self->properties.keys;
            for (; index != count; ++index) {
                if (keys[index].data == g_unk_71727408f0.name.data)
                    break;
            }
        }
        UnkStrIdTableValue* values = self->properties.values;
        bool used = true;
        SetSectionBool(blackboard, section_id, reinterpret_cast<UnkPropertyName*>(&values[index].id), &used);
    }

    if (auto* timeline = reinterpret_cast<UnkTimelineComponent*>(
            fn_71003c6c3c(reinterpret_cast<UnkEvacPart_b61*>(self->owner)))) {
        {
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, &g_unk_710164b458, &mask.id, false, -1.0f);
        }
        {
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, &g_unk_710164b460, &mask.id, false, -1.0f);
        }
    }
}
