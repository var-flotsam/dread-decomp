// The CEmmyAIComponent unit. fn_71005590fc: with the flag at +0x27720 set, clears the word at
// +0x29eec and enters state g_unk_71726ded60; otherwise takes the owner's part (fn_71005012bc), leaves state
// 1 at +0x29ef0 through vtable slot 0x850, sends a fixed request with mode 0 under kind
// 0x7c5476647f60cdaa to the owner's sub-object (+0x220 of the owner at +0x30; descriptors as
// UnkSpitSend, src/game_s2/UnkSpitSend.h), calls fn_710086c7a8 on the part and enters state g_unk_71726debd0;
// then fn_710054095c. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkTU794.h"
#include "game/UnkTU811.h"
#include "types.h"

struct UnkEmmyActor_b55;
extern char g_unk_71726ded60[];
extern char g_unk_71726debd0[];

struct UnkDoorLifeOwner_b56;
struct UnkDoorLifePart_b56;
void fn_710086c7a8(UnkDoorLifePart_b56* part, void* id, bool on);
extern char g_unk_71726e1f90[] __attribute__((visibility("hidden")));

struct UnkEmmyOwner2_b57a3 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkEmmyAI2_b57a3 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkEmmyOwner2_b57a3* owner;
    u8 pad_0x38[0x27720 - 0x38];
    bool field_0x27720;
    u8 pad_0x27721[0x29eec - 0x27721];
    u32 field_0x29eec;
    s32 field_0x29ef0;

    void vf_0x850() { reinterpret_cast<void (*)(UnkEmmyAI2_b57a3*)>(vtable[0x850 / 8])(this); }
};

void fn_710054095c(UnkEmmyAI2_b57a3* self);

void fn_71005590fc(UnkEmmyAI2_b57a3* self) {
    char* state;
    if (self->field_0x27720) {
        self->field_0x29eec = 0;
        state = g_unk_71726ded60;
    } else {
        UnkDoorLifePart_b56* part = fn_71005012bc(reinterpret_cast<UnkDoorLifeOwner_b56*>(self->owner));
        if (self->field_0x29ef0) {
            self->field_0x29ef0 = 0;
            self->vf_0x850();
        }
        void* target = self->owner->field_0x220;
        if (target) {
            base::reflection::UnkReflTypeIdArg mode;
            mode.id = 0;
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(target, 0xe27a4a9050c8ea64, 0x7c5476647f60cdaa, &mode, &g_unk_710164b958,
                          &g_unk_7172769740, &mask);
        }
        if (part)
            fn_710086c7a8(part, g_unk_71726e1f90, true);
        state = g_unk_71726debd0;
    }
    fn_7100544a34(reinterpret_cast<UnkEmmyActor_b55*>(self), state, true);
    fn_710054095c(self);
}
