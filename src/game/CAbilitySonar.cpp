// The CAbilitySonar unit (tools/tu_map.py unit 434, by its reflected class): fn_7100339ea0 loads a
// resource of one registered type from a path and keeps it in two holders. Which object the holders
// belong to is inferred from the code only; names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// A resource type (as src/game_s7/fn_71001b1dd0.cpp registers them): its name first.
struct UnkResType {
    CStrId name;
};
extern const UnkResType g_unk_716e454078;

struct UnkResRegistry;
extern UnkResRegistry* g_unk_7101d472f8;

// A path id whose release decrements and tests the count in two steps.
struct UnkResPathId {
    UnkStringData* data;

    UnkResPathId(const CFilePathStrId& other) : data(other.data) {
        if (&g_unk_7101d47200->empty != data)
            ++data->ref_count;
    }
    ~UnkResPathId() {
        if (UnkStringPool* pool = g_unk_7101d47200) {
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
    CStrId type;
    UnkResPathId path;

    UnkResKey(const CStrId& t, const CFilePathStrId& p) : type(t), path(p) {}
};

// A counted reference to a resource (base/UnkCountedResource.h).
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

struct UnkSonarNotified {
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

struct UnkSonarResources {
    u8 pad_0x0[0xa];
    u8 field_0xa;  // passed to the resource lookup
    u8 pad_0xb[0x5];
    UnkResRef field_0x10;
    u8 pad_0x18[0x10];
    u8 field_0x28[0x8];  // an object told about the change (its class is not known here)
    u8 pad_0x30[0x70];
    UnkResRef field_0xa0;

    UnkSonarNotified* Notified() { return reinterpret_cast<UnkSonarNotified*>(field_0x28); }
};

// Loads the resource of type g_unk_716e454078 at `path`; when there is one, keeps it at +0x10 and
// +0xa0, tells the object at +0x28 (vf_0x90) and returns true.
bool fn_7100339ea0(UnkSonarResources* self, const CFilePathStrId& path) {
    UnkResRef resource;
    u8 flag = self->field_0xa;
    UnkResKey key(g_unk_716e454078.name, path);
    UnkResKey found = fn_71000ea448(g_unk_7101d472f8, &key, nullptr);
    if (found.type.data == &g_unk_7101d47108->empty || found.path.data == &g_unk_7101d47200->empty)
        return false;
    resource = fn_71000ead80(g_unk_7101d472f8, &found, flag, nullptr);
    if (!resource.ptr)
        return false;
    self->field_0x10 = resource;
    UnkSonarNotified* notified = self->Notified();
    self->field_0xa0 = resource;
    notified->vf_0x90();
    return true;
}
