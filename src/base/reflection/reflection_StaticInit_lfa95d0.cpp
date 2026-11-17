// base::reflection's unit (tools/tu_map.py, unit 2610): the unit's static initialiser. It builds the header
// data, then registers CSubareaInfo, the two iterator value types of CRntVector<std::unique_ptr<CSubareaInfo>>,
// the vector type itself and a converter between unique_ptr<CSubareaInfo> and CSubareaInfo*.
// Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflConverter.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::CType;

extern UnkTUHeader g_unk_71729ee000 __attribute__((visibility("hidden")));

// CSubareaInfo.
extern base::reflection::CType g_unk_71729ed338;
void fn_7100fa26a0(CType* self, const CStrId& name, const CType* base, void* a, void (*init)(CClass*), void* b);
void fn_7100fa1254(CClass* cls);
extern char g_unk_71729ed470[];
const base::reflection::CType* fn_7100fa3288();  // base::reflection::refl_GetType<CSubareaInfo*>
extern char g_unk_71729ed471[];
const base::reflection::CType* fn_7100fa3364();  // base::reflection::refl_GetType<std::unique_ptr<CSubareaInfo>>
extern char g_unk_71729ed472[];
// The iterator types of CRntVector<std::unique_ptr<CSubareaInfo>>.
const base::reflection::CType* fn_7100fa3568();
extern char g_unk_71729ed473[];
const base::reflection::CType* fn_7100fa3644();
extern char g_unk_71729ed474[];
// The key iterator's value type.
extern base::reflection::CType g_unk_71729ed478;
void fn_7100fa3aa4(CType* self, const CStrId& name, const CType* base, void* a, void (*init)(CClass*), void* b);
void fn_7100fa3720(CClass* cls);
extern char g_unk_71729ed5b0[];
const base::reflection::CType* fn_7100fa3c28();
extern char g_unk_71729ed5b1[];
// The const key iterator's value type.
extern base::reflection::CType g_unk_71729ed5b8;
void fn_7100fa4088(CType* self, const CStrId& name, const CType* base, void* a, void (*init)(CClass*), void* b);
void fn_7100fa3d04(CClass* cls);
extern char g_unk_71729ed6f0[];
const base::reflection::CType* fn_7100fa420c();
extern char g_unk_71729ed6f1[];
// CRntVector<std::unique_ptr<CSubareaInfo>>.
extern base::reflection::CType g_unk_71729ed6f8;
void fn_7100fa42e8(CType* self, const CStrId& name, void* a, void* b);
extern char g_unk_71729ed868[];
const base::reflection::CType* fn_7100fa44c8();
extern char g_unk_71729ed869[];
// The converter from unique_ptr<CSubareaInfo> to CSubareaInfo*.
extern UnkConverter g_unk_71729ed870;
void fn_7100fa809c() __attribute__((visibility("hidden")));

void fn_7100fa95d0() {
    InitHeader(g_unk_71729ee000);

    {
        CStrId name("CSubareaInfo", true);
        fn_7100fa26a0(&g_unk_71729ed338, name, nullptr, nullptr, fn_7100fa1254, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_71729ed338, &g_unk_7101a19000);
    fn_710009ec00(g_unk_71729ed470, &g_unk_71729ed338);
    fn_710009ec00(g_unk_71729ed471, const_cast<CType*>(fn_7100fa3288()));
    fn_710009ec00(g_unk_71729ed472, const_cast<CType*>(fn_7100fa3364()));
    fn_710009ec00(g_unk_71729ed473, const_cast<CType*>(fn_7100fa3568()));
    fn_710009ec00(g_unk_71729ed474, const_cast<CType*>(fn_7100fa3644()));

    {
        CStrId name("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CSubareaInfo>>>::"
                    "TKeyElementIterator::value_type",
                    true);
        fn_7100fa3aa4(&g_unk_71729ed478, name, &g_unk_7101d5e2e0, nullptr, fn_7100fa3720, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_71729ed478, &g_unk_7101a19000);
    fn_710009ec00(g_unk_71729ed5b0, &g_unk_71729ed478);
    fn_710009ec00(g_unk_71729ed5b1, const_cast<CType*>(fn_7100fa3c28()));

    {
        CStrId name("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CSubareaInfo>>>::"
                    "TKeyElementConstIterator::value_type",
                    true);
        fn_7100fa4088(&g_unk_71729ed5b8, name, &g_unk_7101d5e2e0, nullptr, fn_7100fa3d04, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_71729ed5b8, &g_unk_7101a19000);
    fn_710009ec00(g_unk_71729ed6f0, &g_unk_71729ed5b8);
    fn_710009ec00(g_unk_71729ed6f1, const_cast<CType*>(fn_7100fa420c()));

    {
        CStrId name("base::global::CRntVector<std::unique_ptr<CSubareaInfo>>", true);
        fn_7100fa42e8(&g_unk_71729ed6f8, name, nullptr, nullptr);
    }
    fn_7100000250(fn_710009891c, &g_unk_71729ed6f8, &g_unk_7101a19000);
    fn_710009ec00(g_unk_71729ed868, &g_unk_71729ed6f8);
    fn_710009ec00(g_unk_71729ed869, const_cast<CType*>(fn_7100fa44c8()));

    InitConverter(g_unk_71729ed870, refl_TypeOf<fn_7100fa3364>, refl_TypeOf<fn_7100fa3288>, fn_7100fa809c);
}
