// CGobblerAIComponent's unit (tools/tu_map.py unit 832): after the base step (fn_7100369650), when the owner's
// (+0x30) slot +0x398 holds a CGobblerSpawnPointComponent, its door link (+0x3b8) has the door face the side the
// owner looks to (fn_71008db24c or fn_71008db440, then fn_71008d498c), its second link's (+0x3f8) model shows
// the flag for that side and hides the other, and the owner's model (+0x228) hides both. Then fn_7100581f88.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBossAIComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkTU1376.h"
#include "game/UnkTU367.h"
#include "types.h"

extern char g_unk_7101649f98[];  // the flag shown when the owner looks right
extern char g_unk_7101649fa0[];  // the flag shown when it looks left

struct UnkGobblerLinkTarget_l581de0 {
    u8 pad_0x0[0x10];
    void* object;
};

struct UnkGobblerSpawnPoint_l581de0 {
    virtual const base::reflection::CClass* vf_0x0();
    u8 pad_0x8[0x3b8 - 0x8];
    u8 door_link[0x40];   // 0x3b8
    u8 model_link[0x40];  // 0x3f8
};

struct UnkGobblerSlot_l581de0 {
    u8 pad_0x0[0x10];
    UnkGobblerSpawnPoint_l581de0* object;
};

struct UnkGobblerOwner_l581de0 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x1b0];
    UnkFlagTarget* model;  // 0x228
    u8 pad_0x230[0x168];
    UnkGobblerSlot_l581de0* field_0x398;
};
static_assert(__builtin_offsetof(UnkGobblerOwner_l581de0, field_0x398) == 0x398, "layout");

struct UnkGobblerAI_l581de0 {
    u8 pad_0x0[0x30];
    UnkGobblerOwner_l581de0* owner;
};

void fn_71008db24c(UnkDoorLock* door, UnkGobblerOwner_l581de0* owner);
void fn_71008db440(UnkDoorLock* door, UnkGobblerOwner_l581de0* owner);
void fn_7100581f88(UnkGobblerAI_l581de0* self);

static inline UnkGobblerLinkTarget_l581de0* LinkTarget(u8* link) {
    return reinterpret_cast<UnkGobblerLinkTarget_l581de0*>(
        *fn_71003b0c4c(reinterpret_cast<UnkZiplineLink_b49*>(link)));
}

// Whether the link leads to an object.
static inline bool IsLinked(u8* link) {
    UnkGobblerLinkTarget_l581de0* target = LinkTarget(link);
    return target && target->object;
}

// Whether the owner looks to the right (its transform's x axis points along +x).
static inline bool LooksRight(UnkGobblerAI_l581de0* self) {
    return fn_710002614c(&self->owner->transform).m[2] > 0.0f;
}

void fn_7100581de0(UnkGobblerAI_l581de0* self) {
    fn_7100369650(reinterpret_cast<char*>(self));
    UnkGobblerSlot_l581de0* slot = self->owner->field_0x398;
    if (slot) {
        UnkGobblerSpawnPoint_l581de0* spawn = slot->object;
        if (spawn && fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(spawn->vf_0x0()),
                                   reinterpret_cast<const base::reflection::CType*>(fn_7100a19b44()))) {
            u8* door_link = spawn->door_link;
            if (IsLinked(door_link)) {
                UnkGobblerLinkTarget_l581de0* target = LinkTarget(door_link);
                UnkDoorLock* door =
                    fn_71002af860(static_cast<UnkWeakTarget*>(target ? target->object : nullptr));
                if (door) {
                    if (LooksRight(self))
                        fn_71008db24c(door, self->owner);
                    else
                        fn_71008db440(door, self->owner);
                    fn_71008d498c(reinterpret_cast<UnkDoorLife_b44a2*>(door), 0, 0, 0);
                }
            }
            u8* model_link = spawn->model_link;
            if (IsLinked(model_link)) {
                UnkActor* actor = static_cast<UnkActor*>(LinkTarget(model_link)->object);
                if (UnkFlagTarget* model = static_cast<UnkFlagTarget*>(actor->field_0x228)) {
                    if (LooksRight(self)) {
                        fn_7101172764(model, g_unk_7101649f98);
                        fn_7101172780(model, g_unk_7101649fa0);
                    } else {
                        fn_7101172764(model, g_unk_7101649fa0);
                        fn_7101172780(model, g_unk_7101649f98);
                    }
                }
            }
            if (UnkFlagTarget* model = self->owner->model) {
                fn_7101172780(model, g_unk_7101649fa0);
                fn_7101172780(model, g_unk_7101649f98);
            }
        }
    }
    fn_7100581f88(self);
}
