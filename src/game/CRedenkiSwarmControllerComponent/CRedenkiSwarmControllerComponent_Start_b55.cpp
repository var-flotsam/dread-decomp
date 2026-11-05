// CRedenkiSwarmControllerComponent unit (tools/tu_map.py, the name from reflection): starts a swarm
// member, as the CFingSwarmControllerComponent handler fn_7100579d08 does: clears the byte at +0x180,
// finds the billboard component under the controller's +0xe8 and starts the effect keyed by the value
// at +0x30 of its +0xd0 (creating the global manager on first use), then stores the member's byte
// value shifted by 128 (fn_71007452a4 / fn_71007456fc) and its float at +0x168, and makes the float
// at +0x160 non-negative. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;

struct UnkRedenkiKeyHolder_b55 {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkRedenkiBillboard_b55 {
    void** vtable;
    u8 pad_0x8[0xc8];
    UnkRedenkiKeyHolder_b55* field_0xd0;
};

struct UnkRedenkiChain2_b55 {
    u8 pad_0x0[0x10];
    UnkRedenkiBillboard_b55* field_0x10;
};

struct UnkRedenkiChain1_b55 {
    u8 pad_0x0[0x10];
    UnkRedenkiChain2_b55* field_0x10;
};

struct UnkRedenkiController_b55 {
    u8 pad_0x0[0xe8];
    UnkRedenkiChain1_b55* field_0xe8;
};

// The member type, by the name src/game/CFingSwarmControllerComponent/fn_7100579d08.cpp gives it.
struct UnkTarget {
    u8 pad_0x0[0x160];
    f32 field_0x160;
    u8 pad_0x164[4];
    f32 field_0x168;
    u8 pad_0x16c[0x14];
    u8 field_0x180;
};

extern void* g_unk_71726da048;
extern char g_unk_710164a430[];
void fn_7100e1bca4(void* manager);
void* fn_7100e1cb50(void* manager, u64* key, void* arg);
void fn_7100745440(UnkTarget* target, void* effect, u32 flag);
u8 fn_71007452a4(UnkTarget* target);
void fn_71007456fc(UnkTarget* target, u8 value);

void fn_7100655280(UnkRedenkiController_b55* self, UnkTarget* target) {
    target->field_0x180 = 0;
    UnkRedenkiBillboard_b55* obj = self->field_0xe8->field_0x10->field_0x10;
    UnkRedenkiBillboard_b55* billboard =
        obj && fn_710009d50c(reinterpret_cast<const CClass* (*)(UnkRedenkiBillboard_b55*)>(obj->vtable[0])(obj),
                             fn_710072be34())
            ? obj
            : nullptr;
    void* manager = g_unk_71726da048;
    if (!manager) {
        manager = fn_71000839a0(0xe0, 0x3f);
        fn_7100e1bca4(manager);
        g_unk_71726da048 = manager;
    }
    u64 key = billboard->field_0xd0->field_0x30;
    fn_7100745440(target, fn_7100e1cb50(manager, &key, g_unk_710164a430), 0);
    u8 value = fn_71007452a4(target) + 128;
    fn_71007456fc(target, value);
    target->field_0x168 = value;
    if (target->field_0x160 < 0.0f)
        target->field_0x160 = -target->field_0x160;
}
