#pragma once

// The resource registry's key and reference types, and the lookups that use them. The registry is
// the object g_unk_7101d472f8 points at (src/base/UnkResRegistry.cpp builds it); a resource is named
// by its type's name and its file path, and a holder keeps a counted reference to it
// (base/UnkCountedResource.h). Names and layouts are placeholders inferred from the code.

#include "base/UnkCountedResource.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

// A resource type (as src/game/msapi/res/CRenderRes/fn_71001b1dd0.cpp registers them): its name first.
struct UnkResType {
    base::global::CStrId name;
};

// Registered resource types, by what they load.
extern const UnkResType g_unk_716e453ff8;  // models
extern const UnkResType g_unk_716e4540f8;  // textures
extern const UnkResType g_unk_716e454078;  // what the sonar ability loads
extern const UnkResType g_unk_716e453f78;  // what unit 2739 loads (src/game/UnkUnit2739/fn_71011ab410.cpp)

struct UnkResRegistry;
extern UnkResRegistry* g_unk_7101d472f8;

// A path id whose release decrements and tests the count in two steps.
struct UnkResPathId {
    base::global::UnkStringData* data;

    UnkResPathId(const base::global::CFilePathStrId& other) : data(other.data) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    ~UnkResPathId() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            data = &pool->empty;
        }
    }
};

// A resource's key: its type's name and its path (fn_7100333544 is the destructor).
struct UnkResKey {
    base::global::CStrId type;
    UnkResPathId path;

    UnkResKey(const base::global::CStrId& t, const base::global::CFilePathStrId& p) : type(t), path(p) {}
};

// A counted reference to a resource.
struct UnkResRef {
    UnkCountedResource* ptr;

    UnkResRef() : ptr(nullptr) {}
    UnkResRef& operator=(const UnkResRef& other) {
        if (ptr != other.ptr) {
            UnkCountedResource* p = other.ptr;
            if (p)
                ++p->ref_count;
            UnkCountedResource* old = ptr;
            if (old && --old->ref_count == 0)
                fn_710001737c(old);
            ptr = p;
        }
        return *this;
    }
    ~UnkResRef() {
        if (ptr && --ptr->ref_count == 0)
            fn_710001737c(ptr);
    }
};

// The registered key for `key` (its parts empty when there is none), and the resource it names.
UnkResKey fn_71000ea448(UnkResRegistry* registry, const UnkResKey* key, void* a);
UnkResRef fn_71000ead80(UnkResRegistry* registry, const UnkResKey* key, u8 flag, void* a);

// An object that keeps one resource in two holders (+0x10 and +0xa0) and tells the object at +0x28
// when it changes: the sonar ability (src/game/CAbilitySonar.cpp) and the components that copy its
// loader for their own resource types.
struct UnkResHolderNotified {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();  // told that the resource changed
};

// The sonar's layout; the copies' differ only in the offsets.
struct UnkResHolder {
    u8 pad_0x0[0xa];
    u8 flag;  // 0xa, passed to the resource lookup
    u8 pad_0xb[0x5];
    UnkResRef first;  // 0x10
    u8 pad_0x18[0x10];
    u8 notified[0x8];  // 0x28, an object told about the change (its class is not known here)
    u8 pad_0x30[0x70];
    UnkResRef second;  // 0xa0

    UnkResHolderNotified* Notified() { return reinterpret_cast<UnkResHolderNotified*>(notified); }
};

// Loads the resource of type `Type` at `path`; when there is one, keeps it in both holders
// (field_0x10 and field_0xa0 in UnkResHolder's layout), tells the notified object (vf_0x90) and
// returns true. Each user is its own copy, with its own layout `Self` (the same member names).
template <const UnkResType& Type, class Self>
__attribute__((always_inline)) inline bool UnkResHolderLoad(Self* self, const base::global::CFilePathStrId& path) {
    UnkResRef resource;
    u8 flag = self->flag;
    UnkResKey key(Type.name, path);
    UnkResKey found = fn_71000ea448(g_unk_7101d472f8, &key, nullptr);
    if (found.type.data == &g_unk_7101d47108->empty || found.path.data == &g_unk_7101d47200->empty)
        return false;
    resource = fn_71000ead80(g_unk_7101d472f8, &found, flag, nullptr);
    if (!resource.ptr)
        return false;
    self->first = resource;
    UnkResHolderNotified* notified = self->Notified();
    self->second = resource;
    notified->vf_0x90();
    return true;
}
