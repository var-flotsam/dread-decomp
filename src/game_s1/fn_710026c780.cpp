// The static initialiser of navmesh::CDynamicSmartLinkRule's unit (helpers in game/UnkTUInit.h): the class, kept
// after the header, and the vector of rule pointers with its iterator value types.
#include <new>

#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkTUInit.h"

extern UnkTUHeader g_unk_71726a3680 __attribute__((visibility("hidden")));
extern char g_unk_71726a2d30;
extern char g_unk_71726a2d31;
extern char g_unk_71726a2d32;
extern char g_unk_71726a2d33;
extern UnkReflType g_unk_71726a2d38;
extern UnkReflType g_unk_7101d5e2e0;
extern char g_unk_71726a2e70;
extern char g_unk_71726a2e71;
extern UnkReflType g_unk_71726a2e78;
extern char g_unk_71726a2fb0;
extern char g_unk_71726a2fb1;
extern UnkReflType g_unk_71726a2fb8;
extern char g_unk_71726a3128;
UnkReflType* fn_7100269948();
UnkReflType* fn_7100269a24();
UnkReflType* fn_7100269b00();
void fn_7100269f60(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_7100269bdc();
UnkReflType* fn_710026a0e4();
void fn_710026a544(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_710026a1c0();
UnkReflType* fn_710026a6c8();
void fn_710026a7a4(UnkReflType* self, const CStrId& name, void* a, void* b);

void fn_710026c780() {
    InitHeader(g_unk_71726a3680);
    fn_71002696e4();
    fn_710009ec00(&g_unk_71726a2d30, TUType<0x20>(g_unk_71726a3680));
    fn_710009ec00(&g_unk_71726a2d31, fn_7100269948());
    fn_710009ec00(&g_unk_71726a2d32, fn_7100269a24());
    fn_710009ec00(&g_unk_71726a2d33, fn_7100269b00());
    InitClass(&g_unk_71726a2d38,
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<navmesh::CDynamicSmartLinkRule*>>::"
              "TKeyElementIterator::value_type",
              fn_7100269f60,
              fn_7100269bdc,
              &g_unk_7101d5e2e0);
    fn_710009ec00(&g_unk_71726a2e70, &g_unk_71726a2d38);
    fn_710009ec00(&g_unk_71726a2e71, fn_710026a0e4());
    InitClass(&g_unk_71726a2e78,
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<navmesh::CDynamicSmartLinkRule*>>::"
              "TKeyElementConstIterator::value_type",
              fn_710026a544,
              fn_710026a1c0,
              &g_unk_7101d5e2e0);
    fn_710009ec00(&g_unk_71726a2fb0, &g_unk_71726a2e78);
    fn_710009ec00(&g_unk_71726a2fb1, fn_710026a6c8());
    InitVector(&g_unk_71726a2fb8, "base::global::CRntVector<navmesh::CDynamicSmartLinkRule*>", fn_710026a7a4);
    fn_710009ec00(&g_unk_71726a3128, &g_unk_71726a2fb8);
}
