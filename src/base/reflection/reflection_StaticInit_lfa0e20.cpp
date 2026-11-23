// base::reflection's unit (tools/tu_map.py, unit 2609): the unit's static initialiser. It builds the header
// data, then registers CSubareaCharclassGroup, the iterator value types of
// CRntVector<std::unique_ptr<CSubareaCharclassGroup>>, the vector type itself, a converter between
// unique_ptr<CSubareaCharclassGroup> and CSubareaCharclassGroup*, and the "No Enemies" string id.
// Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflConverter.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTU2577.h"
#include "game/UnkTUInit.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::CType;

extern UnkTUHeader g_unk_71729ed320 __attribute__((visibility("hidden")));

// CSubareaCharclassGroup.
extern base::reflection::CType g_unk_71729ec6e0;
void fn_7100f9d2c4(CType* self, const CStrId& name, const CType* base, void* a, void (*init)(CClass*), void* b);
void fn_7100f9cde8(CClass* cls);
extern char g_unk_71729ec818[];
extern char g_unk_71729ec819[];
const base::reflection::CType* fn_7100f9d5bc();  // base::reflection::refl_GetType<std::unique_ptr<CSubareaCharclassGroup>>
extern char g_unk_71729ec81a[];
// The iterator types of CRntVector<std::unique_ptr<CSubareaCharclassGroup>>.
const base::reflection::CType* fn_7100f9d7c0();
extern char g_unk_71729ec81b[];
const base::reflection::CType* fn_7100f9d89c();
extern char g_unk_71729ec81c[];
// The key iterator's value type.
extern base::reflection::CType g_unk_71729ec820;
void fn_7100f9dcfc(CType* self, const CStrId& name, const CType* base, void* a, void (*init)(CClass*), void* b);
void fn_7100f9d978(CClass* cls);
extern char g_unk_71729ec958[];
const base::reflection::CType* fn_7100f9de80();
extern char g_unk_71729ec959[];
// The const key iterator's value type.
extern base::reflection::CType g_unk_71729ec960;
void fn_7100f9e2e0(CType* self, const CStrId& name, const CType* base, void* a, void (*init)(CClass*), void* b);
void fn_7100f9df5c(CClass* cls);
extern char g_unk_71729eca98[];
const base::reflection::CType* fn_7100f9e464();
extern char g_unk_71729eca99[];
// CRntVector<std::unique_ptr<CSubareaCharclassGroup>>.
extern base::reflection::CType g_unk_71729ecaa0;
void fn_7100f9e540(CType* self, const CStrId& name, void* a, void* b);
extern char g_unk_71729ecc10[];
const base::reflection::CType* fn_7100f9e720();
extern char g_unk_71729ecc11[];
const base::reflection::CType* fn_7100ee1014();
extern char g_unk_71729ecc12[];
// The converter from unique_ptr<CSubareaCharclassGroup> to CSubareaCharclassGroup*.
extern UnkConverter g_unk_71729ecc18;
void fn_7100f9fc9c() __attribute__((visibility("hidden")));
// The "No Enemies" string id.
extern CStrId g_unk_71729ecc38;

void fn_7100fa0e20() {
    InitHeader(g_unk_71729ed320);

    {
        CStrId name("CSubareaCharclassGroup", true);
        fn_7100f9d2c4(&g_unk_71729ec6e0, name, nullptr, nullptr, fn_7100f9cde8, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_71729ec6e0, &g_unk_7101a19000);
    fn_710009ec00(g_unk_71729ec818, &g_unk_71729ec6e0);
    fn_710009ec00(g_unk_71729ec819, const_cast<CType*>(fn_7100ef0e00()));
    fn_710009ec00(g_unk_71729ec81a, const_cast<CType*>(fn_7100f9d5bc()));
    fn_710009ec00(g_unk_71729ec81b, const_cast<CType*>(fn_7100f9d7c0()));
    fn_710009ec00(g_unk_71729ec81c, const_cast<CType*>(fn_7100f9d89c()));

    {
        CStrId name("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<"
                    "CSubareaCharclassGroup>>>::TKeyElementIterator::value_type",
                    true);
        fn_7100f9dcfc(&g_unk_71729ec820, name, &g_unk_7101d5e2e0, nullptr, fn_7100f9d978, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_71729ec820, &g_unk_7101a19000);
    fn_710009ec00(g_unk_71729ec958, &g_unk_71729ec820);
    fn_710009ec00(g_unk_71729ec959, const_cast<CType*>(fn_7100f9de80()));

    {
        CStrId name("base::reflection::CollectionTypeMapper<base::global::CRntVector<std::unique_ptr<"
                    "CSubareaCharclassGroup>>>::TKeyElementConstIterator::value_type",
                    true);
        fn_7100f9e2e0(&g_unk_71729ec960, name, &g_unk_7101d5e2e0, nullptr, fn_7100f9df5c, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_71729ec960, &g_unk_7101a19000);
    fn_710009ec00(g_unk_71729eca98, &g_unk_71729ec960);
    fn_710009ec00(g_unk_71729eca99, const_cast<CType*>(fn_7100f9e464()));

    {
        CStrId name("base::global::CRntVector<std::unique_ptr<CSubareaCharclassGroup>>", true);
        fn_7100f9e540(&g_unk_71729ecaa0, name, nullptr, nullptr);
    }
    fn_7100000250(fn_710009891c, &g_unk_71729ecaa0, &g_unk_7101a19000);
    fn_710009ec00(g_unk_71729ecc10, &g_unk_71729ecaa0);
    fn_710009ec00(g_unk_71729ecc11, const_cast<CType*>(fn_7100f9e720()));
    fn_710009ec00(g_unk_71729ecc12, const_cast<CType*>(fn_7100ee1014()));

    InitConverter(g_unk_71729ecc18, refl_TypeOf<fn_7100f9d5bc>, refl_TypeOf<fn_7100ef0e00>, fn_7100f9fc9c);

    new (&g_unk_71729ecc38) CStrId("No Enemies", true);
    fn_7100000250(CStrId_Destroy, &g_unk_71729ecc38, &g_unk_7101a19000);
}
