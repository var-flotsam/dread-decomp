// Static initialiser of the unit of the CAnimationPrefix::SPrefix::Enum enum.
#include "UnkStaticInit.h"

using namespace unk_static_init;

struct UnkStatics_710070f4d0 {
    HeaderIds ids;
    UnkStrIdBase field_0x18;
    UnkStrIdBase field_0x20;
    UnkStrIdBase field_0x28;
    UnkStrIdBase field_0x30;
    UnkStrIdBase field_0x38;
    UnkStrIdBase field_0x40;
    UnkStrIdBase field_0x48;
    UnkStrIdBase field_0x50;
    UnkStrIdBase field_0x58;
    UnkStrIdBase field_0x60;
    UnkStrIdBase field_0x68;
    UnkStrIdBase field_0x70;
    UnkStrIdBase field_0x78;
    UnkStrIdBase field_0x80;
    UnkStrIdBase field_0x88;
    UnkStrIdBase field_0x90;
    UnkStrIdBase field_0x98;
    UnkStrIdBase field_0xa0;
    UnkStrIdBase field_0xa8;
    UnkStrIdBase field_0xb0;
};
extern UnkStatics_710070f4d0 g_unk_717270e130 __attribute__((visibility("hidden")));
extern UnkEnumType g_unk_717270dfd0;
void fn_710070ef18(UnkEnumType* type, const CStrId& name, void (*factory)(), void* arg);
void fn_710070e70c();
extern char g_unk_717270e0e8[];
extern UnkConversion g_unk_717270e0f0;
extern UnkConversion g_unk_717270e110;
void fn_710070f32c() __attribute__((visibility("hidden")));
void fn_710070f338() __attribute__((visibility("hidden")));

void fn_710070f4d0() {
    InitHeaderIds(&g_unk_717270e130.ids);
    InitEnum(&g_unk_717270dfd0, "CAnimationPrefix::SPrefix::Enum", fn_710070ef18, fn_710070e70c,
             g_unk_717270e0e8, &g_unk_717270e0f0, fn_710070f32c, &g_unk_717270e110, fn_710070f338);
    InitId(&g_unk_717270e130.field_0x18, "water");
    InitId(&g_unk_717270e130.field_0x20, "speedbooster");
    InitId(&g_unk_717270e130.field_0x28, "speedbooster45up");
    InitId(&g_unk_717270e130.field_0x30, "left");
    InitId(&g_unk_717270e130.field_0x38, "right");
    InitId(&g_unk_717270e130.field_0x40, "patrol");
    InitId(&g_unk_717270e130.field_0x48, "search");
    InitId(&g_unk_717270e130.field_0x50, "chase");
    InitId(&g_unk_717270e130.field_0x58, "chase2");
    InitId(&g_unk_717270e130.field_0x60, "chasereachable");
    InitId(&g_unk_717270e130.field_0x68, "combat");
    InitId(&g_unk_717270e130.field_0x70, "flee");
    InitId(&g_unk_717270e130.field_0x78, "brokenshield");
    InitId(&g_unk_717270e130.field_0x80, "grab2");
    InitId(&g_unk_717270e130.field_0x88, "grabwater");
    InitId(&g_unk_717270e130.field_0x90, "protoemmytuto");
    InitId(&g_unk_717270e130.field_0x98, "preseta");
    InitId(&g_unk_717270e130.field_0xa0, "presetb");
    InitId(&g_unk_717270e130.field_0xa8, "presetc");
    InitId(&g_unk_717270e130.field_0xb0, "");
}
