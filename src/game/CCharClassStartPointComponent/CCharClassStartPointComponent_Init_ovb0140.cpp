// CCharClassStartPointComponent's unit static initialiser: the class registrations, the checkpoint offset
// class type and the "LastCheckpointOffset" property. Names and layouts are placeholders.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflStatics.h"
#include "base/reflection/UnkReflTypeDtors.h"

using namespace unk_static_init;

struct UnkStatics_710116d120 {
    HeaderIds ids;
    u8 pad_0x18[0x10];
    u8 field_0x28[8];
    u8 pad_0x30[0x130];
    u8 field_0x160[8];
};
extern UnkStatics_710116d120 g_unk_7172a0a628 __attribute__((visibility("hidden")));
extern char g_unk_7172a0a4a0[];
extern char g_unk_7172a0a4a1[];
extern UnkEnumType g_unk_7172a0a4a8;
extern char g_unk_7172a0a5e0[];
extern UnkPropertyEx<> g_unk_7172a0a5e8;
extern void* g_unk_7101a45fe8[];
extern void* g_unk_7101a45f90[];
void fn_710116b8ec(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*, void (*)(), void*);
void fn_710116b770();
void fn_71004fa3b8(void* p);

void fn_710116d120() {
    InitHeaderIds(&g_unk_7172a0a628.ids);
    fn_710116aaa0();
    fn_710009ec00(g_unk_7172a0a4a0, g_unk_7172a0a628.field_0x28);
    fn_710116ade8();
    fn_710009ec00(g_unk_7172a0a4a1, g_unk_7172a0a628.field_0x160);
    {
        CStrId n("TCheckpointOffset", true);
        fn_710116b8ec(&g_unk_7172a0a4a8, n, nullptr, nullptr, fn_710116b770, nullptr);
    }
    __cxa_atexit(fn_7100096604, &g_unk_7172a0a4a8, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172a0a5e0, &g_unk_7172a0a4a8);
    InitPropertyExVtableFirst(&g_unk_7172a0a5e8, g_unk_7101a45fe8, g_unk_71726db788, g_unk_7101a45f90, fn_71002ef5bc,
                              "LastCheckpointOffset", &g_unk_7172a0a4a8, fn_71004fd2cc, fn_71004fa3b8);
}
