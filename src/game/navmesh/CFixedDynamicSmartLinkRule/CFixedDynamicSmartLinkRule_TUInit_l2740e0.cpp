// Static initialiser of navmesh::CFixedDynamicSmartLinkRule's unit (helpers in game/UnkTUInit.h): the
// header, twelve unit directions around the circle, the SMask class, its vector's iterator value
// types and the vector type. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"

using base::math::CVector2D;

static inline UnkReflType* AsUnk(const base::reflection::CType* t) {
    return reinterpret_cast<UnkReflType*>(const_cast<base::reflection::CType*>(t));
}

struct UnkStatics_l2740e0 {
    UnkTUHeader header;
    CVector2D directions[12];  // 0x18: every 30 degrees or so, counter-clockwise from (0.89, 0.45)
    u8 pad_0x78[8];
    u8 field_0x80[8];
};
static_assert(__builtin_offsetof(UnkStatics_l2740e0, field_0x80) == 0x80, "UnkStatics_l2740e0 layout");
extern UnkStatics_l2740e0 g_unk_71726a4350 __attribute__((visibility("hidden")));

extern char g_unk_71726a39b0;
extern base::reflection::CType g_unk_71726a39b8;
extern char g_unk_71726a3af0;
extern char g_unk_71726a3af1;
extern char g_unk_71726a3af2;
extern base::reflection::CType g_unk_71726a3af8;
extern char g_unk_71726a3c30;
extern char g_unk_71726a3c31;
extern base::reflection::CType g_unk_71726a3c38;
extern char g_unk_71726a3d70;
extern char g_unk_71726a3d71;
extern base::reflection::CType g_unk_71726a3d78;
extern char g_unk_71726a3ee8;

void fn_710026d954(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_710026d950();
void fn_710026e014(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_710026dc90(base::reflection::CClass* cls);
void fn_710026e5f8(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_710026e274(base::reflection::CClass* cls);
void fn_710026e858(UnkReflType* self, const CStrId& name, void* a, void* b);  // refl_InitType<CRntVector<SMask>>
const base::reflection::CType* fn_710026dad8();  // refl_GetType<...CRntVector<SMask>>::TElementIterator>
const base::reflection::CType* fn_710026dbb4();  // refl_GetType<...CRntVector<SMask>>::TElementConstIterator>
const base::reflection::CType* fn_710026e198();
const base::reflection::CType* fn_710026e77c();

void fn_71002740e0() {
    UnkStatics_l2740e0& s = g_unk_71726a4350;
    InitHeader(s.header);
    s.directions[0] = {0.89442718f, 0.44721359f};
    s.directions[1] = {0.707106769f, 0.707106769f};
    s.directions[2] = {0.44721359f, 0.89442718f};
    s.directions[3] = {-0.44721359f, 0.89442718f};
    s.directions[4] = {-0.707106769f, 0.707106769f};
    s.directions[5] = {-0.89442718f, 0.44721359f};
    s.directions[6] = {-0.89442718f, -0.44721359f};
    s.directions[7] = {-0.707106769f, -0.707106769f};
    s.directions[8] = {-0.44721359f, -0.89442718f};
    s.directions[9] = {0.44721359f, -0.89442718f};
    s.directions[10] = {0.707106769f, -0.707106769f};
    s.directions[11] = {0.89442718f, -0.44721359f};

    fn_710026d6d4();
    fn_710009ec00(&g_unk_71726a39b0, s.field_0x80);
    InitClass(AsUnk(&g_unk_71726a39b8), "navmesh::CFixedDynamicSmartLinkRule::SMask", fn_710026d954, fn_710026d950);
    fn_710009ec00(&g_unk_71726a3af0, &g_unk_71726a39b8);
    fn_710009ec00(&g_unk_71726a3af1, AsUnk(fn_710026dad8()));
    fn_710009ec00(&g_unk_71726a3af2, AsUnk(fn_710026dbb4()));
    InitClass(AsUnk(&g_unk_71726a3af8),
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<navmesh::CFixedDynamicSmartLinkRule::"
              "SMask>>::TKeyElementIterator::value_type",
              fn_710026e014, reinterpret_cast<void (*)()>(&fn_710026dc90), AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71726a3c30, &g_unk_71726a3af8);
    fn_710009ec00(&g_unk_71726a3c31, AsUnk(fn_710026e198()));
    InitClass(AsUnk(&g_unk_71726a3c38),
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<navmesh::CFixedDynamicSmartLinkRule::"
              "SMask>>::TKeyElementConstIterator::value_type",
              fn_710026e5f8, reinterpret_cast<void (*)()>(&fn_710026e274), AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(&g_unk_71726a3d70, &g_unk_71726a3c38);
    fn_710009ec00(&g_unk_71726a3d71, AsUnk(fn_710026e77c()));
    InitVector(AsUnk(&g_unk_71726a3d78), "base::global::CRntVector<navmesh::CFixedDynamicSmartLinkRule::SMask>",
               fn_710026e858);
    fn_710009ec00(&g_unk_71726a3ee8, &g_unk_71726a3d78);
}
