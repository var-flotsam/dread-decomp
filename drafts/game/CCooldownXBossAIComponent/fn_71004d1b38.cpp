// CCooldownXBossAIComponent's unit (tools/tu_map.py unit 754): looks up four anim-tree nodes of the owner by
// fixed keys and sets each one that is a CLayerNode to 1 or 0 by `enable`. Names and layouts are
// placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/animtree/CLayerNode.h"
#include "types.h"

extern __attribute__((visibility("hidden"))) u64 g_unk_71726d8718;
extern __attribute__((visibility("hidden"))) u64 g_unk_71726d8720;
extern __attribute__((visibility("hidden"))) u64 g_unk_71726d8728;
extern __attribute__((visibility("hidden"))) u64 g_unk_71726d8730;

using base::reflection::UnkReflObject;

struct UnkLayerNode;

struct UnkNodeMap_l4d1b38 {
    u8 pad_0x0[0x78];
    UnkReflObject** values;  // 0x78
    u64* keys;               // 0x80
    u32 count;               // 0x88

    UnkReflObject** End() { return &values[count]; }
    UnkReflObject** Find(const u64& key) {
        u64 i = 0;
        for (; i != count; ++i)
            if (keys[i] == key)
                break;
        return &values[i];
    }
};

struct UnkTree_l4d1b38 {
    u8 pad_0x0[0x40];
    UnkNodeMap_l4d1b38* map;  // 0x40
};

struct UnkOwner_l4d1b38 {
    u8 pad_0x0[0x220];
    UnkTree_l4d1b38* tree;  // 0x220
};

struct UnkCooldownAI_l4d1b38 {
    u8 pad_0x0[0x30];
    UnkOwner_l4d1b38* owner;  // 0x30
};

// The anim-tree node with this key, or null when there is none.
static inline UnkReflObject* FindNode(UnkTree_l4d1b38* tree, u64 key) {
    UnkNodeMap_l4d1b38* map = tree->map;
    UnkReflObject** it = map->Find(key);
    if (it == map->End())
        return nullptr;
    UnkReflObject* node = *it;
    if (node && !fn_710009d50c(node->vf_0x0(), fn_71002df4ec()))
        return nullptr;
    return node;
}

static inline void SetLayer(UnkReflObject* node, bool enable) {
    if (node && fn_710009d50c(node->vf_0x0(), fn_71002e43a4()))
        fn_71002e465c(reinterpret_cast<UnkLayerNode*>(node), enable ? 1.0f : 0.0f);
}

// Declared as CCooldownXBossAIComponent_Phases_ovb0088.cpp declares it.
struct UnkCooldownXMsgAI_ovb0088;

void fn_71004d1b38(UnkCooldownXMsgAI_ovb0088* p, bool enable) {
    UnkCooldownAI_l4d1b38* self = reinterpret_cast<UnkCooldownAI_l4d1b38*>(p);
    UnkTree_l4d1b38* tree = self->owner->tree;
    if (!tree)
        return;
    UnkReflObject* first = FindNode(tree, g_unk_71726d8718);
    UnkReflObject* second = FindNode(tree, g_unk_71726d8720);
    UnkReflObject* third = FindNode(tree, g_unk_71726d8728);
    UnkReflObject* fourth = FindNode(tree, g_unk_71726d8730);
    SetLayer(first, enable);
    SetLayer(second, enable);
    SetLayer(third, enable);
    SetLayer(fourth, enable);
}
