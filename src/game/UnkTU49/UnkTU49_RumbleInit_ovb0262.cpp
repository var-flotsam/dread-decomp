// Unit 49's static initialiser (helpers in game/UnkTUInit.h): the TU's "sources" and "binaries" ids, the
// enum base::input::ERumbleType with its converters to and from the integer type, and the class
// base::input::TRumbleFileData. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "game/UnkTUInit.h"

struct UnkRumbleHeader_ovb0262w {
    CStrId sources;
    CStrId binaries;
};

extern UnkRumbleHeader_ovb0262w g_unk_7101d586b0 __attribute__((visibility("hidden")));
extern base::reflection::CType g_unk_7101d58410;
extern char g_unk_7101d58528;
extern UnkConverter g_unk_7101d58530;
extern UnkConverter g_unk_7101d58550;
extern UnkReflType g_unk_7101d58570;
extern char g_unk_7101d586a8;
void fn_710007131c(UnkReflType* self, const CStrId& name, void (*init)(), void* extra);
void fn_71000710b0();
void fn_71000716a0() __attribute__((visibility("hidden")));
void fn_71000716ac() __attribute__((visibility("hidden")));
void fn_71000714f0(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_710007149c();

void fn_7100071b80() {
    new (&g_unk_7101d586b0.sources) CStrId("sources", true);
    fn_7100000250(fn_710000051c, &g_unk_7101d586b0.sources, &g_unk_7101a19000);
    new (&g_unk_7101d586b0.binaries) CStrId("binaries", true);
    fn_7100000250(fn_710000051c, &g_unk_7101d586b0.binaries, &g_unk_7101a19000);
    UnkReflType* rumble_type = reinterpret_cast<UnkReflType*>(&g_unk_7101d58410);
    InitEnum(rumble_type, "base::input::ERumbleType", fn_710007131c, fn_71000710b0);
    fn_710009ec00(&g_unk_7101d58528, &g_unk_7101d58410);
    InitEnumConverters(rumble_type, g_unk_7101d58530, fn_71000716a0, g_unk_7101d58550, fn_71000716ac);
    InitClass(&g_unk_7101d58570, "base::input::TRumbleFileData", fn_71000714f0, fn_710007149c);
    fn_710009ec00(&g_unk_7101d586a8, &g_unk_7101d58570);
}
