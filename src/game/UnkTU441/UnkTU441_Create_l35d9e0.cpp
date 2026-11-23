// Unit 441: the render pool's creator for fade quads: a 0x180-byte model instance named `name` whose node is
// prepared and, for a non-empty path, loaded from it (one that fails is destroyed and freed); the rest are
// added to the pool's chained hash set and returned, also through `out` when given.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntDictionary.h"
#include "base/global/CStrId.h"
#include "game/UnkQuadNode_ovb0259.h"
#include "game/UnkQuadRender_ovb0259.h"
#include "game/UnkTU137.h"
#include "game/UnkTU287.h"
#include "types.h"
#include "game/UnkTU441.h"

struct UnkModelInstance;
struct UnkModelNode;
struct UnkTaggedObject;
void fn_7100147ca4(UnkTaggedObject* self);

struct UnkResHolder2;
bool fn_710035db30(UnkResHolder2* self, const base::global::CFilePathStrId& path);

using UnkQuadSet_l35d9e0 = base::global::CRntDictionary<u64, u64>;

UnkQuadNode_ovb0259* fn_710035d9e0(void* pool, const base::global::CFilePathStrId* path, UnkQuadNode_ovb0259** out,
                                   const base::global::CStrId* name) {
    UnkQuadSet_l35d9e0* set = static_cast<UnkQuadSet_l35d9e0*>(pool);
    auto* quad = static_cast<UnkQuadNode_ovb0259*>(fn_71000839a0(sizeof(UnkQuadNode_ovb0259), 0x38));
    fn_7100147c4c(reinterpret_cast<UnkModelInstance*>(quad), name);
    UnkQuadNode_ovb0259* result = nullptr;
    if (quad) {
        fn_710020a43c(reinterpret_cast<UnkModelNode*>(quad->node));
        if (path->data != &g_unk_7101d47200->empty && !fn_710035db30(reinterpret_cast<UnkResHolder2*>(quad), *path)) {
            fn_7100147ca4(reinterpret_cast<UnkTaggedObject*>(quad));
            operator delete(quad);
            return nullptr;
        }
        set->Insert((u64)quad, (u64)quad);
        result = quad;
    }
    if (out)
        *out = quad;
    return result;
}
