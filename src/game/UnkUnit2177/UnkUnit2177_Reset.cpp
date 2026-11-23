// Unit 2177 (tools/tu_map.py): hands `arg` to the collision manager (created on first use,
// fn_7100d476e4), releases every node of the list at +0x10 (fn_7100d62bd8, then the owner's +0x20
// through its vtable slot 0x48; a node's next link is at +0x120), empties the list and destroys the
// elements of the global array g_unk_7172791e48. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkElem188.h"
#include "game/UnkTU2177.h"
#include "game/UnkTU2506.h"
#include "game/UnkTU365.h"
#include "game/logic/collision/CPolygonCollectionShape.h"
#include "types.h"

// The collision manager singleton (src/game/CCollisionComponent/fn_7100863a8c.cpp).
struct UnkManager_7100863a8c;

struct UnkUnit2177Node {
    u8 pad_0x0[0x120];
    UnkUnit2177Node* next;  // 0x120
};
struct UnkUnit2177Sink {
    void** vtable;
    void vf_0x48(UnkUnit2177Node* node) {
        reinterpret_cast<void (*)(UnkUnit2177Sink*, UnkUnit2177Node*)>(vtable[0x48 / 8])(this, node);
    }
};
struct UnkUnit2177 {
    u8 pad_0x0[0x8];
    u32 count;  // 0x8
    u8 pad_0xc[0x4];
    UnkUnit2177Node* head;  // 0x10
    UnkUnit2177Node* tail;  // 0x18
    UnkUnit2177Sink* sink;  // 0x20
};

static inline UnkManager_7100863a8c* CollisionManager() {
    if (!g_unk_71726a4788) {
        UnkManager_7100863a8c* m = static_cast<UnkManager_7100863a8c*>(fn_71000839a0(0x6b0, 0x3f));
        fn_7100d46804(m);
        g_unk_71726a4788 = m;
    }
    return g_unk_71726a4788;
}

void fn_7100c72140(UnkUnit2177* self, void* arg) {
    fn_7100d476e4(CollisionManager(), arg);
    for (UnkUnit2177Node* n = self->head; n;) {
        UnkUnit2177Node* next = n->next;
        fn_7100d62bd8(n);
        self->sink->vf_0x48(n);
        n = next;
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    g_unk_7172791e48.Clear();
}
