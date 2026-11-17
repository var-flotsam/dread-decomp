// behaviortree::CStorePosInBlcAction's unit (tools/tu_map.py, unit 463): fn_7100375e84 runs the node: it
// picks the actor whose position is stored (the owner; the owner's target when the source at +0x38 is
// "Target"; else the entity the blackboard value it names identifies), then stores that actor's position
// under the name at +0x30 (fn_7100395600). Names and layouts are placeholders.
#include "../../../game_aca57c/UnkBufferString.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/behaviortree/CBehavior.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::math::CVector3D;

using UnkBlcValue_l375e84 = UnkBufferString<0x100, g_unk_7101a194b8>;

// Signature as other files declare it (the original returns whether the value was found).
struct UnkStringEqualsCondition_A2_b72;
struct UnkInlineString_A2_b72;
using UnkResolveFn_l375e84 = bool (*)(UnkStringEqualsCondition_A2_b72*, CStrId*, UnkInlineString_A2_b72*);

struct UnkBlcName_l375e84;

struct UnkStorePosInBlc_l375e84 {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
    u8 pad_0x20[0x10];
    UnkBlcName_l375e84* target;  // 0x30
    CStrId source;  // 0x38
};

void fn_7100395600(UnkSelf* behavior, void* record, const CVector3D* value);

// An id built from a string out of line, released like CStrId.
struct UnkBuiltId_l375e84 {
    union {
        CStrId id;
    };

    explicit UnkBuiltId_l375e84(const CRntString& str) { fn_7100000850(&id, str, true); }
    ~UnkBuiltId_l375e84() { id.~CStrId(); }
};

s32 fn_7100375e84(UnkStorePosInBlc_l375e84* self) {
    alignas(8) u32 kind;
    UnkActor* actor = self->owner;
    UnkBlcValue_l375e84 value;
    bool useTarget;
    {
        CRntString name(self->source.Text());
        CStrId targetName("Target", true);
        ++targetName.data->reads;
        useTarget = name.key == static_cast<s64>(targetName.data->Key()) && fn_7100d7c990(self->owner);
    }
    if (useTarget) {
        actor = fn_7100d7c990(self->owner);
    } else if (self->source.data != &g_unk_7101d47108->empty &&
               reinterpret_cast<UnkResolveFn_l375e84>(fn_7100395044)(
                   reinterpret_cast<UnkStringEqualsCondition_A2_b72*>(self), &self->source,
                   reinterpret_cast<UnkInlineString_A2_b72*>(&value))) {
        // The entity the game manager knows by the value's id.
        UnkGameManager* manager = g_unk_7101d47310;
        UnkActor* found;
        {
            u64 key;
            UnkBuiltId_l375e84 built(*value.Str());
            u32 kindValue = g_unk_71727bebb8;
            key = built.id.data->Key();
            kind = kindValue;
            found = static_cast<UnkActor*>(fn_7100deba8c(manager, &key, &kind));
        }
        if (found)
            actor = found;
    }
    if (!actor)
        return 6;
    f32 x = actor->field_0x44;
    f32 y = actor->field_0x54;
    f32 z = actor->field_0x64;
    CVector3D pos;
    pos.x = x;
    pos.y = y;
    pos.z = z;
    fn_7100395600(reinterpret_cast<UnkSelf*>(self), &self->target, &pos);
    return 5;
}
