// Static initialiser registering 2 enum types: for each, builds the enum info object, registers
// its destructor with atexit, adds it to the registry, then fills two handler records and
// registers both. Enums: base::math::ETransformSpace, base::math::EMaxAxis.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflConverter.h"
#include "game/UnkTU22.h"
#include "game/UnkTUInit.h"
#include "base/UnkEnumInfo.h"
#include "base/reflection/TypeObject.h"

#define HIDDEN __attribute__((visibility("hidden")))

using base::global::CStrId;

// The reflected int and unsigned types (base/reflection/ReflectedAttributes.h), seen through the
// placeholder.
template <const base::reflection::CType* (*Getter)()>
static inline UnkEnumInfo* TypeOf() {
    return reinterpret_cast<UnkEnumInfo*>(const_cast<base::reflection::CType*>(Getter()));
}

namespace base::math { enum class ETransformSpace : u32; }
template <> UnkEnumInfo base::reflection::TypeObject<base::math::ETransformSpace, UnkEnumInfo>::object asm("g_unk_7101d58900");
namespace base::math { enum class EMaxAxis : u32; }
template <> UnkEnumInfo base::reflection::TypeObject<base::math::EMaxAxis, UnkEnumInfo>::object asm("g_unk_7101d58a60");
extern UnkConverter g_unk_7101d58a20;
extern UnkConverter g_unk_7101d58a40;
extern UnkConverter g_unk_7101d58b80;
extern UnkConverter g_unk_7101d58ba0;
extern u8 g_unk_7101d58a18[];
extern u8 g_unk_7101d58b78[];
void fn_710007a91c();
void fn_710007aac8(UnkEnumInfo* obj, CStrId* name, void (*fn)(), void* a);
void fn_710007ac48();
void fn_710007af74(UnkEnumInfo* obj, CStrId* name, void (*fn)(), void* a);
void fn_710007b0f4() HIDDEN;
void fn_710007b100() HIDDEN;
void fn_710007b10c() HIDDEN;
void fn_710007b118() HIDDEN;
void ETransformSpaceAndEMaxAxis_InitStatics() {
    {
        CStrId name("base::math::ETransformSpace", true);
        fn_710007aac8(&base::reflection::TypeObject<base::math::ETransformSpace, UnkEnumInfo>::object, &name, fn_710007a91c, nullptr);
    }
    fn_7100000250(CEnumType_Destroy, &base::reflection::TypeObject<base::math::ETransformSpace, UnkEnumInfo>::object, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d58a18, &base::reflection::TypeObject<base::math::ETransformSpace, UnkEnumInfo>::object);
    {
        void* a = base::reflection::TypeObject<base::math::ETransformSpace, UnkEnumInfo>::object.desc_0x8->field_0x30;
        void* b = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
        g_unk_7101d58a20.from = a;
        g_unk_7101d58a20.to = b;
        g_unk_7101d58a20.convert = fn_710007b0f4;
        g_unk_7101d58a20.next = nullptr;
        fn_71000a16c4(&g_unk_7101d58a20);
        void* c = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
        void* d = base::reflection::TypeObject<base::math::ETransformSpace, UnkEnumInfo>::object.desc_0x8->field_0x30;
        g_unk_7101d58a40.from = c;
        g_unk_7101d58a40.to = d;
        g_unk_7101d58a40.convert = fn_710007b100;
        g_unk_7101d58a40.next = nullptr;
        fn_71000a16c4(&g_unk_7101d58a40);
    }
    {
        CStrId name("base::math::EMaxAxis", true);
        fn_710007af74(&base::reflection::TypeObject<base::math::EMaxAxis, UnkEnumInfo>::object, &name, fn_710007ac48, nullptr);
    }
    fn_7100000250(CEnumType_Destroy, &base::reflection::TypeObject<base::math::EMaxAxis, UnkEnumInfo>::object, &g_unk_7101a19000);
    fn_710009ec00(g_unk_7101d58b78, &base::reflection::TypeObject<base::math::EMaxAxis, UnkEnumInfo>::object);
    {
        void* a = base::reflection::TypeObject<base::math::EMaxAxis, UnkEnumInfo>::object.desc_0x8->field_0x30;
        void* b = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
        g_unk_7101d58b80.from = a;
        g_unk_7101d58b80.to = b;
        g_unk_7101d58b80.convert = fn_710007b10c;
        g_unk_7101d58b80.next = nullptr;
        fn_71000a16c4(&g_unk_7101d58b80);
        void* c = TypeOf<fn_7100014768>()->desc_0x8->field_0x30;
        void* d = base::reflection::TypeObject<base::math::EMaxAxis, UnkEnumInfo>::object.desc_0x8->field_0x30;
        g_unk_7101d58ba0.from = c;
        g_unk_7101d58ba0.to = d;
        g_unk_7101d58ba0.convert = fn_710007b118;
        g_unk_7101d58ba0.next = nullptr;
        fn_71000a16c4(&g_unk_7101d58ba0);
    }
}
