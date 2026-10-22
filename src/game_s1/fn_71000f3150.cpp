// The static initialiser of base::serialization::CTypedRefWriter's unit (helpers in game/UnkTUInit.h): the header
// ids in the other order with the function records, and the writer's three context classes.
#include <new>

#include "base/global/CStrId.h"
#include "game/UnkTUInit.h"

// The header built the other way round: "sources" first, the word last.
struct UnkTUHeaderRev {
    u32 field_0x0;
    CStrId sources;
    CStrId binaries;
};

__attribute__((always_inline)) static inline void InitHeaderRev(UnkTUHeaderRev& h) {
    new (&h.sources) CStrId("sources", true);
    fn_7100000250(fn_710000051c, &h.sources, &g_unk_7101a19000);
    new (&h.binaries) CStrId("binaries", true);
    fn_7100000250(fn_710000051c, &h.binaries, &g_unk_7101a19000);
    h.field_0x0 = 6;
}

// The same with three function records after a gap (and no word stored).
struct UnkTUHeaderRevFns {
    CStrId sources;
    CStrId binaries;
    u8 field_0x10[0x30];
    UnkFnRecord fns[3];
};

extern UnkTUHeaderRevFns g_unk_7101d68140 __attribute__((visibility("hidden")));
extern UnkReflType g_unk_7101d67d80;
extern char g_unk_7101d67eb8;
extern UnkReflType g_unk_7101d67ec0;
extern char g_unk_7101d67ff8;
extern UnkReflType g_unk_7101d68000;
extern char g_unk_7101d68138;
void fn_71000efdb4(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71000efdb0();
void fn_71000eff14(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71000eff10();
void fn_71000f0074(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71000f0070();

void fn_71000f3150() {
    new (&g_unk_7101d68140.sources) CStrId("sources", true);
    fn_7100000250(fn_710000051c, &g_unk_7101d68140.sources, &g_unk_7101a19000);
    new (&g_unk_7101d68140.binaries) CStrId("binaries", true);
    fn_7100000250(fn_710000051c, &g_unk_7101d68140.binaries, &g_unk_7101a19000);
    InitRecord(g_unk_7101d68140.fns[0], fn_710000cf00);
    InitRecord(g_unk_7101d68140.fns[1], fn_710000d470);
    InitRecord(g_unk_7101d68140.fns[2], fn_710000d4f4);
    InitClass(&g_unk_7101d67d80, "base::serialization::CTypedRefWriter::SBaseContext", fn_71000efdb4, fn_71000efdb0);
    fn_710009ec00(&g_unk_7101d67eb8, &g_unk_7101d67d80);
    InitClass(&g_unk_7101d67ec0,
              "base::serialization::CTypedRefWriter::SObjectContext",
              fn_71000eff14,
              fn_71000eff10,
              &g_unk_7101d67d80);
    fn_710009ec00(&g_unk_7101d67ff8, &g_unk_7101d67ec0);
    InitClass(&g_unk_7101d68000,
              "base::serialization::CTypedRefWriter::SCollectionContext",
              fn_71000f0074,
              fn_71000f0070,
              &g_unk_7101d67d80);
    fn_710009ec00(&g_unk_7101d68138, &g_unk_7101d68000);
}
