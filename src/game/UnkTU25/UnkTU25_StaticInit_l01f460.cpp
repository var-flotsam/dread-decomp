// Unit 25: its static initialiser: the "sources" and "binaries" ids, the reflected types of
// base::core::AssetName, base::core::AssetID and base::global::CRntVector<base::core::AssetName> (with its
// iterators), AssetID's conversions from CRntString and CStrId and to CRntString, and an empty resource
// type registry. Names and layouts are placeholders.
#include <new>

#include "base/UnkAtExit.h"
#include "base/UnkResType.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflConverter.h"
#include "base/reflection/UnkReflTypeGlobals.h"
#include "game/UnkTU25.h"
#include "game/UnkTUInit.h"
#include "types.h"

using base::global::UnkStrIdBase;

// The unit's two header ids, "sources" first.
struct UnkHeaderIds_l01f460 {
    UnkStrIdBase sources;
    UnkStrIdBase binaries;
};
extern char g_unk_7101d4e570[] __attribute__((visibility("hidden")));

static inline UnkHeaderIds_l01f460& HeaderIds() { return *reinterpret_cast<UnkHeaderIds_l01f460*>(g_unk_7101d4e570); }

// The resource type registry (destroyed by fn_7100004b94): a name, the type descriptions and two
// arrays of ids.
struct UnkResTypes_l01f460;
void fn_7100005534(UnkResTypes_l01f460* self, u32 capacity);

struct UnkResTypes_l01f460 : base::global::CRntVector<unk_static_init::UnkResType> {
    UnkResTypes_l01f460() : CRntVector(0xf) { fn_7100005534(this, 0); }
};

struct UnkResRegistry_l01f460 {
    CStrId name;
    UnkResTypes_l01f460 types;
    base::global::CRntVector<CStrId> ids_a{0xf};
    base::global::CRntVector<CStrId> ids_b{0xf};

    UnkResRegistry_l01f460() : name(nullptr, true) {}
};
extern UnkResRegistry_l01f460 g_unk_7101d4e070;

namespace base::reflection {
class CClass;
}

// The reflected types and the lists they are noted in.
extern char g_unk_7101d4d998[];  // base::core::AssetName
extern char g_unk_7101d4dad0[];
extern char g_unk_7101d4dc10[];
extern char g_unk_7101d4dc11[];
extern char g_unk_7101d4dc12[];
extern char g_unk_7101d4dc18[];  // its iterator's value type
extern char g_unk_7101d4dd50[];
extern char g_unk_7101d4dd51[];
extern char g_unk_7101d4dd58[];  // its const iterator's value type
extern char g_unk_7101d4de90[];
extern char g_unk_7101d4de91[];
extern char g_unk_7101d4de98[];  // base::global::CRntVector<base::core::AssetName>
extern char g_unk_7101d4e008[];
extern UnkConverter g_unk_7101d4e010;
extern UnkConverter g_unk_7101d4e030;
extern UnkConverter g_unk_7101d4e050;

void fn_7100018bc4(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(),
                   void* b);
void fn_7100018ec8(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(),
                   void* b);
void fn_7100019588(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(),
                   void* b);
void fn_7100019b6c(UnkReflType* self, const base::global::CStrId& name, UnkReflType* base, void* a, void (*init)(),
                   void* b);
void fn_7100019dcc(UnkReflType* self, const base::global::CStrId& name, void* a, void* b);
void fn_7100018a44(base::reflection::CClass* cls);
void fn_7100018d48(base::reflection::CClass* cls);
void fn_7100019204(base::reflection::CClass* cls);
void fn_71000197e8(base::reflection::CClass* cls);
const base::reflection::CType* fn_710001904c();
const base::reflection::CType* fn_7100019128();
const base::reflection::CType* fn_710001970c();
const base::reflection::CType* fn_7100019cf0();

// AssetID's conversions.
struct UnkPair_l01ef24;
struct UnkInlineString;
UnkPair_l01ef24* fn_710001ef24(UnkPair_l01ef24* self, const UnkInlineString* other);
void fn_710001f1c8();
void fn_710001ca14() __attribute__((visibility("hidden")));

static inline UnkReflType* AsUnk(char* t) { return reinterpret_cast<UnkReflType*>(t); }
static inline UnkReflType* AsUnk(const base::reflection::CType* t) {
    return reinterpret_cast<UnkReflType*>(const_cast<base::reflection::CType*>(t));
}
static inline void (*AsInit(void (*init)(base::reflection::CClass*)))() {
    return reinterpret_cast<void (*)()>(init);
}

void fn_710001f460() {
    new (&HeaderIds().sources) UnkStrIdBase("sources", true);
    fn_7100000250(fn_710000051c, &HeaderIds().sources, g_unk_7101a19000);
    new (&HeaderIds().binaries) UnkStrIdBase("binaries", true);
    fn_7100000250(fn_710000051c, &HeaderIds().binaries, g_unk_7101a19000);

    InitClass(AsUnk(g_unk_7101d4d998), "base::core::AssetName", fn_7100018bc4, AsInit(fn_7100018a44));
    fn_710009ec00(g_unk_7101d4dad0, g_unk_7101d4d998);
    InitClass(AsUnk(&g_unk_7101d4dad8), "base::core::AssetID", fn_7100018ec8, AsInit(fn_7100018d48));
    fn_710009ec00(g_unk_7101d4dc10, &g_unk_7101d4dad8);
    fn_710009ec00(g_unk_7101d4dc11, const_cast<base::reflection::CType*>(fn_710001904c()));
    fn_710009ec00(g_unk_7101d4dc12, const_cast<base::reflection::CType*>(fn_7100019128()));
    InitClass(AsUnk(g_unk_7101d4dc18),
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<base::core::AssetName>>::"
              "TKeyElementIterator::value_type",
              fn_7100019588, AsInit(fn_7100019204), AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(g_unk_7101d4dd50, g_unk_7101d4dc18);
    fn_710009ec00(g_unk_7101d4dd51, const_cast<base::reflection::CType*>(fn_710001970c()));
    InitClass(AsUnk(g_unk_7101d4dd58),
              "base::reflection::CollectionTypeMapper<base::global::CRntVector<base::core::AssetName>>::"
              "TKeyElementConstIterator::value_type",
              fn_7100019b6c, AsInit(fn_71000197e8), AsUnk(&g_unk_7101d5e2e0));
    fn_710009ec00(g_unk_7101d4de90, g_unk_7101d4dd58);
    fn_710009ec00(g_unk_7101d4de91, const_cast<base::reflection::CType*>(fn_7100019cf0()));
    InitVector(AsUnk(g_unk_7101d4de98), "base::global::CRntVector<base::core::AssetName>", fn_7100019dcc);
    fn_710009ec00(g_unk_7101d4e008, g_unk_7101d4de98);

    InitConverterFrom(g_unk_7101d4e010, AsUnk(&g_unk_7101d4dad8), refl_TypeOf<fn_7100007f84>,
                      reinterpret_cast<void (*)()>(fn_710001ef24));
    InitConverterFrom(g_unk_7101d4e030, AsUnk(&g_unk_7101d4dad8), refl_TypeOf<fn_7100009658>, fn_710001f1c8);
    InitConverterTo(g_unk_7101d4e050, AsUnk(&g_unk_7101d4dad8), refl_TypeOf<fn_7100007f84>, fn_710001ca14);

    new (&g_unk_7101d4e070) UnkResRegistry_l01f460();
    fn_7100000250(fn_7100004b94, &g_unk_7101d4e070, g_unk_7101a19000);
}
