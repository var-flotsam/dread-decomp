// base::animation::CAnimationResource's unit (unit 11 of tools/tu_map.py): loads the resource's
// object through the registry (g_unk_7101d4b5a0, created on first use), as CCurveResource's load
// (src/base/curves/CCurveResource_Load_b45.cpp; tag 10 here). On success the object replaces the
// one held at 0x58, its data (vf_0x38) is kept at 0x60, and the section of the data whose key is
// the hash 0x6c164c71ad4ccaf4 at 0x68 (null without one). Returns whether it loaded. Names and
// layouts are placeholders.
#include "base/UnkRegistry.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkVector.h"
#include "base/animation/CAnimationResource.h"
#include "game/UnkTU25.h"
#include "types.h"

// As src/base/curves/CCurveResource_Load_b45.cpp declares it.
struct UnkPathPairTemp_b45a1 {
    void* id;
    void* path;
    ~UnkPathPairTemp_b45a1() { fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(this)); }
};

struct UnkAnimSection_b49a1 {
    u64 key;
    u8 unk_0x8[0x28];
};

struct UnkAnimData_b49a1 {
    u8 unk_0x0[0x10];
    s32 count;
    u8 unk_0x14[4];
    UnkAnimSection_b49a1 sections[1];
};

struct UnkAnimObject_b49a1 {
    virtual ~UnkAnimObject_b49a1();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual UnkAnimData_b49a1* vf_0x38();
};

struct UnkAnimResource_b49a1 {
    void* vtable;
    u8 path[0x50];                    // 0x8
    UnkAnimObject_b49a1* object;      // 0x58
    UnkAnimData_b49a1* data;          // 0x60
    UnkAnimSection_b49a1* section;    // 0x68
};

struct UnkAnimLoadRequest_b49a1 {
    u8 pad_0x0[0x18];
    u64 size;
};

struct UnkAnimRegistryLoader_b49a1 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual bool vf_0x28(UnkPathPairTemp_b45a1* key, UnkAnimObject_b49a1** out, u32 count, u64 size, u32 align,
                         u32 tag);
};

bool fn_7100004db0(UnkAnimResource_b49a1* self, UnkAnimLoadRequest_b49a1* request) {
    self->data = nullptr;
    UnkRegistry_0x70* reg = g_unk_7101d4b5a0;
    if (!reg) {
        reg = static_cast<UnkRegistry_0x70*>(fn_71000839a0(0x70, 0x3f));
        fn_7100087388(reg);
        g_unk_7101d4b5a0 = reg;
    }
    UnkAnimObject_b49a1* object;
    bool ok;
    {
        UnkPathPairTemp_b45a1 key = fn_710001ab24(self->path);
        ok = reinterpret_cast<UnkAnimRegistryLoader_b49a1*>(reg)->vf_0x28(&key, &object, 1, request->size, 0x80, 10);
    }
    if (ok) {
        UnkAnimObject_b49a1* loaded = object;
        UnkAnimObject_b49a1* old = self->object;
        self->object = loaded;
        if (old)
            delete old;
        UnkAnimData_b49a1* data = self->object->vf_0x38();
        self->data = data;
        if (data) {
            UnkAnimSection_b49a1* found;
            u32 i = 0;
            for (;;) {
                asm("" : "+r"(i));
                if ((s32)i >= data->count) {
                    found = nullptr;
                    break;
                }
                UnkAnimSection_b49a1* s = &data->sections[i];
                ++i;
                if (s->key == 0x6c164c71ad4ccaf4) {
                    found = s;
                    break;
                }
            }
            self->section = found;
        }
    }
    return ok;
}
