// The static initialiser of base::animation::CBlendSpaceResource's unit (helpers in game/UnkTUInit.h): the header
// ids in the other order with the function records, the class and its blend targets' vector, and the resource
// type (two source extensions) added to its registry (as in fn_7100005700.cpp).
#include <new>

#include "base/UnkResType.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"

// The data starts with the header built the other way round ("sources" first, no word stored),
// three function records after a gap, and the resource class's type.
struct UnkStatics_71000113b0 {
    CStrId sources;
    CStrId binaries;
    u8 field_0x10[0x10];
    UnkFnRecord fns[3];
    u8 type[8];
};

using unk_static_init::UnkResType;

extern UnkStatics_71000113b0 g_unk_7101d4cdc8 __attribute__((visibility("hidden")));
extern char g_unk_7101d4bd38;
extern char g_unk_7101d4bd39;
extern char g_unk_7101d4bd3a;
extern UnkReflType g_unk_7101d4bd40;
extern UnkReflType g_unk_7101d5e2e0;
extern char g_unk_7101d4be78;
extern char g_unk_7101d4be79;
extern UnkReflType g_unk_7101d4be80;
extern char g_unk_7101d4bfb8;
extern char g_unk_7101d4bfb9;
extern UnkReflType g_unk_7101d4bfc0;
extern char g_unk_7101d4c130;
extern char g_unk_7101d4c131;
UnkReflType* fn_7100009838();
UnkReflType* fn_7100009914();
void fn_7100009d80(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71000099f0();
UnkReflType* fn_7100009f04();
void fn_710000a370(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100009fe0();
UnkReflType* fn_710000a4f4();
void fn_710000a5d0(UnkReflType* self, const CStrId& name, void* a, void* b);
extern UnkReflType g_unk_7101d4cfb0 __attribute__((visibility("hidden")));
extern char g_unk_7101d4c138[];

void fn_71000113b0() {
    new (&g_unk_7101d4cdc8.sources) CStrId("sources", true);
    fn_7100000250(fn_710000051c, &g_unk_7101d4cdc8.sources, &g_unk_7101a19000);
    new (&g_unk_7101d4cdc8.binaries) CStrId("binaries", true);
    fn_7100000250(fn_710000051c, &g_unk_7101d4cdc8.binaries, &g_unk_7101a19000);
    InitRecord(g_unk_7101d4cdc8.fns[0], fn_710000cf00);
    InitRecord(g_unk_7101d4cdc8.fns[1], fn_710000d470);
    InitRecord(g_unk_7101d4cdc8.fns[2], fn_710000d4f4);
    fn_71000092e4();
    fn_710009ec00(&g_unk_7101d4bd38, reinterpret_cast<UnkReflType*>(g_unk_7101d4cdc8.type));
    fn_710009ec00(&g_unk_7101d4bd39, fn_7100009838());
    fn_710009ec00(&g_unk_7101d4bd3a, fn_7100009914());
    InitClass(&g_unk_7101d4bd40,
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<base::animation::CBlendSpaceResource::"
              "SBlendTarget>>::TKeyElementIterator::value_type",
              fn_7100009d80,
              fn_71000099f0,
              &g_unk_7101d5e2e0);
    fn_710009ec00(&g_unk_7101d4be78, &g_unk_7101d4bd40);
    fn_710009ec00(&g_unk_7101d4be79, fn_7100009f04());
    InitClass(&g_unk_7101d4be80,
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<base::animation::CBlendSpaceResource::"
              "SBlendTarget>>::TKeyElementConstIterator::value_type",
              fn_710000a370,
              fn_7100009fe0,
              &g_unk_7101d5e2e0);
    fn_710009ec00(&g_unk_7101d4bfb8, &g_unk_7101d4be80);
    fn_710009ec00(&g_unk_7101d4bfb9, fn_710000a4f4());
    InitVector(&g_unk_7101d4bfc0,
               "base::global::CRntVector<base::animation::CBlendSpaceResource::SBlendTarget>",
               fn_710000a5d0);
    fn_710009ec00(&g_unk_7101d4c130, &g_unk_7101d4bfc0);
    fn_710000a7cc();
    fn_710009ec00(&g_unk_7101d4c131, &g_unk_7101d4cfb0);
    {
        UnkResType types[1];
        const char* sources0[2];
        sources0[0] = ".mbls";
        sources0[1] = ".mblst";
        fn_7100004578(&types[0], "base::animation::CBlendSpaceResource", ".bmbls", sources0, 2);
        fn_7100004698(g_unk_7101d4c138, "blendspace", types, 1);
        fn_7100004a44(&types[0]);
    }
    fn_7100000250(fn_7100004b94, g_unk_7101d4c138, &g_unk_7101a19000);
}
