// CCubeMapComponent's unit (tools/tu_map.py unit 1207): sets the texture of the first render object (+0x50):
// releases the texture held at +0x58, loads the one the path names (none for the empty path), and hands it,
// if it is a base::res::CRes, to the render object, which is told of the change. Names and layouts are
// placeholders.
#include "base/UnkCountedResource.h"
#include "base/UnkResKey.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::reflection::CType;
using base::reflection::UnkReflObject;

struct UnkCubeMapComponent_l79fca8 {
    u8 pad_0x0[0x50];
    UnkResHolder2* render;  // 0x50
    UnkResRef texture;      // 0x58
};

// Points `ref` at `res`, counting the new reference and releasing the old one.
static inline void SetRes_l79fca8(UnkResRef& ref, UnkCountedResource* res) {
    if (ref.ptr != res) {
        if (res)
            ++res->ref_count;
        UnkCountedResource* old = ref.ptr;
        if (old && --old->ref_count == 0)
            fn_710001737c(old);
        ref.ptr = res;
    }
}

// The resource if it is a base::res::CRes, else null.
static inline UnkCountedResource* AsRes_l79fca8(UnkCountedResource* res) {
    UnkReflObject* obj = reinterpret_cast<UnkReflObject*>(res);
    return res && fn_710009d50c(obj->vf_0x0(), reinterpret_cast<const CType*>(fn_71000e8a5c())) ? res : nullptr;
}

// Hands the texture to the render object, which is told of the change when there is one.
static inline void SetTexture_l79fca8(UnkResHolder2* render, UnkCountedResource* texture) {
    SetRes_l79fca8(render->first, texture);
    if (texture) {
        SetRes_l79fca8(render->second, texture);
        render->Notified()->vf_0x90();
    }
}

void fn_710079fca8(UnkCubeMapComponent_l79fca8* self, const CFilePathStrId& path) {
    if (UnkCountedResource* old = self->texture.ptr) {
        if (--old->ref_count == 0)
            fn_710001737c(old);
        self->texture.ptr = nullptr;
    }
    if (path.data != &g_unk_7101d47200->empty) {
        UnkResRegistry* registry = g_unk_7101d472f8;
        UnkResKey key(g_unk_716e4540f8.name, path);
        UnkResRef loaded = fn_71000ead80(registry, &key, 0x14, nullptr);
        SetRes_l79fca8(self->texture, loaded.ptr);
    }
    SetTexture_l79fca8(self->render, AsRes_l79fca8(self->texture.ptr));
}
