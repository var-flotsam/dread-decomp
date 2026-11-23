// CLogicShapeComponent's unit (tools/tu_map.py unit 1402): the unit's static initialiser. The header data,
// the component's class, the reflected types of CRntSmallDictionary<CStrId, bool> (its key-element value
// types and the dictionary), the "pLogicShape" value and the component's registry id.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkAtExit.h"
#include "base/UnkStrIdRegistry.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::CType;

// The unit's statics (the compiler merges them into one block).
struct UnkLogicShapeTU {
    UnkTUHeader header;
    u8 pad_0x18[0x8];
    CStrId component_id;      // 0x20
    char component_type[8];   // 0x28
};
extern UnkLogicShapeTU g_unk_7172736740 __attribute__((visibility("hidden")));

extern char g_unk_7172735cd0[];
extern char g_unk_7172735cd1[];
extern char g_unk_7172735cd2[];
extern char g_unk_7172735e10[];
extern char g_unk_7172735e11[];
extern char g_unk_7172735f50[];
extern char g_unk_7172735f51[];
extern char g_unk_71727360c8[];

// The dictionary's iterator types (refl_GetType<...TElementIterator> and its const one), and its
// key-element value types' and its own getters.
const CType* fn_71009132bc();
const CType* fn_7100913398();
const CType* fn_710091397c();
const CType* fn_7100913f60();

// The key-element value types (KeyElementValue<...> and ConstKeyElementValue<...>): built from a name with a
// base type and an init function.
extern CType g_unk_7172735cd8;
extern CType g_unk_7172735e18;
void fn_7100913474(CClass* cls);
void fn_7100913a58(CClass* cls);
void fn_71009137f8(CType* type, const CStrId& name, const CType* base, void* a, void (*init)(CClass*), void* b);
void fn_7100913ddc(CType* type, const CStrId& name, const CType* base, void* a, void (*init)(CClass*), void* b);

// The dictionary type (refl_InitType<CRntSmallDictionary<CStrId, bool>>).
struct UnkEnumType;
extern UnkEnumType g_unk_7172735f58;
void fn_710091403c(UnkReflType* self, const CStrId& name, void* a, void* b);

// The "pLogicShape" value (a string id).
extern UnkValue g_unk_71727360d0;

// The component's name: its guard and object are reached through the GOT.
inline const CStrId& LogicShapeId() {
    static CStrId s_id("LOGICSHAPE", true);
    return s_id;
}

static inline void AddType(char* list, const CType* type) {
    fn_710009ec00(list, const_cast<CType*>(type));
}

void fn_7100916f30() {
    UnkLogicShapeTU& tu = g_unk_7172736740;
    InitHeader(tu.header);
    fn_7100912a38();
    fn_710009ec00(g_unk_7172735cd0, tu.component_type);
    AddType(g_unk_7172735cd1, fn_71009132bc());
    AddType(g_unk_7172735cd2, fn_7100913398());
    {
        CStrId name("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, "
                    "bool>>::TKeyElementIterator::value_type",
                    true);
        fn_71009137f8(&g_unk_7172735cd8, name, &g_unk_7101d5e2e0, nullptr, fn_7100913474, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_7172735cd8, &g_unk_7101a19000);
    AddType(g_unk_7172735e10, &g_unk_7172735cd8);
    AddType(g_unk_7172735e11, fn_710091397c());
    {
        CStrId name("base::reflection::CollectionTypeMapper<base::global::CRntSmallDictionary<base::global::CStrId, "
                    "bool>>::TKeyElementConstIterator::value_type",
                    true);
        fn_7100913ddc(&g_unk_7172735e18, name, &g_unk_7101d5e2e0, nullptr, fn_7100913a58, nullptr);
    }
    fn_7100000250(fn_7100096604, &g_unk_7172735e18, &g_unk_7101a19000);
    AddType(g_unk_7172735f50, &g_unk_7172735e18);
    AddType(g_unk_7172735f51, fn_7100913f60());
    InitVector(reinterpret_cast<UnkReflType*>(&g_unk_7172735f58),
               "base::global::CRntSmallDictionary<base::global::CStrId, bool>", fn_710091403c);
    fn_710009ec00(g_unk_71727360c8, &g_unk_7172735f58);
    new (&g_unk_71727360d0) UnkValue(CStrId("pLogicShape", true), refl_TypeOf<fn_7100009658>);
    fn_7100000250(fn_710010c424, &g_unk_71727360d0, &g_unk_7101a19000);
    new (&tu.component_id) CStrId(fn_7100de8f78(LogicShapeId()));
    fn_7100000250(CStrId_Destroy, &tu.component_id, &g_unk_7101a19000);
}
