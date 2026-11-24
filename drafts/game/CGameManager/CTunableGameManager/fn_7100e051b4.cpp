// CGameManager::CTunableGameManager's unit (tools/tu_map.py): creates a character class and loads the
// runtime asset of the given name into it, under the "actordef" category; a class that loads goes into
// the factory's cache by its path. Names and layouts are placeholders.
#include "base/UnkCache.h"
#include "base/UnkCountedResource.h"
#include "base/UnkResKey.h"
#include "base/UnkTunableLine.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2527.h"
#include "game/UnkTU2729.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;

struct UnkActorDefFactory;
struct UnkCharClass_ovb0066;
struct UnkAssetNameData_ovb0113;

void CCharClass_Construct(UnkCharClass_ovb0066* self);
UnkTunableLine CGameManager_BuildRuntimeAssetPath(UnkAssetNameData_ovb0113** id);
extern char g_unk_7101a20c58[];

// A counted reference to a resource whose destructor reads the pointer once (as in
// src/game/CSpawnPointComponent/CSpawnPointComponent_b52.cpp).
struct UnkSpawnDefRef_b52 {
    UnkCountedResource* ptr;

    UnkSpawnDefRef_b52& operator=(const UnkSpawnDefRef_b52& other) {
        UnkCountedResource* p = other.ptr;
        if (ptr != p) {
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
        return *this;
    }
    ~UnkSpawnDefRef_b52() {
        UnkCountedResource* p = ptr;
        if (p && --p->ref_count == 0)
            fn_710001737c(p);
    }
};

// The new character class, as far as this function uses it.
struct UnkCharClass_le051b4 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    u8 pad_0x8[0x54 - 0x8];
    s32 ref_count;  // 0x54
};

// A file-path id built from a string's text (fn_710001a270).
struct UnkStringPath_le051b4 : base::global::UnkFilePathStrIdBase {
    explicit UnkStringPath_le051b4(const CRntString& text) { fn_710001a270(this, text, true); }
    ~UnkStringPath_le051b4() { reinterpret_cast<CFilePathStrId*>(this)->~CFilePathStrId(); }
    const CFilePathStrId& Path() const { return *reinterpret_cast<const CFilePathStrId*>(this); }
};

// What the cache loads by: the category and the file path.
struct UnkActorDefKey_le051b4 {
    CStrId category;
    CFilePathStrId path;

    UnkActorDefKey_le051b4(const CStrId& category, const CFilePathStrId& path)
        : category(category), path(path) {}
};

// The load request for a named asset.
struct UnkActorDefRequest_le051b4 {
    char* vtable;
    u64 field_0x8;
    u32 field_0x10;
    void* field_0x18;
    CStrId name;  // 0x20
    void* field_0x28;

    explicit UnkActorDefRequest_le051b4(const CStrId& name)
        : vtable(g_unk_7101a20c58 + 0x10), field_0x8(0), field_0x10(2), field_0x18(nullptr), name(name),
          field_0x28(nullptr) {}
    ~UnkActorDefRequest_le051b4() { vtable = g_unk_7101a20c58 + 0x10; }
};

UnkSpawnDefRef_b52 fn_7100e051b4(UnkActorDefFactory* factory, const CStrId* name) {
    UnkCharClass_le051b4* object = static_cast<UnkCharClass_le051b4*>(fn_71000839a0(0x168, 5));
    CCharClass_Construct(reinterpret_cast<UnkCharClass_ovb0066*>(object));
    UnkSpawnDefRef_b52 ref;
    if (object) {
        object->vf_0x18();
        ++object->ref_count;
        ref.ptr = reinterpret_cast<UnkCountedResource*>(object);
    } else {
        ref.ptr = nullptr;
    }

    UnkActorDefKey_le051b4 key(
        *reinterpret_cast<const CStrId*>(g_unk_71727b9530),
        UnkStringPath_le051b4(*CGameManager_BuildRuntimeAssetPath(reinterpret_cast<UnkAssetNameData_ovb0113**>(const_cast<CStrId*>(name)))
                                   .Str())
            .Path());
    UnkActorDefRequest_le051b4 request(*name);
    UnkCountedResource* resource = ref.ptr;
    if (fn_71000171a0(resource, reinterpret_cast<const char*>(&key),
                      reinterpret_cast<base::UnkLoadRequest*>(&request)))
        fn_7101193cb4(reinterpret_cast<UnkCache_71011934b0*>(factory),
                      reinterpret_cast<UnkCounted_71011934b0*>(resource),
                      reinterpret_cast<UnkKey_71011934b0* const*>(&key.path));
    return ref;
}
