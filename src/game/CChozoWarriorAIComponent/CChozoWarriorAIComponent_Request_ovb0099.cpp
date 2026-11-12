// CChozoWarriorAIComponent unit (tools/tu_map.py). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CChozoWarriorAIComponent.h"
#include "game/CCollisionComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU2553.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

struct UnkCollisionContainer;
struct UnkVec3;
void fn_710007b444(void* dst, const UnkVec3* value);

extern const UnkVec3 g_unk_71016499f8 __attribute__((visibility("hidden")));

struct UnkChozoWarriorOwner {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x2a0 - 0x228];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkChozoWarriorAI {
    u8 pad_0x0[0x30];
    UnkChozoWarriorOwner* owner;  // 0x30
};

// The damage request being sent.
struct UnkChozoWarriorRequest {
    u8 pad_0x0[0x49];
    bool field_0x49;
    u8 pad_0x4a[0x2];
    u8 field_0x4c[0x4c];
    void* field_0x98;
    u8 pad_0xa0[0x10];
    u32 field_0xb0;
};

// The string a track record's id names, as far as its hash (+0x30).
struct UnkChozoWarriorStringData {
    u8 pad_0x0[0x30];
    u64 hash;
};

// Adjusts an outgoing damage request: kind 5 when the collider it comes from (+0x98) is registered
// under the unit's id, and the fixed direction (+0x4c, flagged at +0x49) while the owner plays a
// shinespark-impact tweak track with the given name hash.
void fn_710049aab4(UnkChozoWarriorAI* self, void*, UnkChozoWarriorRequest* request) {
    if (UnkCollisionContainer* collision = self->owner->collision) {
        if (request->field_0x98) {
            CStrId* id = fn_7100864018(collision, request->field_0x98);
            if (id->data == g_unk_71726d1270.data)
                request->field_0xb0 = 5;
        }
    }
    if (fn_7100df5a4c(g_unk_7101d47310)) {
        if (UnkAnimationComponent* animation = self->owner->animation) {
            UnkTrackQuery query = {fn_7100c624f8(), nullptr, -1.0f};
            fn_710117b08c(animation, &query);
            if (query.result &&
                reinterpret_cast<UnkChozoWarriorStringData*>(query.result->sources.data)->hash ==
                    0x1f25561b70bc2b7f) {
                request->field_0x49 = true;
                fn_710007b444(request->field_0x4c, &g_unk_71016499f8);
            }
        }
    }
}

// A target held through a weak handle: the object at +0x208 is told to stop (fn_71009e85f4).
struct UnkChozoWarriorTarget {
    u8 pad_0x0[0x208];
    void* field_0x208;
};

// A tracked target: a flag, a word and the weak handle of the target.
struct UnkChozoWarriorTracked {
    bool active;
    u32 value;
    UnkWeakHandle* handle;
};

struct UnkChozoWarriorAIFull {
    u8 pad_0x0[0x30];
    UnkChozoWarriorOwner* owner;  // 0x30
    u8 pad_0x38[0x14a18 - 0x38];
    UnkChozoWarriorTracked tracked;  // 0x14a18
};

// Releases the tracked target (telling its +0x208 object to stop first) and sends the owner's
// animation a zero under the unit's hash.
void fn_71004996b0(UnkChozoWarriorAIFull* self) {
    UnkAnimationComponent* animation = self->owner->animation;
    if (!animation)
        return;
    UnkChozoWarriorTracked& tracked = self->tracked;
    if (UnkWeakHandle* handle = tracked.handle) {
        if (UnkChozoWarriorTarget* target = reinterpret_cast<UnkChozoWarriorTarget*>(handle->target)) {
            if (target->field_0x208)
                fn_71009e85f4(target->field_0x208, true);
        }
    }
    tracked.active = false;
    tracked.value = 0;
    ResetRef(&tracked.handle);
    base::reflection::refl_EmitU32(animation, 0xaabb13fe0e3931fb, 0xf4061ff25a2163d6, 0, &g_unk_710164b958,
                                   &g_unk_7172769740);
}
