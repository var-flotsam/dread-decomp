// CGameManager::CTunableGameManager's unit (tools/tu_map.py unit 2553): makes sure a model is loaded for a file
// path: when the map at +0x29c8 has none and the registry knows the file, creates the model, puts it on layer 0
// and keeps it under the path. False when the file is unknown. Names and layouts are placeholders.
#include <new>

#include "base/UnkRegistry.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkStrPair.h"
#include "base/animation/CAnimationResource.h"
#include "base/file/CThreadSafeDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntDictionary.h"
#include "base/global/CStrId.h"
#include "game/CSpittailMagmaBallMovementComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU127.h"
#include "game/UnkTU253.h"
#include "types.h"

using base::global::CFilePathStrId;

// The model's parts this uses: its scene node at +8 and its data at +0x138.
struct UnkModelParts_e0484c {
    u8 pad_0x0[0x8];
    UnkSceneNode node;
    u8 pad_0x20[0x118];
    UnkModelData* data;
};
static_assert(__builtin_offsetof(UnkModelParts_e0484c, data) == 0x138, "layout");

// A hash map by file path whose nodes are also kept in a list.
using UnkPathMap_e0484c = base::global::CRntDictionary<CFilePathStrId, UnkModel*>;

template <class T, u64 Size, void (*Ctor)(T*)>
static inline T* Singleton_e0484c(T*& instance) {
    if (!instance) {
        T* p = static_cast<T*>(fn_71000839a0(Size, 0x3f));
        Ctor(p);
        instance = p;
    }
    return instance;
}

// The game manager's members this uses (game/UnkGameManager.h leaves them as padding).
struct UnkGameManagerModels_e0484c {
    u8 pad_0x0[0x29c8];
    UnkPathMap_e0484c field_0x29c8;
};

bool fn_7100e0484c(UnkGameManager* manager, const CFilePathStrId* path) {
    auto* self = reinterpret_cast<UnkGameManagerModels_e0484c*>(manager);
    if (self->field_0x29c8.FindNode(*path))
        return true;
    UnkRegistry_0x70* registry = Singleton_e0484c<UnkRegistry_0x70, 0x70, fn_7100087388>(g_unk_7101d4b5a0);
    bool known;
    {
        UnkStrPair_0x10 s;
        fn_710008d2ec(&s, path);
        known = fn_7100087850(registry, &s);
        fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(&s));
    }
    if (!known)
        return false;
    UnkRenderManager* render = g_unk_7101d47300;
    UnkModel* created = nullptr;
    fn_71006cae2c(render->field_0x218, *path, &created, &base::global::CStrId::Empty());
    fn_7100219ae0(render, created ? &reinterpret_cast<UnkModelParts_e0484c*>(created)->node : nullptr, 0);
    UnkModel* model = created;
    fn_71001c0968(reinterpret_cast<UnkModelParts_e0484c*>(model)->data, true);
    fn_71001436a0(model, 0);
    self->field_0x29c8.Insert(*path, model);
    return true;
}
