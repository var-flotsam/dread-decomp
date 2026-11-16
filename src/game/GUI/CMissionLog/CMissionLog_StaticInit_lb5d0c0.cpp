// Static initialiser of GUI::CMissionLog's unit: the header statics, the reflected
// CollectionTypeMapper<GUI::CMissionLog::TMissionLogEntries> iterator value types and the TMissionLogEntries
// vector type, then the unit's string ids (widget names, events and captions). Names and layouts are placeholders.
#include "game/GUI/CMissionLog.h"
#include "../../../game_s7/UnkStaticInitS7.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"

using namespace unk_static_init;

// The unit's statics (the compiler merges them; the ids below live in the same block).
struct UnkStatics_7100b5d0c0 {
    HeaderIds ids;
    u8 pad_0x18[0xb8];
    f32 field_0xd0;
    f32 field_0xd4;
    bool field_0xd8;
    u8 pad_0xd9[7];
    u8 field_0xe0[8];
};
static_assert(__builtin_offsetof(UnkStatics_7100b5d0c0, field_0xe0) == 0xe0, "layout");
extern UnkStatics_7100b5d0c0 g_unk_7172764fe8 __attribute__((visibility("hidden")));
extern UnkStrIdBase g_unk_7172765008[22] __attribute__((visibility("hidden")));

extern char g_unk_71727647a8[];
extern char g_unk_71727647a9[];
extern char g_unk_71727647aa[];
UnkEnumType* fn_7100b54c68();  // refl_GetType<CollectionTypeMapper<TMissionLogEntries>::TElementIterator>
UnkEnumType* fn_7100b54d44();  // refl_GetType<CollectionTypeMapper<TMissionLogEntries>::TElementConstIterator>
extern UnkEnumType g_unk_71727647b0;
void fn_7100b54e20(base::reflection::CClass* cls);
void fn_7100b551a4(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*,
                   void (*)(base::reflection::CClass*), void*);
extern char g_unk_71727648e8[];
extern char g_unk_71727648e9[];
UnkEnumType* fn_7100b55328();
extern UnkEnumType g_unk_71727648f0;
void fn_7100b55404(base::reflection::CClass* cls);
void fn_7100b55788(UnkEnumType* type, const CStrId& name, const base::reflection::CType*, void*,
                   void (*)(base::reflection::CClass*), void*);
extern char g_unk_7172764a28[];
extern char g_unk_7172764a29[];
UnkEnumType* fn_7100b5590c();
void fn_7100b559e8(UnkEnumType* type, const CStrId& name, void*, void*);
extern char g_unk_7172764ba0[];

void fn_7100b5d0c0() {
    InitHeaderIds(&g_unk_7172764fe8.ids);
    g_unk_7172764fe8.field_0xd0 = -1.0f;
    g_unk_7172764fe8.field_0xd4 = -1.0f;
    g_unk_7172764fe8.field_0xd8 = true;
    fn_7100b54a10();
    fn_710009ec00(g_unk_71727647a8, g_unk_7172764fe8.field_0xe0);
    fn_710009ec00(g_unk_71727647a9, fn_7100b54c68());
    fn_710009ec00(g_unk_71727647aa, fn_7100b54d44());
    {
        CStrId n("base::reflection::CollectionTypeMapper<GUI::CMissionLog::TMissionLogEntries>::TKeyElementIterator::"
                 "value_type",
                 true);
        fn_7100b551a4(&g_unk_71727647b0, n, &g_unk_7101d5e2e0, nullptr, fn_7100b54e20, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_71727647b0, g_unk_7101a19000);
    fn_710009ec00(g_unk_71727648e8, &g_unk_71727647b0);
    fn_710009ec00(g_unk_71727648e9, fn_7100b55328());
    {
        CStrId n("base::reflection::CollectionTypeMapper<GUI::CMissionLog::TMissionLogEntries>::"
                 "TKeyElementConstIterator::value_type",
                 true);
        fn_7100b55788(&g_unk_71727648f0, n, &g_unk_7101d5e2e0, nullptr, fn_7100b55404, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_71727648f0, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172764a28, &g_unk_71727648f0);
    fn_710009ec00(g_unk_7172764a29, fn_7100b5590c());
    {
        CStrId n("GUI::CMissionLog::TMissionLogEntries", true);
        fn_7100b559e8(&g_unk_7172764a30, n, nullptr, nullptr);
    }
    fn_7100000250(fn_710009891c, &g_unk_7172764a30, g_unk_7101a19000);
    fn_710009ec00(g_unk_7172764ba0, &g_unk_7172764a30);
    InitId(&g_unk_7172765008[0], "MissionLogInput");
    InitId(&g_unk_7172765008[1], "Handle");
    InitId(&g_unk_7172765008[2], "Bar");
    InitId(&g_unk_7172765008[3], "Header-R-Bg");
    InitId(&g_unk_7172765008[4], "Header-R-Icon");
    InitId(&g_unk_7172765008[5], "Header-R-Label");
    InitId(&g_unk_7172765008[6], "Back");
    InitId(&g_unk_7172765008[7], "EventsListCompo");
    InitId(&g_unk_7172765008[8], "TutosListCompo");
    InitId(&g_unk_7172765008[9], "dialogcomposition");
    InitId(&g_unk_7172765008[10], "digital_medium");
    InitId(&g_unk_7172765008[11], "OnPageClose");
    InitId(&g_unk_7172765008[12], "toDialog");
    InitId(&g_unk_7172765008[13], "#TERM_CHARACTER_ADAM");
    InitId(&g_unk_7172765008[14], "#TERM_CHARACTER_PROFESSOR");
    InitId(&g_unk_7172765008[15], "toMissionLog");
    InitId(&g_unk_7172765008[16], "#CAPTION_PROFESSOREVENT_PAGE_23");
    InitId(&g_unk_7172765008[17], "#CAPTION_PROFESSOREVENT_PAGE_24");
    InitId(&g_unk_7172765008[18], "#TERM_CHARACTER_SAMUS");
    InitId(&g_unk_7172765008[19], "OnPageOpen");
    InitId(&g_unk_7172765008[20], "#MLOG_DEFEAT_BOSS_CWXE_SHIP");
    InitId(&g_unk_7172765008[21], "#MLOG_INIT");
}
