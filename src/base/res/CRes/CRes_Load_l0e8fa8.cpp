// base::res::CRes's unit (tools/tu_map.py): loads the resource's object through the registry
// (g_unk_7101d4b5a0, created on first use), as CCurveResource's load does: the kind (by the path's
// extension, else the request's) picks the alignment, and on success the object's data (vf_0x30) is
// kept at 0x68. Returns whether it loaded. Names and layouts are placeholders.
#include "base/UnkRegistry.h"
#include "base/UnkStrIdPathPair.h"
#include "base/animation/CAnimationResource.h"
#include "base/file/CThreadSafeDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "game/UnkPathPairTemp_b45a1.h"
#include "game/UnkTU25.h"
#include "types.h"

using base::global::CFilePathStrId;

// The resource's key: an id and path pair made and destroyed out of line, assigned in place.
struct UnkResKey_l0e8fa8 {
    void* id;
    void* path;

    UnkResKey_l0e8fa8() { fn_710008d2a0(reinterpret_cast<UnkPathPairTemp_b45a1*>(this)); }
    ~UnkResKey_l0e8fa8() { fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(this)); }

    UnkResKey_l0e8fa8& operator=(const UnkPathPairTemp_b45a1& other) {
        *reinterpret_cast<UnkStrIdPathPair*>(this) = reinterpret_cast<const UnkStrIdPathPair&>(other);
        return *this;
    }
};

u32 CRes_GetKindFromPathExtension(void* self, const CFilePathStrId& path, u32 fallback);

struct UnkResObject_l0e8fa8 {
    virtual ~UnkResObject_l0e8fa8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void* vf_0x30();
};

struct UnkResPath_l0e8fa8 {
    void* id;
    CFilePathStrId file;  // 0x8
};

struct UnkRes_l0e8fa8 {
    void* vtable;
    UnkResPath_l0e8fa8 path;  // 0x8
    u8 pad_0x18[0x40];
    UnkResObject_l0e8fa8* object;  // 0x58
    u32 kind;  // 0x60
    u8 pad_0x64[0x4];
    void* data;  // 0x68
};
static_assert(__builtin_offsetof(UnkRes_l0e8fa8, data) == 0x68, "layout");

struct UnkResLoadRequest_l0e8fa8 {
    u8 pad_0x0[0x10];
    u32 kind;  // 0x10
    u8 pad_0x14[0x4];
    u64 size;  // 0x18
};

// The registry's interface, as far as used here.
struct UnkResRegistryLoader_l0e8fa8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual bool vf_0x28(UnkResKey_l0e8fa8* key, UnkResObject_l0e8fa8** out, u32 count, u64 size, u32 align,
                         u32 kind);
};

static inline UnkRegistry_0x70* Registry() {
    UnkRegistry_0x70* reg = g_unk_7101d4b5a0;
    if (!reg) {
        reg = static_cast<UnkRegistry_0x70*>(fn_71000839a0(0x70, 0x3f));
        fn_7100087388(reg);
        g_unk_7101d4b5a0 = reg;
    }
    return reg;
}

bool fn_71000e8fa8(UnkRes_l0e8fa8* self, UnkResLoadRequest_l0e8fa8* request) {
    u32 kind = CRes_GetKindFromPathExtension(self, self->path.file, request->kind);
    self->kind = kind;
    u32 align;
    switch (kind) {
    case 11:
        align = 0x20;
        break;
    case 9:
    case 20:
        align = 0x80;
        break;
    // Its own group, apart from 9 and 20: written as one group the switch becomes bit tests.
    case 29:
    case 30:
    case 31:
        align = 0x80;
        break;
    default:
        align = 8;
        break;
    }
    if (request->kind == 12)
        self->kind = 12;

    UnkResKey_l0e8fa8 key;
    key = MakePathPairFromRegistryCallback(&self->path);
    bool ok = reinterpret_cast<UnkResRegistryLoader_l0e8fa8*>(Registry())
                  ->vf_0x28(&key, &self->object, 1, request->size, align, self->kind);
    self->data = ok ? self->object->vf_0x30() : nullptr;
    return ok;
}
