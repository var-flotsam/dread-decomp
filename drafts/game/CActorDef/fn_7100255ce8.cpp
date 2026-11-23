// CActorDef's unit (tools/tu_map.py unit 345): loads an actor definition from its file (the "actordef"
// category and the path `key`): reads its name and its class, then loads the given object, or a new one
// of that class, through a load request. An object that fails to load is released. Names and layouts
// are placeholders.
#include "game/CActorDef.h"

#include "base/UnkCache.h"
#include "base/UnkHashStream.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkStrPair.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/file/CThreadSafeDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU2527.h"
#include "game/UnkTU56.h"
#include "game/UnkTU83.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntFile;
using base::global::CRntString;
using base::global::CStrId;
using base::reflection::CType;

struct UnkFileReader;
struct UnkRefCounted;
bool fn_7100017390(UnkRefCounted* object, base::UnkLoadRequest* visitor);

extern char g_unk_7101a20c58[];
// CActorDef's class descriptor (the object CActorDef::refl_GetClass returns).
extern char g_unk_71726a0268[] __attribute__((visibility("hidden")));

// The object an actor definition is loaded into.
struct UnkActorDefObject_l255ce8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
};

// The class an object is created from: its size and constructor.
struct UnkActorDefClass_l255ce8 {
    virtual const base::reflection::CClass* vf_0x0() const;
    u8 pad_0x8[0x10];
    u64 size;                         // 0x18
    u8 pad_0x20[0x8];
    void (*construct)(void* object);  // 0x28
};

// The file a definition is read from: its category and its path.
struct UnkActorDefFileId_l255ce8 {
    CStrId category;
    CFilePathStrId path;

    UnkActorDefFileId_l255ce8(const CStrId& category, CFilePathStrId path) : category(category), path(path) {}
};

// The request an object is loaded with: the definition's name and the open file.
struct UnkActorDefLoadRequest_l255ce8 : base::UnkLoadRequest {
    CStrId name;      // 0x20
    CRntFile* file;   // 0x28

    UnkActorDefLoadRequest_l255ce8(const CStrId& name, CRntFile* file) : name(name), file(file) {
        vtable = g_unk_7101a20c58 + 0x10;
        field_0x8 = 0;
        field_0x10 = 2;
        field_0x18 = nullptr;
    }
    ~UnkActorDefLoadRequest_l255ce8() { vtable = g_unk_7101a20c58 + 0x10; }
};

// A new object of the class `cls`, or null when it is not a CActorDef or cannot be made.
static inline UnkActorDefObject_l255ce8* CreateObject(UnkActorDefClass_l255ce8* cls) {
    fn_7100254b14();
    if (!fn_710009d50c(reinterpret_cast<const CType*>(cls), reinterpret_cast<const CType*>(g_unk_71726a0268)) ||
        !cls->construct)
        return nullptr;
    void* object = fn_7100083990()->vf_0x40(cls->size, 8, 5);
    if (!object)
        return nullptr;
    cls->construct(object);
    return static_cast<UnkActorDefObject_l255ce8*>(object);
}

// The registered class named `name`, or null when there is none or it is not a class.
static inline UnkActorDefClass_l255ce8* FindClass(const CStrId& name) {
    u64 key = name.data->Key();
    auto* cls = reinterpret_cast<UnkActorDefClass_l255ce8*>(fn_71000a5118(g_unk_7101d4c8d0, &key));
    if (cls)
        cls = fn_710009d50c(reinterpret_cast<const CType*>(cls->vf_0x0()),
                            reinterpret_cast<const CType*>(fn_7100096100()))
                  ? cls
                  : nullptr;
    return cls;
}

UnkCounted_71011934b0* fn_7100255ce8(const CStrId& name, UnkKey_71011934b0* const* key, u64 flags) {
    UnkActorDefObject_l255ce8* object = reinterpret_cast<UnkActorDefObject_l255ce8*>(flags);
    const CFilePathStrId* path = reinterpret_cast<const CFilePathStrId*>(key);
    UnkActorDefFileId_l255ce8 fileId(reinterpret_cast<const CStrId&>(g_unk_71727b9530), *path);
    CRntFile file;
    fn_710007be8c(&file, 0x16, nullptr);
    bool opened;
    {
        UnkStrPair_0x10 s;
        fn_710008d2ec(&s, path);
        {
            CRntString mode("rb");
            opened = fn_710007c1ac(reinterpret_cast<UnkFileReader*>(&file), &s, &mode);
        }
        fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(&s));
    }
    UnkActorDefObject_l255ce8* result = nullptr;
    if (opened) {
        CRntFile_Seek(&file, 8, 0);
        CStrId id = name;
        const char* str;
        CRntFile_ReadCString(&file, &str);
        if (id == g_unk_7101d470e0)
            id = CStrId(str, true);
        CRntFile_ReadCString(&file, &str);
        CStrId className(str, true);
        UnkActorDefClass_l255ce8* cls = FindClass(className);
        UnkActorDefLoadRequest_l255ce8 request(id, &file);
        bool loaded;
        if (object) {
            loaded = fn_7100017390(reinterpret_cast<UnkRefCounted*>(object), &request);
        } else if (cls) {
            object = CreateObject(cls);
            if (object)
                loaded = fn_71000171a0(object, reinterpret_cast<const char*>(&fileId), &request);
        }
        if (object) {
            if (loaded) {
                result = object;
            } else {
                object->vf_0x28();
                object->vf_0x10();
            }
        }
    }
    fn_710007c0e4(&file);
    return reinterpret_cast<UnkCounted_71011934b0*>(result);
}
