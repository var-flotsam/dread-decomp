// The CKraidAIComponent unit. fn_71005fc978, the event handler: for an event of the usual track kind whose
// id hash is 0xdd97b56941457ece, sets the weight of the anim tree's (owner +0x220) layer node
// g_unk_71726f3798 to 0 (kind 1) or to 1 and sends the tree a fixed request with mode 1 (kind 3). Always
// returns true. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/animtree/CLayerNode.h"
#include "types.h"

using base::reflection::UnkReflObject;

extern __attribute__((visibility("hidden"))) u64 g_unk_71726f3798;  // the layer node's key

struct UnkKraidNodeMap_l5fc978 {
    u8 pad_0x0[0x78];
    UnkReflObject** values;  // 0x78
    u64* keys;               // 0x80
    u32 count;               // 0x88

    UnkReflObject** Find(const u64& key) {
        for (u64 i = 0; i != count; ++i) {
            if (keys[i] == key)
                return &values[i];
        }
        return values + count;
    }
};

struct UnkKraidTree_l5fc978 {
    u8 pad_0x0[0x40];
    UnkKraidNodeMap_l5fc978* map;  // 0x40
};

struct UnkKraidOwner_l5fc978 {
    u8 pad_0x0[0x220];
    UnkKraidTree_l5fc978* tree;  // 0x220
};

struct UnkKraidAI_l5fc978 {
    u8 pad_0x0[0x30];
    UnkKraidOwner_l5fc978* owner;
};

struct UnkKraidIdNode_l5fc978 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkKraidIdHolder_l5fc978 {
    u8 pad_0x0[0x38];
    UnkKraidIdNode_l5fc978* node;
};

struct UnkKraidIdRef_l5fc978 {
    u8 pad_0x0[0x10];
    UnkKraidIdHolder_l5fc978* holder;
};

struct UnkKraidEvent_l5fc978 {
    s32 kind;
    u8 pad_0x4[4];
    UnkKraidIdRef_l5fc978* ref;
    UnkKraidIdNode_l5fc978* track;
};

// The layer node under g_unk_71726f3798 in the tree, when it is a CLayerNode.
static inline UnkLayerNode* FindLayer_l5fc978(UnkKraidTree_l5fc978* tree) {
    UnkKraidNodeMap_l5fc978* map = tree->map;
    UnkReflObject** it = map->Find(g_unk_71726f3798);
    UnkReflObject* layer = nullptr;
    if (it != map->values + map->count) {
        if (UnkReflObject* node = *it)
            layer = fn_710009d50c(node->vf_0x0(), fn_71002e43a4()) ? node : nullptr;
    }
    return reinterpret_cast<UnkLayerNode*>(layer);
}

bool fn_71005fc978(UnkKraidAI_l5fc978* self, UnkKraidEvent_l5fc978* event) {
    UnkKraidTree_l5fc978* tree = self->owner->tree;
    if (!tree || event->track->hash != 0xf4061ff25a2163d6)
        return true;
    if (event->ref->holder->node->hash != 0xdd97b56941457ece)
        return true;
    if (event->kind == 3) {
        fn_71002e465c(FindLayer_l5fc978(tree), 1.0f);
        base::reflection::UnkReflTypeIdArg mode;
        mode.id = 1;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        CAnimationComponent_SendActionToTrack(tree, 0x05f77053e2c8495c, 0xb2a7539239c3aad5, &mode, &g_unk_710164b958, &g_unk_7172769740,
                      &mask);
    } else if (event->kind == 1) {
        fn_71002e465c(FindLayer_l5fc978(tree), 0.0f);
    }
    return true;
}
