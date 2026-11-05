// CChozoWarriorAIComponent's unit (tools/tu_map.py unit 733): with `reset` set, clears the flags and the
// timer at +0x14999..+0x14a1c, drops the counted object at +0x14a20 and sends the owner's (+0x30)
// sub-object at +0x220 the value 2 under a fixed hash (refl_EmitU32, base/reflection/UnkReflStatics.h);
// otherwise runs fn_71004996b0 while the flag at +0x14a18 is set. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkChozoWarriorAI733;
void fn_71004996b0(UnkChozoWarriorAI733* self);

struct UnkCwRefObj_b57 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;
};

struct UnkCwOwner_b57 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

// The block at +0x14999 (the original addresses it from there).
struct __attribute__((packed)) UnkCwState_b57 {
    bool field_0x0;
    u8 pad_0x1[0x7e];
    bool field_0x7f;
    u8 pad_0x80[0x3];
    f32 field_0x83;
    UnkCwRefObj_b57* field_0x87;
};

struct UnkCwAI_b57 {
    u8 pad_0x0[0x30];
    UnkCwOwner_b57* owner;
};

void fn_710049ad04(UnkCwAI_b57* self, void* context, bool reset) {
    UnkCwState_b57* state = reinterpret_cast<UnkCwState_b57*>(reinterpret_cast<char*>(self) + 0x14999);
    asm("" : "+r"(state));
    if (reset) {
        void* target = self->owner->field_0x220;
        if (!target) {
            return;
        }
        state->field_0x0 = false;
        state->field_0x7f = false;
        state->field_0x83 = 0.0f;
        if (UnkCwRefObj_b57* obj = state->field_0x87) {
            if (--obj->ref_count == 0) {
                obj->vf_0x10();
            }
            state->field_0x87 = nullptr;
        }
        base::reflection::refl_EmitU32(target, 0x4b8786ae5226bd5f, 0xf4061ff25a2163d6, 2, &g_unk_710164b958,
                                       &g_unk_7172769740);
    } else if (state->field_0x7f) {
        fn_71004996b0(reinterpret_cast<UnkChozoWarriorAI733*>(self));
    }
}
