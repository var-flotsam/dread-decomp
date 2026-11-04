// CChozoWarriorXAIComponent's unit (tools/tu_map.py): runs the base's facing update (fn_7100498db8) and,
// while the counter at +0x149cc is set, sends the owner's (+0x30) sub-object at +0x220 a zero under the
// hash held at +0x30 of the object fn_710049b3cc describes. Names are placeholders.
#include "base/reflection/UnkReflStatics.h"
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkSelf;
struct UnkChecker;

struct UnkWarriorXSendOwner_b58 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkWarriorXSend_b58 {
    u8 pad_0x0[0x30];
    UnkWarriorXSendOwner_b58* owner;
    u8 pad_0x38[0x149cc - 0x38];
    s32 field_0x149cc;
};

struct UnkWarriorXHashed_b58 {
    u8 pad_0x0[0x30];
    u64 hash;
};

void fn_7100498db8(UnkSelf* self);
struct UnkDescribed;
UnkDescribed* fn_710049b3cc(UnkChecker* self);

void fn_710048f8b0(UnkWarriorXSend_b58* self) {
    fn_7100498db8(reinterpret_cast<UnkSelf*>(self));
    if (self->field_0x149cc != 0) {
        void* target = self->owner->field_0x220;
        if (target) {
            UnkWarriorXHashed_b58* d = *reinterpret_cast<UnkWarriorXHashed_b58**>(
                fn_710049b3cc(reinterpret_cast<UnkChecker*>(self)));
            base::reflection::refl_EmitU32(target, d->hash, 0xf4061ff25a2163d6, 0, &g_unk_710164b958,
                                           &g_unk_7172769740);
        }
    }
}
