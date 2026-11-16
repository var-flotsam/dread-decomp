// CCharClassLiquidPoolComponent's unit (tools/tu_map.py unit 1394): the unit's static initialiser. The header
// data, the component's and CLiquidPoolBaseComponent's classes, the iterator value types of
// CRntSmallDictionary<CStrId, CAABox2D> and the dictionary type, the enum CLiquidPoolBaseComponent::ELiquidType
// with its int converters, then the splash effect paths. Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflConverter.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"
#include "types.h"

#include <new>

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::CType;

// The unit's statics (the compiler merges them into one block).
struct UnkLiquidPoolTU {
    UnkTUHeader header;
    u8 pad_0x18[0x10];
    CFilePathStrId effects[10];  // 0x28
    u8 pad_0x78[0x20];
    f32 field_0x98;
    f32 field_0x9c;
    bool field_0xa0;
    u8 pad_0xa1[7];
    char component_class[0x1e0 - 0xa8];  // 0xa8
    char base_class[8];                  // 0x1e0
};
static_assert(__builtin_offsetof(UnkLiquidPoolTU, base_class) == 0x1e0, "layout");
extern UnkLiquidPoolTU g_unk_7172734188 __attribute__((visibility("hidden")));

extern char g_unk_71727337e8[];
extern char g_unk_71727337e9[];
// The iterator types of CRntSmallDictionary<CStrId, CAABox2D>.
const CType* fn_71008fedb8();
extern char g_unk_71727337ea[];
const CType* fn_71008fee94();
extern char g_unk_71727337eb[];
// The key iterator's value type.
extern base::reflection::CType g_unk_71727337f0;
void fn_71008ff2f4(CType* self, const CStrId& name, const CType* base, void* a, void (*init)(CClass*), void* b);
void fn_71008fef70(CClass* cls);
extern char g_unk_7172733928[];
const CType* fn_71008ff478();
extern char g_unk_7172733929[];
// The const key iterator's value type.
extern base::reflection::CType g_unk_7172733930;
void fn_71008ff8d8(CType* self, const CStrId& name, const CType* base, void* a, void (*init)(CClass*), void* b);
void fn_71008ff554(CClass* cls);
extern char g_unk_7172733a68[];
const CType* fn_71008ffa5c();
extern char g_unk_7172733a69[];
// CRntSmallDictionary<CStrId, CAABox2D>.
extern base::reflection::CType g_unk_7172733a70;
void fn_71008ffb38(CType* self, const CStrId& name, void* a, void* b);
extern char g_unk_7172733be0[];
// CLiquidPoolBaseComponent::ELiquidType and its converters to and from int.
extern base::reflection::CType g_unk_7172733be8;
void fn_71008fff3c(CType* self, const CStrId& name, void (*init)(void*), void* extra);
void fn_71008ffd30(void* e);  // src/clones/clones_03.cpp
extern char g_unk_7172733d00[];
extern UnkConverter g_unk_7172733d08;
extern UnkConverter g_unk_7172733d28;
void fn_7100903848() __attribute__((visibility("hidden")));
void fn_7100903854() __attribute__((visibility("hidden")));

static inline void InitEffect(CFilePathStrId* id, const char* path) {
    new (id) CFilePathStrId(path, true);
    fn_7100000250(fn_7100000c7c, id, &g_unk_7101a19000);
}

void fn_7100905300() {
    UnkLiquidPoolTU& tu = g_unk_7172734188;
    InitHeader(tu.header);
    tu.field_0x98 = -1.0f;
    tu.field_0x9c = -1.0f;
    tu.field_0xa0 = true;
    fn_71008fdfbc();
    fn_710009ec00(g_unk_71727337e8, tu.component_class);
    fn_71008fe62c();
    fn_710009ec00(g_unk_71727337e9, tu.base_class);
    fn_710009ec00(g_unk_71727337ea, const_cast<CType*>(fn_71008fedb8()));
    fn_710009ec00(g_unk_71727337eb, const_cast<CType*>(fn_71008fee94()));

    {
        CStrId name("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, "
                    "base::spatial::CAABox2D>>::TKeyElementIterator::value_type",
                    true);
        fn_71008ff2f4(&g_unk_71727337f0, name, &g_unk_7101d5e2e0, nullptr, fn_71008fef70, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_71727337f0, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7172733928, &g_unk_71727337f0);
    fn_710009ec00(g_unk_7172733929, const_cast<CType*>(fn_71008ff478()));

    {
        CStrId name("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, "
                    "base::spatial::CAABox2D>>::TKeyElementConstIterator::value_type",
                    true);
        fn_71008ff8d8(&g_unk_7172733930, name, &g_unk_7101d5e2e0, nullptr, fn_71008ff554, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_7172733930, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7172733a68, &g_unk_7172733930);
    fn_710009ec00(g_unk_7172733a69, const_cast<CType*>(fn_71008ffa5c()));

    {
        CStrId name("base::global::CRntSmallDictionary<base::global::CStrId, base::spatial::CAABox2D>", true);
        fn_71008ffb38(&g_unk_7172733a70, name, nullptr, nullptr);
    }
    fn_7100000250(fn_710009891c, &g_unk_7172733a70, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7172733be0, &g_unk_7172733a70);

    {
        CStrId name("CLiquidPoolBaseComponent::ELiquidType", true);
        fn_71008fff3c(&g_unk_7172733be8, name, fn_71008ffd30, nullptr);
    }
    fn_7100000250(fn_7100099468, &g_unk_7172733be8, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7172733d00, &g_unk_7172733be8);
    InitEnumConverters(reinterpret_cast<UnkReflType*>(&g_unk_7172733be8), g_unk_7172733d08, fn_7100903848,
                       g_unk_7172733d28, fn_7100903854);

    InitEffect(&tu.effects[0], "actors/characters/samus/fx/watersplash.bcptl");
    InitEffect(&tu.effects[1], "actors/characters/samus/fx/samus_splashenter.bcmdl");
    InitEffect(&tu.effects[2], "system/fx/hits/impactbeam_water.bcptl");
    InitEffect(&tu.effects[3], "actors/characters/samus/fx/waterexit.bcptl");
    InitEffect(&tu.effects[4], "generic/gale.bnvib");
    InitEffect(&tu.effects[5], "actors/characters/samus/fx/lavasplash.bcptl");
    InitEffect(&tu.effects[6], "actors/characters/samus/fx/splash_lavaenter.bcmdl");
    InitEffect(&tu.effects[7], "system/fx/hits/impactbeam_lava.bcptl");
    InitEffect(&tu.effects[8], "actors/characters/samus/fx/lavaexit.bcptl");
    InitEffect(&tu.effects[9], "actors/characters/samus/fx/splash_lavaexit.bcmdl");
}
