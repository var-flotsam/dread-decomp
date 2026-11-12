// CGooplotAIComponent's unit (tools/tu_map.py unit 851): fn_7100599e0c. After the base
// (fn_7100369650), the actor (+0x30) is handed to the lazily created manager's list at +0x138
// (fn_710059c970); then the component's state is reset (flags, counters, a vector set to the
// zero vector) and fn_7100599ec4 runs. Names and layouts are placeholders.
#include "base/math/UnkZeroVector3.h"
#include "base/UnkVector.h"
#include "game/UnkManager_71726af468.h"
#include "types.h"

#include "game/UnkBehaviorTree.h"

struct UnkGooplotAI_A3_b66;
void fn_710059c970(void* list, void* actor);
void fn_7100599ec4(UnkGooplotAI_A3_b66* self);

struct UnkGooplotFlags_A3_b66 {
    bool field_0x0;
    bool field_0x1;
    bool field_0x2;
    u8 pad_0x3[0x197];
    bool field_0x19a;
    u8 pad_0x19b[0x25];
    u32 field_0x1c0;
};

struct UnkGooplotTarget_A3_b66 {
    u32 x, y, z;
    u32 field_0xc;
    u32 field_0x10;
};

struct UnkGooplotAI_A3_b66 {
    u8 pad_0x0[0x30];
    void* actor;  // +0x30
    u8 pad_0x38[0x146d8 - 0x38];
    UnkGooplotFlags_A3_b66 flags;  // +0x146d8
    UnkGooplotTarget_A3_b66 target;  // +0x1489c
};

void fn_7100599e0c(UnkGooplotAI_A3_b66* self) {
    UnkGooplotTarget_A3_b66* target = &self->target;
    UnkGooplotFlags_A3_b66* flags = &self->flags;
    asm("" : "+r"(flags));
    fn_7100369650(reinterpret_cast<char*>(self));
    if (!g_unk_71726af468) {
        UnkManager_71726af468* m = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(m);
        g_unk_71726af468 = m;
    }
    fn_710059c970(reinterpret_cast<char*>(g_unk_71726af468) + 0x138, self->actor);
    flags->field_0x19a = false;
    flags->field_0x2 = false;
    flags->field_0x0 = false;
    flags->field_0x1 = true;
    flags->field_0x1c0 = 0;
    const u32* zero = reinterpret_cast<const u32*>(&g_unk_710164779c);
    target->field_0x10 = 0;
    target->x = zero[0];
    target->y = zero[1];
    target->z = zero[2];
    target->field_0xc = 0;
    fn_7100599ec4(self);
}
