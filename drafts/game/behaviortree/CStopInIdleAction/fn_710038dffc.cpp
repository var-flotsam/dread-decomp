// behaviortree::CStopInIdleAction's unit (tools/tu_map.py unit 489): the node's update. While the owner was hit
// or turning it runs (2) until that is over; a hit that left it standing restarts the navigation (a kind-5
// parameter block with its four flags set) and plays the idle animation, the name at +0x48 overridden by the
// blackboard value the id at +0x50 names. Otherwise it runs while the idle animation is not yet playing, or
// for the time at +0x3c when no animation was asked for. Returns 2 while running, 5 when done. Names and
// layouts are placeholders.
#include "../../../../src/game_aca57c/UnkBufferString.h"
#include "../../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU57.h"
#include "game/behaviortree/CBehavior.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::global::CStrId;

// An animation name built from a string id (fn_710007eb6c).
struct UnkIdleAnimName_l38dffc : UnkBufferString<0x100, g_unk_7101a194b8> {
    explicit UnkIdleAnimName_l38dffc(const CStrId* id) { CRntString_AssignStrId(Str(), id); }
};

// The kind-5 navigation parameter block (vtable g_unk_7101a25b10), with the flags at +0x38 to +0x3d named.
struct UnkIdleParams_l38dffc {
    void** vtable;
    u8 field_0x8[13];
    u8 pad_0x15[3];
    u32 field_0x18;
    bool field_0x1c;
    u8 pad_0x1d[3];
    u8 field_0x20[16];
    bool field_0x30;
    u8 pad_0x31[3];
    u32 kind;  // 0x34
    bool field_0x38;
    bool field_0x39;
    bool field_0x3a;
    bool field_0x3b;
    bool field_0x3c;
    bool field_0x3d;
};

struct UnkIdleOwner_l38dffc {
    u8 pad_0x0[0x218];
    void* movement;                    // 0x218
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x70];
    void* navigation;  // 0x298
};

struct UnkStopInIdleAction_l38dffc {
    u8 pad_0x0[0x18];
    UnkIdleOwner_l38dffc* owner;  // 0x18
    u8 pad_0x20[0x10];
    void* navigation;  // 0x30
    bool hit;          // 0x38
    bool field_0x39;
    bool turning;  // 0x3a
    bool play_anim;  // 0x3b
    f32 duration;    // 0x3c
    f32 elapsed;     // 0x40
    u8 pad_0x44[4];
    CStrId anim;           // 0x48
    CStrId anim_property;  // 0x50
};

// Signatures as other files declare them.
struct UnkStringEqualsCondition_A2_b72;
struct UnkInlineString_A2_b72;

u32 fn_710038dffc(UnkStopInIdleAction_l38dffc* self, f32 dt) {
    if (self->turning || self->hit) {
        if (self->field_0x39) {
            UnkIdleParams_l38dffc params;
            void* movement = self->owner->movement;
            if (!movement || fn_71010cc3a4(movement) == 4)
                return 2;
            self->hit = false;
            self->field_0x39 = false;

            params.field_0x1c = false;
            params.vtable = g_unk_7101a25b10 + 2;
            __builtin_memset(params.field_0x8, 0, 13);
            params.field_0x30 = false;
            __builtin_memset(params.field_0x20, 0, 16);
            params.field_0x38 = true;
            params.field_0x39 = true;
            params.field_0x3a = true;
            params.field_0x3b = true;
            params.field_0x3d = false;
            params.kind = 5;
            fn_71006f4b0c(self->navigation, &params, 1);

            UnkIdleAnimName_l38dffc name(&self->anim);
            if (self->anim_property.data != &g_unk_7101d47108->empty)
                fn_7100395044(reinterpret_cast<UnkStringEqualsCondition_A2_b72*>(self), &self->anim_property,
                              reinterpret_cast<UnkInlineString_A2_b72*>(&name));
            const char* str = name.Str()->data;
            if (str && *str && self->owner) {
                if (UnkAnimationComponent* target = self->owner->animation) {
                    base::reflection::UnkReflTypeIdArg mode;
                    mode.id = 1;
                    u64 key = name.Str()->key;
                    base::reflection::UnkReflTypeIdArg mask;
                    mask.id = UnkAll_SpitSend().value;
                    fn_7101176690(target, key, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740,
                                  &mask);
                }
            }
            return 2;
        }
        UnkAnimationComponent* animation = self->owner->animation;
        void* movement = self->owner->movement;
        if (movement && fn_71010cc3a4(movement) == 4)
            return 2;
        if (animation && fn_710117b348(animation, fn_7100d2b3a8()))
            return 2;
        return 5;
    }

    if (self->play_anim) {
        UnkIdleAnimName_l38dffc name(&self->anim);
        if (self->anim_property.data != &g_unk_7101d47108->empty)
            fn_7100395044(reinterpret_cast<UnkStringEqualsCondition_A2_b72*>(self), &self->anim_property,
                          reinterpret_cast<UnkInlineString_A2_b72*>(&name));
        u32 status = 5;
        if (self->owner) {
            if (UnkAnimationComponent* animation = self->owner->animation) {
                const CStrId* current = fn_7101177ae8(animation, 0xf4061ff25a2163d6);
                ++current->data->reads;
                if (!CRntString_KeysDiffer(&current->data->String(), name.Str()))
                    status = fn_71011782ec(animation, 0xf4061ff25a2163d6) ? 5 : 2;
            }
        }
        return status;
    }

    self->elapsed += dt;
    return !(self->elapsed >= self->duration) ? 2 : 5;
}
