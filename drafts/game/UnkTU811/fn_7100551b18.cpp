// UnkTU811 (CEmmyAIComponent's unit): fn_7100551b18 answers a message by its string id: for one id it
// lets the grab object at +0x260 go (when the bumped-track check is armed, only for that class) and
// returns 2; for another it tells whether the bumped-track check holds. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CGrabComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::CStrId;

struct UnkEmmyOwner_l551b18 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x260 - 0x228];
    void* field_0x260;
};

struct UnkEmmyAI_l551b18 {
    u8 pad_0x0[0x30];
    UnkEmmyOwner_l551b18* owner;
    u8 pad_0x38[0x29ef4 - 0x38];
    bool checkReleaseTrack;
    bool checkOtherTrack;
};

u32 fn_7100551b18(UnkEmmyAI_l551b18* self, const CStrId& id) {
    bool* flags = &self->checkReleaseTrack;
    if (id == base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x318]) {
        UnkEmmyOwner_l551b18* owner = self->owner;
        if (flags[0]) {
            UnkAnimationComponent* anim = owner->field_0x220;
            if (!anim || !fn_710117b348(anim, fn_7100cee4b8()))
                return 1;
            owner = self->owner;
        }
        void* grab = owner->field_0x260;
        u32 result = 0;
        if (grab) {
            if (!fn_710113d83c(grab))
                return 0;
            flags[0] = false;
            result = 2;
        }
        return result;
    }
    if (id == base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x12e]) {
        if (!flags[1])
            return 0;
        UnkAnimationComponent* anim = self->owner->field_0x220;
        if (anim && fn_710117b348(anim, fn_7100cee4b8()))
            return 0;
    }
    return 1;
}
