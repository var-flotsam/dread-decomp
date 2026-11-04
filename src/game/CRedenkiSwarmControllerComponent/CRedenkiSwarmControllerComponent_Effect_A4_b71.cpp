// CRedenkiSwarmControllerComponent unit (tools/tu_map.py, the name from reflection): finds the billboard
// component under +0xe8 (as CRedenkiSwarmControllerComponent_Start_b55.cpp), starts the effect keyed by the
// value at +0x30 of its +0xd0 through the global manager (created on first use) and hands it to
// fn_710074e190 with 1.0. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CClass;

struct UnkRedenkiKeyHolder_A4_b71 {
    u8 pad_0x0[0x30];
    u64 field_0x30;
};

struct UnkRedenkiBillboard_A4_b71 {
    void** vtable;
    u8 pad_0x8[0xc8];
    UnkRedenkiKeyHolder_A4_b71* field_0xd0;
};

struct UnkRedenkiChain2_A4_b71 {
    u8 pad_0x0[0x10];
    UnkRedenkiBillboard_A4_b71* field_0x10;
};

struct UnkRedenkiChain1_A4_b71 {
    u8 pad_0x0[0x10];
    UnkRedenkiChain2_A4_b71* field_0x10;
};

struct UnkRedenkiController_A4_b71 {
    u8 pad_0x0[0xe8];
    UnkRedenkiChain1_A4_b71* field_0xe8;
};

extern void* g_unk_71726da048;
extern char g_unk_710164a420[];
void fn_7100e1bca4(void* manager);
void* fn_7100e1cb50(void* manager, u64* key, void* arg);
void fn_710074e190(void* self, void* effect, f32 value);

void fn_7100656bd0(UnkRedenkiController_A4_b71* self) {
    UnkRedenkiChain1_A4_b71* c1 = self->field_0xe8;
    UnkRedenkiBillboard_A4_b71* billboard = nullptr;
    if (c1 && c1->field_0x10) {
        UnkRedenkiBillboard_A4_b71* obj = c1->field_0x10->field_0x10;
        billboard =
            obj && fn_710009d50c(
                       reinterpret_cast<const CClass* (*)(UnkRedenkiBillboard_A4_b71*)>(obj->vtable[0])(obj),
                       fn_710072be34())
                ? obj
                : nullptr;
    }
    void* manager = g_unk_71726da048;
    if (!manager) {
        manager = fn_71000839a0(0xe0, 0x3f);
        fn_7100e1bca4(manager);
        g_unk_71726da048 = manager;
    }
    u64 key = billboard->field_0xd0->field_0x30;
    fn_710074e190(self, fn_7100e1cb50(manager, &key, g_unk_710164a420), 1.0f);
}
