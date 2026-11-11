// The CInfesterAIComponent unit: after fn_7101108124, when the flag at +0x146de is set, stops the
// animation layers 6 and 0xc and 0xb of the owner's animation (+0x220 of the owner at +0x30), sends it
// a fixed request with mode 0 (descriptors as UnkSpitSend, src/game_s2/UnkSpitSend.h), clears the
// flag, hands the hash of the id at +0x510 of the object at +0x146c8 to the owner's +0x228
// (fn_7101172780), clears +0x146d8..+0x146dd and calls fn_71005cb6b4(1); otherwise, when the flag
// at +0x146df is set, clears it and stops the three layers with 1 for the last. Names are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkTU2702.h"
#include "types.h"

using base::global::CStrId;

struct UnkQuetzoaAIComponent;
struct UnkActorTarget;
void fn_7101178e08(UnkActorTarget*, u32, u32, u32, f32);
void fn_710117662c(UnkActorTarget*, u32, u32, u32, u32, f32);
struct UnkFlagTarget;

struct UnkInfesterOwner_b57a3 {
    u8 pad_0x0[0x220];
    UnkActorTarget* field_0x220;
    UnkFlagTarget* field_0x228;
};

struct UnkInfesterData_b57a3 {
    u8 pad_0x0[0x510];
    CStrId field_0x510;
};

struct UnkInfesterAI_b57a3 {
    u8 pad_0x0[0x30];
    UnkInfesterOwner_b57a3* owner;
    u8 pad_0x38[0x146c8 - 0x38];
    UnkInfesterData_b57a3* field_0x146c8;
    u8 pad_0x146d0[8];
    u32 field_0x146d8;
    u16 field_0x146dc;
    bool field_0x146de;
    bool field_0x146df;
};

void fn_71005cb6b4(UnkInfesterAI_b57a3* self, bool on);

void fn_71005cb424(UnkInfesterAI_b57a3* self, void* arg) {
    fn_7101108124(reinterpret_cast<UnkQuetzoaAIComponent*>(self), arg);
    UnkActorTarget* anim = self->owner->field_0x220;
    if (self->field_0x146de) {
        u64 hash;
        base::reflection::UnkReflTypeIdArg mode;
        base::reflection::UnkReflTypeIdArg mask;
        if (anim) {
            fn_7101178e08(anim, 6, 0, 0, -1.0f);
            fn_7101178e08(anim, 0xc, 0, 0, -1.0f);
            fn_710117662c(anim, 0xb, 0, 0, 0, -1.0f);
            mode.id = 0;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(anim, 0x56b5846baf59688c, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740,
                          &mask);
        }
        self->field_0x146de = false;
        if (UnkFlagTarget* target = self->owner->field_0x228) {
            base::global::UnkStringData* d = self->field_0x146c8->field_0x510.data;
            if (&g_unk_7101d47108->empty != d)
                ++d->ref_count;
            hash = *reinterpret_cast<u64*>(reinterpret_cast<char*>(d) + 0x30);
            fn_7101172780(target, reinterpret_cast<const char*>(&hash));
            if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
                if (d != &pool->empty && --d->ref_count == 0)
                    fn_71000803a4(pool, d);
            }
        }
        self->field_0x146d8 = 0;
        self->field_0x146dc = 0;
        fn_71005cb6b4(self, true);
    } else if (self->field_0x146df) {
        self->field_0x146df = false;
        fn_7101178e08(anim, 6, 0, 0, -1.0f);
        fn_7101178e08(anim, 0xc, 0, 0, -1.0f);
        fn_710117662c(anim, 0xb, 0, 1, 0, -1.0f);
    }
}
