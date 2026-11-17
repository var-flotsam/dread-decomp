// base::reflection's unit (tools/tu_map.py, unit 2613): the unit's static initialiser. It builds the header
// data, then registers CSubareaSetup, the two iterator value types of CRntVector<std::unique_ptr<CSubareaSetup>>,
// the vector type itself and a converter between unique_ptr<CSubareaSetup> and CSubareaSetup*.
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

extern UnkTUHeader g_unk_71729efa18 __attribute__((visibility("hidden")));

// CSubareaSetup.
extern base::reflection::CType g_unk_71729eee60;
void fn_7100fb3274(CType* self, const CStrId& name, const CType* base, void* a, void (*init)(CClass*), void* b);
void fn_7100fb2988(CClass* cls);
extern char g_unk_71729eef98[];
const base::reflection::CType* fn_7100ef0764();  // base::reflection::refl_GetType<CSubareaSetup*>
extern char g_unk_71729eef99[];
const base::reflection::CType* fn_7100fb390c();
extern char g_unk_71729eef9a[];
// The iterator types of CRntVector<std::unique_ptr<CSubareaSetup>>.
const base::reflection::CType* fn_7100fb3b10();
extern char g_unk_71729eef9b[];
const base::reflection::CType* fn_7100fb3bec();
extern char g_unk_71729eef9c[];
// The key iterator's value type.
extern base::reflection::CType g_unk_71729eefa0;
void fn_7100fb404c(CType* self, const CStrId& name, const CType* base, void* a, void (*init)(CClass*), void* b);
void fn_7100fb3cc8(CClass* cls);
extern char g_unk_71729ef0d8[];
const base::reflection::CType* fn_7100fb41d0();
extern char g_unk_71729ef0d9[];
// The const key iterator's value type.
extern base::reflection::CType g_unk_71729ef0e0;
void fn_7100fb4630(CType* self, const CStrId& name, const CType* base, void* a, void (*init)(CClass*), void* b);
void fn_7100fb42ac(CClass* cls);
extern char g_unk_71729ef218[];
const base::reflection::CType* fn_7100fb47b4();
extern char g_unk_71729ef219[];
// CRntVector<std::unique_ptr<CSubareaSetup>>.
extern base::reflection::CType g_unk_71729ef220;
void fn_7100fb4890(CType* self, const CStrId& name, void* a, void* b);
extern char g_unk_71729ef390[];
const base::reflection::CType* fn_7100fb4a70();
extern char g_unk_71729ef391[];
const base::reflection::CType* fn_7100ee0f38();
extern char g_unk_71729ef392[];
// The converter from unique_ptr<CSubareaSetup> to CSubareaSetup*.
extern UnkConverter g_unk_71729ef398;
void fn_7100fb64c8() __attribute__((visibility("hidden")));

void fn_7100fb7480() {
    InitHeader(g_unk_71729efa18);

    {
        CStrId name("CSubareaSetup", true);
        fn_7100fb3274(&g_unk_71729eee60, name, nullptr, nullptr, fn_7100fb2988, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_71729eee60, &g_unk_7101a19000);
    fn_710009ec00(g_unk_71729eef98, &g_unk_71729eee60);
    fn_710009ec00(g_unk_71729eef99, const_cast<CType*>(fn_7100ef0764()));
    fn_710009ec00(g_unk_71729eef9a, const_cast<CType*>(fn_7100fb390c()));
    fn_710009ec00(g_unk_71729eef9b, const_cast<CType*>(fn_7100fb3b10()));
    fn_710009ec00(g_unk_71729eef9c, const_cast<CType*>(fn_7100fb3bec()));

    {
        CStrId name("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CSubareaSetup>>>::"
                    "TKeyElementIterator::value_type",
                    true);
        fn_7100fb404c(&g_unk_71729eefa0, name, &g_unk_7101d5e2e0, nullptr, fn_7100fb3cc8, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_71729eefa0, &g_unk_7101a19000);
    fn_710009ec00(g_unk_71729ef0d8, &g_unk_71729eefa0);
    fn_710009ec00(g_unk_71729ef0d9, const_cast<CType*>(fn_7100fb41d0()));

    {
        CStrId name("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<CSubareaSetup>>>::"
                    "TKeyElementConstIterator::value_type",
                    true);
        fn_7100fb4630(&g_unk_71729ef0e0, name, &g_unk_7101d5e2e0, nullptr, fn_7100fb42ac, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_71729ef0e0, &g_unk_7101a19000);
    fn_710009ec00(g_unk_71729ef218, &g_unk_71729ef0e0);
    fn_710009ec00(g_unk_71729ef219, const_cast<CType*>(fn_7100fb47b4()));

    {
        CStrId name("base::global::CRntVector<std::unique_ptr<CSubareaSetup>>", true);
        fn_7100fb4890(&g_unk_71729ef220, name, nullptr, nullptr);
    }
    fn_7100000250(fn_710009891c, &g_unk_71729ef220, &g_unk_7101a19000);
    fn_710009ec00(g_unk_71729ef390, &g_unk_71729ef220);
    fn_710009ec00(g_unk_71729ef391, const_cast<CType*>(fn_7100fb4a70()));
    fn_710009ec00(g_unk_71729ef392, const_cast<CType*>(fn_7100ee0f38()));

    InitConverter(g_unk_71729ef398, refl_TypeOf<fn_7100fb390c>, refl_TypeOf<fn_7100ef0764>, fn_7100fb64c8);
}
