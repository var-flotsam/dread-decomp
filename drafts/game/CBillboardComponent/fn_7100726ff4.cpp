// CBillboardComponent's unit (tools/tu_map.py, unit 1152): after the base component's step, caches the
// owner as the billboard character class it should be and creates the two billboards its paths name (a
// material for a ".bsmat" path, a texture otherwise). Names and layouts are placeholders.
#include "base/UnkResKey.h"
#include "base/global/CRntDictionary.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkComponent.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::reflection::UnkReflObject;

struct UnkBillboardObject_ovb0113;
struct UnkWidgetArg;
using UnkBillboardSet_ovb0113 = base::global::CRntDictionary<u64, u64>;
UnkBillboardObject_ovb0113* fn_7100729eac(UnkBillboardSet_ovb0113* set, const UnkResKey* key,
                                          UnkBillboardObject_ovb0113** out, UnkWidgetArg* arg);

struct UnkBillboardCharClass_l726ff4 : UnkReflObject {
    u8 pad_0x8[0x60];
    CFilePathStrId path_a;  // 0x68
    CFilePathStrId path_b;  // 0x70
};

struct UnkBillboardComponent_l726ff4 {
    u8 pad_0x0[0x10];
    UnkReflObject* owner;  // 0x10
    u8 pad_0x18[0x28];
    UnkBillboardCharClass_l726ff4* char_class;  // 0x40
    u8 pad_0x48[0xd8];
    UnkBillboardObject_ovb0113* billboard_a;  // 0x120
    UnkBillboardObject_ovb0113* billboard_b;  // 0x128
};
static_assert(__builtin_offsetof(UnkBillboardComponent_l726ff4, billboard_a) == 0x120, "layout");

// The resource type a path names: a material for a ".bsmat" path, a texture otherwise.
static inline const UnkResType& PathType(const CFilePathStrId& path) {
    ++path.data->reads;
    if (path.data == &g_unk_7101d47200->empty)
        return g_unk_716e4540f8;
    return fn_710007fb9c(&path.data->String(), ".bsmat") ? g_unk_716e453f78 : g_unk_716e4540f8;
}

bool fn_7100726ff4(UnkBillboardComponent_l726ff4* self, void* arg) {
    bool result = fn_71010c41bc(reinterpret_cast<UnkBaseComponent*>(self), arg);

    UnkBillboardCharClass_l726ff4* char_class = nullptr;
    if (UnkReflObject* owner = self->owner) {
        if (fn_710009d50c(owner->vf_0x0(), fn_710072be34()))
            char_class = static_cast<UnkBillboardCharClass_l726ff4*>(owner);
    }
    self->char_class = char_class;

    // Each path names a billboard to create: none for an empty path or one that is not registered.
    if (self->char_class->path_a.data != &g_unk_7101d47200->empty) {
        UnkBillboardObject_ovb0113* billboard;  // set just before the creation
        const UnkResType& type = PathType(self->char_class->path_a);
        UnkResKey key(type.name, self->char_class->path_a);
        UnkResKey found = fn_71000ea448(g_unk_7101d472f8, &key, nullptr);
        if (found.type.data != &g_unk_7101d47108->empty && found.path.data != &g_unk_7101d47200->empty) {
            UnkRenderManager* manager = g_unk_7101d47300;
            billboard = nullptr;
            fn_7100729eac(reinterpret_cast<UnkBillboardSet_ovb0113*>(manager->field_0x218), &found, &billboard,
                          reinterpret_cast<UnkWidgetArg*>(&g_unk_7101d470e0));
            fn_7100219ae0(manager, reinterpret_cast<UnkSceneNode*>(billboard), 0);
            self->billboard_a = billboard;
        }
    }
    if (self->char_class->path_b.data != &g_unk_7101d47200->empty) {
        UnkBillboardObject_ovb0113* billboard;  // set just before the creation
        const UnkResType& type = PathType(self->char_class->path_b);
        UnkResKey key(type.name, self->char_class->path_b);
        UnkResKey found = fn_71000ea448(g_unk_7101d472f8, &key, nullptr);
        if (found.type.data != &g_unk_7101d47108->empty && found.path.data != &g_unk_7101d47200->empty) {
            UnkRenderManager* manager = g_unk_7101d47300;
            billboard = nullptr;
            fn_7100729eac(reinterpret_cast<UnkBillboardSet_ovb0113*>(manager->field_0x218), &found, &billboard,
                          reinterpret_cast<UnkWidgetArg*>(&g_unk_7101d470e0));
            fn_7100219ae0(manager, reinterpret_cast<UnkSceneNode*>(billboard), 0);
            self->billboard_b = billboard;
        }
    }
    return result;
}
