// UnkTU1633: the unit's static initialiser (gameeditor::CGameModelQueryBoard's unit): the header data,
// the class type, an empty callback set, and five named string-id values ("ActorLnk" and the four
// number type names). Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "game/UnkTU341.h"
#include "game/UnkTUInit.h"
#include "types.h"

using base::global::CRntVector;

// An empty callback set, as the unit keeps one.
struct UnkCallbackSet_lad4c90 {
    void* const* vtable;
    CRntVector<void*> items;  // 0x8
    u32 field_0x30;
    void* field_0x38;
};

extern UnkTUHeader g_unk_7172756cb8 __attribute__((visibility("hidden")));
extern UnkCallbackSet_lad4c90 g_unk_7172756cd0 __attribute__((visibility("hidden")));
extern char g_unk_71727569f8[];
extern UnkReflType g_unk_7172756a00;
extern char g_unk_7172756b38[];
extern UnkValue g_unk_7172756b40;
extern UnkValue g_unk_7172756b50;
extern UnkValue g_unk_7172756b60;
extern UnkValue g_unk_7172756b70;
extern UnkValue g_unk_7172756b80;

UnkReflType* fn_7100ad3ce4();  // base::reflection::refl_GetType<gameeditor::CGameModelQueryBoard*>
// gameeditor::CGameModelQueryBoard::refl_InitClass
void fn_7100ad4718(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100ad3dc0();

void fn_7100ad4c90() {
    InitHeader(g_unk_7172756cb8);
    fn_710009ec00(g_unk_71727569f8, fn_7100ad3ce4());
    InitClass(&g_unk_7172756a00, "gameeditor::CGameModelQueryBoard", fn_7100ad4718, fn_7100ad3dc0);
    fn_710009ec00(g_unk_7172756b38, &g_unk_7172756a00);

    UnkCallbackSet_lad4c90& callbacks = g_unk_7172756cd0;
    callbacks.vtable = g_unk_7101a20998 + 2;
    new (&callbacks.items) CRntVector<void*>(15);
    callbacks.field_0x30 = 0;
    callbacks.vtable = g_unk_7101a20950 + 2;
    callbacks.field_0x38 = nullptr;
    fn_7100000250(fn_710023ae38, &callbacks, &g_unk_7101a19000);

    new (&g_unk_7172756b40) UnkValue(CStrId("ActorLnk", true), refl_TypeOf<fn_7100009658>);
    fn_7100000250(fn_710010c424, &g_unk_7172756b40, &g_unk_7101a19000);
    new (&g_unk_7172756b50) UnkValue(CStrId("Int32", true), refl_TypeOf<fn_7100009658>);
    fn_7100000250(fn_710010c424, &g_unk_7172756b50, &g_unk_7101a19000);
    new (&g_unk_7172756b60) UnkValue(CStrId("UInt32", true), refl_TypeOf<fn_7100009658>);
    fn_7100000250(fn_710010c424, &g_unk_7172756b60, &g_unk_7101a19000);
    new (&g_unk_7172756b70) UnkValue(CStrId("SizeT", true), refl_TypeOf<fn_7100009658>);
    fn_7100000250(fn_710010c424, &g_unk_7172756b70, &g_unk_7101a19000);
    new (&g_unk_7172756b80) UnkValue(CStrId("Float32", true), refl_TypeOf<fn_7100009658>);
    fn_7100000250(fn_710010c424, &g_unk_7172756b80, &g_unk_7101a19000);
}
