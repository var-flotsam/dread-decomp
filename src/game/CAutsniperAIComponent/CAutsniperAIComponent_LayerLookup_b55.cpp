// CAutsniperAIComponent's unit (fn_71003db2c4 and its copy fn_71003db3cc): looks up an anim-tree node by a fixed key in a flat map (parallel
// key and value arrays) and, when it is a CLayerNode, forwards the value to it. Names are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

extern __attribute__((visibility("hidden"))) u64 g_unk_71726bfcb0;

using base::reflection::UnkReflObject;

struct UnkLayerNode;
void fn_71002e465c(UnkLayerNode* self, f32 dt);

struct UnkNodeMapB55 {
    u8 pad_0x0[0x78];
    UnkReflObject** values;  // 0x78
    u64* keys;               // 0x80
    u32 count;               // 0x88
};

struct UnkTreeB55 {
    u8 pad_0x0[0x40];
    UnkNodeMapB55* map;  // 0x40
};

struct UnkOwnerB55 {
    u8 pad_0x0[0x220];
    UnkTreeB55* tree;  // 0x220
};

struct UnkSelfB55 {
    u8 pad_0x0[0x30];
    UnkOwnerB55* owner;  // 0x30
};

static inline UnkReflObject** Find(UnkNodeMapB55* map, const u64& key) {
    for (u64 i = 0; i != map->count; ++i) {
        if (map->keys[i] == key) {
            return &map->values[i];
        }
    }
    return map->values + map->count;
}

static inline __attribute__((always_inline)) void ForwardToLayer(UnkSelfB55* self, f32 value) {
    UnkTreeB55* tree = self->owner->tree;
    if (tree == nullptr) {
        return;
    }
    UnkNodeMapB55* map = tree->map;
    UnkReflObject** it = Find(map, g_unk_71726bfcb0);
    if (it == map->values + map->count) {
        return;
    }
    UnkReflObject* node = *it;
    if (node == nullptr) {
        return;
    }
    if (!fn_710009d50c(node->vf_0x0(), fn_71002df4ec())) {
        return;
    }
    if (!fn_710009d50c(node->vf_0x0(), fn_71002e43a4())) {
        return;
    }
    fn_71002e465c(reinterpret_cast<UnkLayerNode*>(node), value);
}

void fn_71003db2c4(UnkSelfB55* self, f32 value) {
    ForwardToLayer(self, value);
}

// Identical copy of fn_71003db2c4.
void fn_71003db3cc(UnkSelfB55* self, f32 value) {
    ForwardToLayer(self, value);
}
