// The CEmmyAIComponent unit. fn_710055b1e8: enters state g_unk_71726debd0 (off), clears the id property
// g_unk_71726debd8 of the blackboard section g_unk_71726b7cb8 (blackboard at +0x1260, unless locked) to
// the empty id; stops layer 8 of the owner's (+0x30) sub-object at +0x220 and sends it a two under the
// "relax" id (refl_EmitU32, first descriptor g_unk_710164b968), then fn_7101176a88 on it. When the owner
// has an actor to act against and fn_710054782c says no, asks fn_7100546a30 for state 2 against it, resets
// the record at +0x14b40 when it is live and leaves with fn_71005479a0. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAnimationComponent.h"
#include "game/CBlackboard.h"
#include "game/UnkActor.h"
#include "game/UnkTU811.h"
#include "game/UnkTUProperty.h"
#include "game/behaviortree/CBehavior.h"
#include "game/behaviortree/CEmmyChaseAction.h"
#include "types.h"

using base::global::CStrId;

struct UnkEmmyActor_b55;

// A typed value as the section setter takes it (as in src/game/CDoorLifeComponent.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

extern UnkPropertyName g_unk_71726debd8;

struct UnkActorTarget;
void fn_710117662c(UnkActorTarget*, u32 value, u32 extra, bool apply, bool flag, f32 time);
extern u32 g_unk_7101649db8;
extern UnkDesc_SpitSend g_unk_710164b968;  // reached through the GOT

struct UnkEmmyRelaxOwner_b59 {
    u8 pad_0x0[0x220];
    UnkActorTarget* field_0x220;
};

struct UnkEmmyRelaxRecord_b59 {
    char* field_0x0;
    u8 pad_0x8[0x4];
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u8 pad_0x18[0x2];
    bool field_0x1a;
    u8 pad_0x1b[0x40 - 0x1b];
    u8 field_0x40;
    u8 pad_0x41[0x3];
    u32 field_0x44;
    u32 field_0x48;
    u32 field_0x4c;
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
};

struct UnkEmmyRelaxAI_b59 {
    u8 pad_0x0[0x30];
    UnkEmmyRelaxOwner_b59* owner;
    u8 pad_0x38[0x1260 - 0x38];
    CBlackboard blackboard;
    u8 pad_after[0x14b40 - 0x1260 - sizeof(CBlackboard)];
    UnkEmmyRelaxRecord_b59 field_0x14b40;
};

bool fn_710054782c(UnkEmmyRelaxAI_b59* self);
void fn_7100552324(UnkEmmyRelaxAI_b59* self);

// The typed value lives in the caller's frame for the whole function: the original gives it its own slot.
static inline void SetSectionId(CBlackboard* blackboard, const CStrId& section, UnkPropertyName* name, CStrId* value,
                                UnkTypedValue& v) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            v.type = fn_7100009658();
            v.value = value;
            fn_7100332c80(s, name, &v);
        }
    }
}

void fn_710055b1e8(UnkEmmyRelaxAI_b59* self) {
    UnkTypedValue v;
    base::reflection::UnkReflTypeIdArg mode;
    base::reflection::UnkReflTypeIdArg mask;
    fn_7100544a34(reinterpret_cast<UnkEmmyActor_b55*>(self), g_unk_71726debd0, false);
    SetSectionId(&self->blackboard, g_unk_71726b7cb8, &g_unk_71726debd8, &g_unk_7101d470e0, v);
    if (self->owner->field_0x220) {
        fn_710117662c(self->owner->field_0x220, 8, g_unk_7101649db8, false, false, -1.0f);
        UnkActorTarget* target = self->owner->field_0x220;
        u64 hash = fn_7100001570("relax", 5);
        mode.id = 2;
        mask.id = base::reflection::UnkReflTypeId<base::reflection::UnkReflTypeIdTag>::id();
        fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b968, &g_unk_7172769740, &mask);
        fn_7101176a88(self->owner->field_0x220, 1);
    }
    if (UnkActor* other = fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner))) {
        if (!fn_710054782c(self)) {
            UnkEmmyRelaxRecord_b59* r = &self->field_0x14b40;
            fn_7100546a30(self, 2, other, nullptr);
            if (r->field_0x0 && *reinterpret_cast<void**>(r->field_0x0 + 0x10)) {
                u32 a = r->field_0xc;
                r->field_0x44 = r->field_0x10;
                r->field_0x48 = r->field_0x14;
                r->field_0x4c = 0;
                r->field_0x50 = a;
                r->field_0x54 = a;
                if (r->field_0x1a)
                    r->field_0x58 = a;
                if (r->field_0x40 != 1) {
                    r->field_0x40 = 1;
                    fn_7100552324(self);
                }
            }
            fn_71005479a0(self);
        }
    }
}
