// Unit 322: the constructor of an object holding a texture, an array of shared objects and a second
// container: it loads "gui/textures/white.bctex" and keeps it if it is a render resource.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/UnkResKey.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntVector;
using base::global::UnkStrIdInline;

extern void* g_unk_7101a20420[];  // the vtable

struct UnkShared_l21c4cc;

// A container of its own allocator, with growth step 10 and allocation flags 0xe.
struct UnkList_l21c4cc {
    void* field_0x0 = nullptr;
    u64 field_0x8 = 0;
    u64 field_0x10 = 0;
    u32 step = 10;
    u32 flags = 0xe;
    base::UnkAllocator* allocator = fn_7100083990();
};

struct UnkTextureHolder_l21c4cc {
    void** vtable;
    u8 pad_0x8[0x8];
    UnkResRef texture;                         // 0x10
    CRntVector<UnkShared_l21c4cc*> objects;    // 0x18
    UnkList_l21c4cc field_0x40;                // 0x40
};
static_assert(__builtin_offsetof(UnkTextureHolder_l21c4cc, field_0x40) == 0x40, "layout");

// The resource if it is a render resource (msapi::res::CRenderRes), else null.
static inline UnkCountedResource* AsRenderRes(UnkCountedResource* res) {
    if (!res)
        return nullptr;
    const base::reflection::UnkReflObject* obj = reinterpret_cast<const base::reflection::UnkReflObject*>(res);
    return fn_710009d50c(obj->vf_0x0(), fn_71001b122c()) ? res : nullptr;
}

// The registry key of the texture at path.
static inline UnkResKey TextureKey(const char* path) {
    return UnkResKey(g_unk_716e4540f8.name, CFilePathStrId(UnkStrIdInline(), path, true));
}

void fn_710021c4cc(UnkTextureHolder_l21c4cc* self) {
    self->vtable = g_unk_7101a20420 + 2;
    new (&self->texture) UnkResRef();
    new (&self->objects) CRntVector<UnkShared_l21c4cc*>(0xf);
    new (&self->field_0x40) UnkList_l21c4cc;

    UnkResKey key = TextureKey("gui/textures/white.bctex");
    UnkResRef loaded = fn_71000ead80(g_unk_7101d472f8, &key, 8, nullptr);
    UnkResRef texture;
    if (UnkCountedResource* res = AsRenderRes(loaded.ptr)) {
        ++res->ref_count;
        texture.ptr = res;
    }
    self->texture = texture;
}
