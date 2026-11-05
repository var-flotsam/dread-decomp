// CCharClassEmmyValveComponent's unit (tools/tu_map.py): fn_71007b3050 and fn_71007b358c, when the
// sub-object at +0x240 has it and fn_710117ae38 says no for two hashes, send it the first hash with
// mode 0 (as UnkSpitSend, src/game_s2/UnkSpitSend.h), with second descriptor g_unk_7172769794 instead
// when fn_710117ae38 says yes for a third hash; then look the owner's (+0x30) collision container
// (+0x2a0) up for the string id g_unk_717271a2c0 (fn_71008640a4) and call the found object's
// vf_0x60 with 0x408 or 0x40a. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkCollisionRecord.h"
#include "types.h"

extern UnkDesc_SpitSend g_unk_7172769794;  // reached through the GOT
extern const base::global::CStrId g_unk_717271a2c0 __attribute__((visibility("hidden")));

struct UnkValveCollisionObject_b56 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60(u32 value);
};

struct UnkValveOwner_b56 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;
};

struct UnkEmmyValve_b56 {
    u8 pad_0x0[0x30];
    UnkValveOwner_b56* owner;
    u8 pad_0x38[0x240 - 0x38];
    UnkAnimationComponent* field_0x240;
};

static inline __attribute__((always_inline)) void SendAndNotify_b56(UnkEmmyValve_b56* self, u64 h1, u64 h2,
                                                                    u64 h3, u32 value) {
    if (self->field_0x240 && !fn_710117ae38(self->field_0x240, h1, false) &&
        !fn_710117ae38(self->field_0x240, h2, false)) {
        UnkAnimationComponent* target = self->field_0x240;
        base::reflection::UnkReflTypeIdArg mode;
        mode.id = 0;
        UnkDesc_SpitSend* desc = fn_710117ae38(target, h3, false) ? &g_unk_7172769794 : &g_unk_7172769740;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7101176690(target, h1, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, desc, &mask);
    }
    if (UnkCollisionContainer* collision = self->owner->collision) {
        if (UnkCollisionObject* object = fn_71008640a4(collision, &g_unk_717271a2c0))
            reinterpret_cast<UnkValveCollisionObject_b56*>(object)->vf_0x60(value);
    }
}

void fn_71007b3050(UnkEmmyValve_b56* self) {
    SendAndNotify_b56(self, 0xa0e33be014f44bd7, 0x6909d27d2c787fba, 0x6a17390fc912b900, 0x408);
}

void fn_71007b358c(UnkEmmyValve_b56* self) {
    SendAndNotify_b56(self, 0x6a17390fc912b900, 0xe43bbd07b9e307b9, 0xa0e33be014f44bd7, 0x40a);
}
