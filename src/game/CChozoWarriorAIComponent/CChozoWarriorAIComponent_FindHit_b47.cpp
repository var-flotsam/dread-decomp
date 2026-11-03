// CChozoWarriorAIComponent unit (tools/tu_map.py unit 733): a visitor step: unless already found, sets the
// flag when the owner's collision object (+0x2a0) maps the hit target's +0x10 to the string id at
// 0x71726d1270. Names are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkCollisionContainer;

struct UnkWarriorOwner_b47 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* field_0x2a0;
};

struct UnkWarriorAI_b47 {
    u8 pad_0x0[0x30];
    UnkWarriorOwner_b47* owner;  // 0x30
};

struct UnkHitTarget_b47 {
    u8 pad_0x0[0x10];
    void* field_0x10;
};

extern CStrId g_unk_71726d1270 __attribute__((visibility("hidden")));
CStrId* fn_7100864018(UnkCollisionContainer* collision, void* key);

void fn_710049ab8c(UnkWarriorAI_b47* self, UnkHitTarget_b47* const* hit, bool* found) {
    if (*found)
        return;
    UnkCollisionContainer* collision = self->owner->field_0x2a0;
    if (!collision)
        return;
    UnkHitTarget_b47* target = *hit;
    if (!target || !target->field_0x10)
        return;
    if (fn_7100864018(collision, target->field_0x10)->data == g_unk_71726d1270.data)
        *found = true;
}
