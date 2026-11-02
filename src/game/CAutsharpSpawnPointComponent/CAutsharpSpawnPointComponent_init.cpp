// The static initialiser of src/game/CAutsharpSpawnPointComponent/CAutsharpSpawnPointComponent_init.cpp7's unit: the TU header with a third id ("spawn_right"), the
// EAutsharpSpawnPointDir enum type with its converters to and from int, then the component's class.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/CEnumType.h"
#include "game/UnkTUInit.h"

// The TU header with one more id after the usual two.
struct UnkTUHeader_7172744418 {
    UnkTUHeader header;
    CStrId field_0x18;
};

extern UnkTUHeader_7172744418 g_unk_7172744418 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71727442b0;
void fn_7100a138a0();
void fn_7100a13a4c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
extern char g_unk_71727443c8;
void fn_7100a14774() __attribute__((visibility("hidden")));
void fn_7100a14780() __attribute__((visibility("hidden")));
extern UnkConverter g_unk_71727443d0;
extern UnkConverter g_unk_71727443f0;
extern char g_unk_7172744410;
extern UnkReflType g_unk_7172744440 __attribute__((visibility("hidden")));

void fn_7100a148d0() {
    InitHeader(g_unk_7172744418.header);
    new (&g_unk_7172744418.field_0x18) CStrId("spawn_right", true);
    fn_7100000250(fn_710000051c, &g_unk_7172744418.field_0x18, &g_unk_7101a19000);
    InitEnum(&g_unk_71727442b0, "EAutsharpSpawnPointDir", fn_7100a13a4c, fn_7100a138a0);
    fn_710009ec00(&g_unk_71727443c8, &g_unk_71727442b0);
    InitEnumConverters(&g_unk_71727442b0, g_unk_71727443d0, fn_7100a14774, g_unk_71727443f0, fn_7100a14780);
    fn_7100a13bd0();
    fn_710009ec00(&g_unk_7172744410, &g_unk_7172744440);
}
