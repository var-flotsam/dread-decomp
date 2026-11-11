// Unit 35 (base::curves::CCurveResource): loads the resource's object through the registry
// (g_unk_7101d4b5a0, created on first use): the id and path pair built from the resource's path
// (fn_710001ab24) is passed to the registry's vf_0x28 with the size at 0x18 of the request; on
// success the object replaces the one held at 0x58 (deleted through its vtable) and its vf_0x38
// is kept at 0x60. Returns whether it loaded. Names and layouts are placeholders.
#include "base/UnkRegistry.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkVector.h"
#include "game/UnkTU25.h"
#include "types.h"

extern UnkRegistry_0x70* g_unk_7101d4b5a0;

// The id and path pair, destroyed out of line.
struct UnkPathPairTemp_b45a1 {
    void* id;
    void* path;
    ~UnkPathPairTemp_b45a1() { fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(this)); }
};

struct UnkCurveObject_b45a1 {
    virtual ~UnkCurveObject_b45a1();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void* vf_0x38();
};

struct UnkCurveResource_b45a1 {
    void* vtable;
    u8 path[0x50];                    // 0x8
    UnkCurveObject_b45a1* object;     // 0x58
    void* data;                       // 0x60
};

struct UnkLoadRequest_b45a1 {
    u8 pad_0x0[0x18];
    u64 size;
};

// The registry's interface, as far as used here.
struct UnkRegistryLoader_b45a1 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual bool vf_0x28(UnkPathPairTemp_b45a1* key, UnkCurveObject_b45a1** out, u32 count, u64 size, u32 align,
                         u32 tag);
};

bool fn_7100029cbc(UnkCurveResource_b45a1* self, UnkLoadRequest_b45a1* request) {
    UnkRegistry_0x70* reg = g_unk_7101d4b5a0;
    if (!reg) {
        reg = static_cast<UnkRegistry_0x70*>(fn_71000839a0(0x70, 0x3f));
        fn_7100087388(reg);
        g_unk_7101d4b5a0 = reg;
    }
    UnkCurveObject_b45a1* object;
    bool ok;
    {
        UnkPathPairTemp_b45a1 key = fn_710001ab24(self->path);
        ok = reinterpret_cast<UnkRegistryLoader_b45a1*>(reg)->vf_0x28(&key, &object, 1, request->size, 0x80, 0x56);
    }
    if (ok) {
        UnkCurveObject_b45a1* loaded = object;
        UnkCurveObject_b45a1* old = self->object;
        self->object = loaded;
        if (old)
            delete old;
        self->data = self->object->vf_0x38();
    }
    return ok;
}
