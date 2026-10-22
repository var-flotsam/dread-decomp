// The static initialiser of CActorSublayer's TU (helpers in game/UnkTUInit.h): the header and the class.
#include <new>

#include "base/global/CStrId.h"
#include "game/UnkTUInit.h"

extern UnkTUHeader g_unk_71726a2168 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_71726a1cd8;
extern char g_unk_71726a1e10;
extern char g_unk_71726a1e11;
extern char g_unk_71726a1e12;
extern char g_unk_71726a1e13;
void fn_71002624c8(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100261c94();
UnkReflType* fn_7100262878();
UnkReflType* fn_7100262954();
UnkReflType* fn_7100262a30();

void fn_7100264490() {
    InitHeader(g_unk_71726a2168);
    InitClass(&g_unk_71726a1cd8, "CActorSublayer", fn_71002624c8, fn_7100261c94);
    fn_710009ec00(&g_unk_71726a1e10, &g_unk_71726a1cd8);
    fn_710009ec00(&g_unk_71726a1e11, fn_7100262878());
    fn_710009ec00(&g_unk_71726a1e12, fn_7100262954());
    fn_710009ec00(&g_unk_71726a1e13, fn_7100262a30());
}
