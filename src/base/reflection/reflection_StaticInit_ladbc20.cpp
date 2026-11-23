// From unit 1634 of data/tu_map.csv (base::reflection): the unit's static initialiser. It registers the
// persistence types: TPersistenceTargetFlagset, gameeditor::CGameModelRoot, gameeditor::EPersistenceTarget
// and its vector, and the converters between them. Names and layouts are placeholders.
#include "base/reflection.h"
#include "base/reflection/CClass.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflConverter.h"
#include "base/reflection/UnkReflTypeDtors.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTUInit.h"
#include "types.h"

// The reflected types are the generated sources' CType; the registration helpers see a type through
// the placeholder layout (UnkReflType).
static inline UnkReflType* AsUnk(const base::reflection::CType* t) {
    return reinterpret_cast<UnkReflType*>(const_cast<base::reflection::CType*>(t));
}

// The unit's data: the header, two values set to -1 and a flag.
struct UnkTU1634Data {
    UnkTUHeader header;
    f32 field_0x18[2];
    bool field_0x20;
};
__attribute__((visibility("hidden"))) extern UnkTU1634Data g_unk_7172758230;

// A function record: a word and the function.
struct UnkTU1634FnRecord {
    u64 field_0x0;
    void (*fn)();
};
extern UnkTU1634FnRecord g_unk_7172757538;
void fn_7100adbbf4();

// TPersistenceTargetFlagset
extern const base::reflection::CType g_unk_7172756d10;
void fn_7100ad5210(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(),
                   void* b);  // TPersistenceTargetFlagset::refl_InitClass
void fn_7100ad520c();
extern char g_unk_7172756e48[];
extern char g_unk_7172756e49[];
UnkReflType* fn_7100ad5394();  // base::reflection::refl_GetType<gameeditor::CGameModelRoot*>

// gameeditor::CGameModelRoot (g_unk_7172756e50)
void fn_7100ad5fa4(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(),
                   void* b);  // gameeditor::CGameModelRoot::refl_InitClass
void fn_7100ad5470();
extern char g_unk_7172756f88[];

// gameeditor::EPersistenceTarget
void fn_7100ad6bcc(UnkReflType* self, const base::global::CStrId& name, void (*init)(),
                   void* extra);  // base::reflection::refl_InitType<gameeditor::EPersistenceTarget>
void fn_7100ad6720();
extern char g_unk_71727570a8[];
extern UnkConverter g_unk_71727570b0;
extern UnkConverter g_unk_71727570d0;
__attribute__((visibility("hidden"))) void fn_7100ad9ce8();
__attribute__((visibility("hidden"))) void fn_7100ad9cf4();
extern char g_unk_71727570f0[];
extern char g_unk_71727570f1[];
UnkReflType* fn_7100ad6d4c();  // its vector's TElementIterator type
UnkReflType* fn_7100ad6e28();  // its vector's TElementConstIterator type

// The vector's key element iterators' value types
extern base::reflection::CType g_unk_71727570f8;
void fn_7100ad7288(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(),
                   void* b);
void fn_7100ad6f04();
extern char g_unk_7172757230[];
extern char g_unk_7172757231[];
UnkReflType* fn_7100ad740c();  // its vector's TKeyElementIterator type
extern base::reflection::CType g_unk_7172757238;
void fn_7100ad786c(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(),
                   void* b);
void fn_7100ad74e8();
extern char g_unk_7172757370[];
extern char g_unk_7172757371[];
UnkReflType* fn_7100ad79f0();  // its vector's TKeyElementConstIterator type

// base::global::CRntVector<gameeditor::EPersistenceTarget>
extern base::reflection::CType g_unk_7172757378;
void fn_7100ad7acc(UnkReflType* self, const base::global::CStrId& name, void* a,
                   void* b);  // base::reflection::refl_InitType<base::global::CRntVector<gameeditor::EPersistenceTarget>>
extern char g_unk_71727574e8[];

// The flag set's converters: from the enum and from an unsigned.
extern UnkConverter g_unk_71727574f0;
extern UnkConverter g_unk_7172757510;
void fn_7100ad51e0();
void fn_7100ad5200();

void fn_7100adbc20() {
    UnkTU1634Data& tu = g_unk_7172758230;
    InitHeader(tu.header);
    tu.field_0x18[0] = -1.0f;
    tu.field_0x18[1] = -1.0f;
    tu.field_0x20 = true;

    UnkReflType* flagset = AsUnk(&g_unk_7172756d10);
    InitClass(flagset, "TPersistenceTargetFlagset", fn_7100ad5210, fn_7100ad520c);
    fn_710009ec00(g_unk_7172756e48, flagset);
    fn_710009ec00(g_unk_7172756e49, fn_7100ad5394());

    InitClass(AsUnk(&g_unk_7172756e50), "gameeditor::CGameModelRoot", fn_7100ad5fa4, fn_7100ad5470);
    fn_710009ec00(g_unk_7172756f88, &g_unk_7172756e50);

    UnkReflType* target = &base::reflection::TypeObject<gameeditor::EPersistenceTarget, UnkReflType>::object;
    InitEnum(target, "gameeditor::EPersistenceTarget", fn_7100ad6bcc, fn_7100ad6720);
    fn_710009ec00(g_unk_71727570a8, target);
    InitEnumConverters(target, g_unk_71727570b0, fn_7100ad9ce8, g_unk_71727570d0, fn_7100ad9cf4);
    fn_710009ec00(g_unk_71727570f0, fn_7100ad6d4c());
    fn_710009ec00(g_unk_71727570f1, fn_7100ad6e28());

    InitClass(AsUnk(&g_unk_71727570f8),
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<gameeditor::EPersistenceTarget>>::"
              "TKeyElementIterator::value_type",
              fn_7100ad7288, fn_7100ad6f04, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(g_unk_7172757230, &g_unk_71727570f8);
    fn_710009ec00(g_unk_7172757231, fn_7100ad740c());

    InitClass(AsUnk(&g_unk_7172757238),
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<gameeditor::EPersistenceTarget>>::"
              "TKeyElementConstIterator::value_type",
              fn_7100ad786c, fn_7100ad74e8, AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(g_unk_7172757370, &g_unk_7172757238);
    fn_710009ec00(g_unk_7172757371, fn_7100ad79f0());

    InitVector(AsUnk(&g_unk_7172757378), "base::global::CRntVector<gameeditor::EPersistenceTarget>", fn_7100ad7acc);
    fn_710009ec00(g_unk_71727574e8, &g_unk_7172757378);

    g_unk_71727574f0 = {target->info->field_0x30, flagset->info->field_0x30, fn_7100ad51e0, nullptr};
    fn_71000a16c4(&g_unk_71727574f0);
    InitConverterFrom(g_unk_7172757510, flagset, refl_TypeOf<fn_7100005178>, fn_7100ad5200);

    g_unk_7172757538.field_0x0 = 0;
    g_unk_7172757538.fn = fn_7100adbbf4;
}
