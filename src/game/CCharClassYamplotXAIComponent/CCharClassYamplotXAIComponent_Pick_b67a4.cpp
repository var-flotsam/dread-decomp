// CCharClassYamplotXAIComponent's unit (tools/tu_map.py, the name from reflection): an override that
// answers g_unk_717270c328 when the actor's class data (+0x218) has its flag at +0xf9 set and the
// actor's AI component (+0x240, cast to CYamplotXAIComponent) has the byte at +0x14761 clear; else it
// defers to the base fn_7100414170 with the same arguments. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

struct UnkYamplotPick_b67a4;
extern UnkYamplotPick_b67a4 g_unk_717270c328 __attribute__((visibility("hidden")));

const UnkYamplotPick_b67a4* fn_7100414170(void* self, void* actor, void* a, void* b);

struct UnkYamplotData_b67a4 {
    u8 pad_0x0[0xf9];
    bool field_0xf9;
};

struct UnkYamplotAIState_b67a4 {
    u8 pad_0x0[0x14761];
    bool field_0x14761;
};

struct UnkYamplotActor_b67a4 {
    u8 pad_0x0[0x218];
    UnkYamplotData_b67a4* data;  // 0x218
    u8 pad_0x220[0x240 - 0x220];
    base::reflection::UnkReflObject* ai;  // 0x240
};

const UnkYamplotPick_b67a4* fn_71006eec40(void* self, UnkYamplotActor_b67a4* actor, void* a, void* b) {
    UnkYamplotData_b67a4* data = actor->data;
    base::reflection::UnkReflObject* ai = actor->ai;
    UnkYamplotAIState_b67a4* cast = ai && fn_710009d50c(ai->vf_0x0(), fn_71006ef064())
                                        ? reinterpret_cast<UnkYamplotAIState_b67a4*>(ai)
                                        : nullptr;
    if (data->field_0xf9 && !cast->field_0x14761)
        return &g_unk_717270c328;
    return fn_7100414170(self, actor, a, b);
}
