#include "base/UnkDestructors.h"
#include "base/UnkRegistry.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CRntVector.h"
#include "base/global/CFilePathStrId.h"
#include "game/UnkRegistryHandler_b0248.h"
#include "game/UnkRegistryNode_b0248.h"
#include "game/UnkRegistryString_b0248.h"
#include "game/UnkRegistry_b0248.h"
#include "game/UnkTU63.h"
#include "types.h"
#include "base/UnkVtableList.h"

using base::UnkStrIdMemberOf;

// A path id and the link to the next one.
struct UnkListedPath_b0248 {
    UnkStrIdMemberOf<g_unk_7101d47200> id;
    UnkListedPath_b0248* next;
};

struct UnkFilePathList_b0248 : base::UnkVtableList<UnkListedPath_b0248> {
    UnkFilePathList_b0248() {
        vtable = &g_unk_7101a1b030[2];
        count = 0;
        head = nullptr;
        tail = nullptr;
        allocator = fn_7100083990();
        flags = 0x13;
    }
    ~UnkFilePathList_b0248() {
        vtable = &g_unk_7101a1b030[2];
        for (UnkListedPath_b0248* n = head; n;) {
            UnkListedPath_b0248* next = n->next;
            n->id.~UnkStrIdMemberOf();
            allocator->vf_0x48(n);
            n = next;
        }
    }
};

bool fn_7100087b0c(UnkRegistry_b0248* self, UnkRegistryHandler_b0248* handler, UnkListedPath_b0248* path, u32 depth,
                   UnkFilePathList_b0248* out) {
    if (depth == 0) {
        u32 result = handler->vf_0x80(path, 0, true, 0, out);
        if (result == 0)
            return true;
        if (result != 1 && self->on_error)
            self->on_error(self->context, handler, result);
        return false;
    }
    UnkFilePathList_b0248 children;
    u32 result = handler->vf_0x80(path, 0, true, 0, &children);
    bool ok = false;
    if (result == 0) {
        ok = true;
        for (UnkListedPath_b0248* n = children.head; n; n = n->next) {
            if (!ok)
                break;
            ok = fn_7100087b0c(self, handler, n, depth - 1, out);
        }
    } else if (result != 1 && self->on_error) {
        self->on_error(self->context, handler, result);
    }
    return ok;
}
