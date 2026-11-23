// CCharClassWaterPoolComponent's unit: its static initialiser. The header data, the component's id, its
// "ChangeIdx" and "WATER_VOLUMES" properties, its classes, CWaterPoolComponent::FloatingEntitiesInfo and the
// vector of them with its iterator value types, then CWaterPoolComponent's registered id. Names and layouts
// are placeholders.
#include <new>

#include "base/UnkAtExit.h"
#include "base/UnkStrIdRegistry.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflStatics.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/CCharClassLiquidPoolComponent.h"
#include "game/CCharClassWaterPoolComponent.h"
#include "game/CEmmyProtoAIComponent.h"
#include "game/UnkTUInit.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTUSymbols.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::CType;

// The unit's statics (the compiler merges them into one block).
struct UnkWaterPoolTU {
    UnkTUHeader header;
    u8 pad_0x18[0x10];
    CStrId water_pool_id;  // 0x28, CWaterPoolComponent's registered id
    f32 field_0x30;
    f32 field_0x34;
    bool field_0x38;
    u8 pad_0x39[7];
    char component_class[0x178 - 0x40];  // 0x40
    char base_class[8];                  // 0x178
};
static_assert(__builtin_offsetof(UnkWaterPoolTU, base_class) == 0x178, "layout");
extern UnkWaterPoolTU g_unk_7172735060 __attribute__((visibility("hidden")));

extern base::global::CStrId g_unk_7172734630;
extern UnkProperty g_unk_7172734638;
extern void* g_unk_7101a81150[];  // the property's vtable
void fn_710090ba1c(void* item);   // base::reflection::refl_Register<..>, instantiated in src/reflection_statics
void fn_7100906580(void* p);
extern char g_unk_71727346a0[];
extern char g_unk_71727346a1[];
// CWaterPoolComponent::FloatingEntitiesInfo.
extern base::reflection::CType g_unk_71727346a8;
void fn_710090732c(CType* self, const CStrId& name, const CType* base, void* a, void (*init)(CClass*), void* b);
void fn_71009071ec(CClass* cls);
extern char g_unk_71727347e0[];
extern char g_unk_71727347e1[];
const CType* fn_71009074b0();
extern char g_unk_71727347e2[];
const CType* fn_710090758c();
// The key iterator's value type.
extern base::reflection::CType g_unk_71727347e8;
void fn_71009079ec(CType* self, const CStrId& name, const CType* base, void* a, void (*init)(CClass*), void* b);
void fn_7100907668(CClass* cls);
extern char g_unk_7172734920[];
extern char g_unk_7172734921[];
const base::reflection::CType* fn_7100907b70();
// The const key iterator's value type.
extern base::reflection::CType g_unk_7172734928;
void fn_7100907fd0(CType* self, const CStrId& name, const CType* base, void* a, void (*init)(CClass*), void* b);
void fn_7100907c4c(CClass* cls);
extern char g_unk_7172734a60[];
extern char g_unk_7172734a61[];
const CType* fn_7100908154();
// base::global::CRntVector<CWaterPoolComponent::FloatingEntitiesInfo>.
extern base::reflection::CType g_unk_7172734a68;
void fn_7100908230(CType* self, const CStrId& name, void* a, void* b);
extern char g_unk_7172734bd8[];

void fn_710090de40() {
    UnkWaterPoolTU& tu = g_unk_7172735060;
    InitHeader(tu.header);
    tu.field_0x30 = -1.0f;
    tu.field_0x34 = -1.0f;
    tu.field_0x38 = true;

    new (&g_unk_7172734630) CStrId("WATERPOOL", true);
    fn_7100000250(CStrId_Destroy, &g_unk_7172734630, &g_unk_7101a19000);
    InitProperty(&g_unk_7172734638, g_unk_7101a81150, g_unk_71726db788, "ChangeIdx", refl_TypeOf<fn_7100014768>(),
                 fn_710090ba1c, fn_7100906580);
    InitPropertyEx(&g_unk_7172734660, g_unk_7101a45fe8, g_unk_71726db788, g_unk_7101a45f90, fn_71002ef5bc,
                   "WATER_VOLUMES", &g_unk_7172733a70, fn_71004fd2cc, fn_71004fa3b8);

    fn_71009065fc();
    fn_710009ec00(g_unk_71727346a0, tu.component_class);
    fn_7100906bcc();
    fn_710009ec00(g_unk_71727346a1, tu.base_class);

    {
        CStrId name("CWaterPoolComponent::FloatingEntitiesInfo", true);
        fn_710090732c(&g_unk_71727346a8, name, nullptr, nullptr, fn_71009071ec, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_71727346a8, &g_unk_7101a19000);
    fn_710009ec00(g_unk_71727347e0, &g_unk_71727346a8);
    fn_710009ec00(g_unk_71727347e1, const_cast<CType*>(fn_71009074b0()));
    fn_710009ec00(g_unk_71727347e2, const_cast<CType*>(fn_710090758c()));

    {
        CStrId name("base::reflection::CollectionTypeMapper<base::global::CRntVector<"
                    "CWaterPoolComponent::FloatingEntitiesInfo>>::TKeyElementIterator::value_type",
                    true);
        fn_71009079ec(&g_unk_71727347e8, name, &g_unk_7101d5e2e0, nullptr, fn_7100907668, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_71727347e8, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7172734920, &g_unk_71727347e8);
    fn_710009ec00(g_unk_7172734921, const_cast<CType*>(fn_7100907b70()));

    {
        CStrId name("base::reflection::CollectionTypeMapper<base::global::CRntVector<"
                    "CWaterPoolComponent::FloatingEntitiesInfo>>::TKeyElementConstIterator::value_type",
                    true);
        fn_7100907fd0(&g_unk_7172734928, name, &g_unk_7101d5e2e0, nullptr, fn_7100907c4c, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_7172734928, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7172734a60, &g_unk_7172734928);
    fn_710009ec00(g_unk_7172734a61, const_cast<CType*>(fn_7100908154()));

    {
        CStrId name("base::global::CRntVector<CWaterPoolComponent::FloatingEntitiesInfo>", true);
        fn_7100908230(&g_unk_7172734a68, name, nullptr, nullptr);
    }
    fn_7100000250(fn_710009891c, &g_unk_7172734a68, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7172734bd8, &g_unk_7172734a68);

    {
        CStrId name("CWaterPoolComponent", true);
        new (&tu.water_pool_id) CStrId(fn_7100de8f78(name));
    }
    fn_7100000250(CStrId_Destroy, &tu.water_pool_id, &g_unk_7101a19000);
}
