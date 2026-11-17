// Unit 211 of tools/tu_map.py (no name hint): fn_7100191d40 builds the player for one two-value curve
// description of a track, by the description's kind: a pair, a range, two wrappers and a labelled
// wrapper around a nested description, an eight-value node, an emitter over two value lists
// (fn_710016d59c) or a grid emitter (fn_710016cea4), each with its argument node. Names and layouts
// are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/UnkTU208.h"
#include "types.h"

struct UnkKeyPlayer_b0197;
struct UnkKeyRequest_b0197 {
    u32 kind_0x0;
    u32 mode_0x4;
};
struct UnkValueNode;
struct UnkNodeOwner;
struct UnkGridEmitter_ovb0167;
struct UnkOwner8_ovb0167;

u64 fn_7100191d40(UnkKeyPlayer_b0197* self, const UnkKeyRequest_b0197* request, void* track);
void fn_710016d59c(UnkValueNode* self, UnkNodeOwner* owner, void* arg, u32 countA, const u32* valuesA, u32 countB,
                   const u32* valuesB, f32 scale);
void fn_710016cea4(UnkGridEmitter_ovb0167* self, UnkOwner8_ovb0167* owner, void* arg, u32 cell_count, u32 columns,
                   u32 rows, void* cells);
u32 fn_710016bad0(void* scene);

extern char g_unk_7101a1de08[];
extern char g_unk_7101a1de38[];
extern char g_unk_7101a1de68[];
extern char g_unk_7101a1de98[];
extern char g_unk_7101a1dec8[];
extern char g_unk_7101a1d908[];
extern char g_unk_7101a1dca0[];
extern char g_unk_7101a1dcc8[];
extern char g_unk_7101a1dcf0[];

struct UnkEntry_l191d40 {
    u32 words[2] = {};
};

struct UnkTrack_l191d40 {
    void* vtable;
    void* scene;  // 0x8
    u8 pad_0x10[0x8];
    UnkEntry_l191d40* entries;  // 0x18
};

// The descriptions: a common header whose mode_0x4 is the kind, then the kind's data.
struct UnkDescPair_l191d40 {
    UnkKeyRequest_b0197 header;
    u32 values[2];  // 0x8
};
struct UnkDescRange_l191d40 {
    UnkKeyRequest_b0197 header;
    u32 values[4];  // 0x8
    u8 flag;  // 0x18
};
struct UnkDescWrap_l191d40 {
    UnkKeyRequest_b0197 header;
    UnkKeyRequest_b0197 inner;  // 0x8, the nested description
};
struct UnkDescEight_l191d40 {
    UnkKeyRequest_b0197 header;
    u32 values[8];  // 0x8
};
struct UnkDescLabel_l191d40 {
    UnkKeyRequest_b0197 header;
    u32 label;  // 0x8
    UnkKeyRequest_b0197 inner;  // 0xc, the nested description
};
struct UnkArgValue_l191d40 {
    u32 value;
    u8 flag;
};
// The head the two emitter descriptions share: the argument's kind and value.
struct UnkDescArgHead_l191d40 {
    UnkKeyRequest_b0197 header;
    u32 argKind;  // 0x8
    f32 value;  // 0xc
    const UnkArgValue_l191d40* arg;  // 0x10
};
struct UnkValueList_l191d40 {
    u32 count;
    u32 values[1];
};
struct UnkValueLists_l191d40 {
    const UnkValueList_l191d40* a;
    const UnkValueList_l191d40* const* b;
};
struct UnkDescLists_l191d40 {
    UnkDescArgHead_l191d40 head;
    const UnkValueLists_l191d40* lists;  // 0x18
};
struct UnkDescGrid_l191d40 {
    UnkDescArgHead_l191d40 head;
    u32 columns;  // 0x18
    u32 rows;  // 0x1c
    u8 cells[1];  // 0x20
};

// The players.
struct UnkNode_l191d40 {
    char* vtable;
    UnkTrack_l191d40* track;  // 0x8
};
struct UnkNodePair_l191d40 : UnkNode_l191d40 {
    u32 values[2];  // 0x10
};
struct UnkNodeRange_l191d40 : UnkNode_l191d40 {
    u32 values[4];  // 0x10
    u8 flag;  // 0x20
};
struct UnkNodeWrap_l191d40 : UnkNode_l191d40 {
    u64 inner;  // 0x10
};
struct UnkNodeEight_l191d40 : UnkNode_l191d40 {
    u32 values[8];  // 0x10
};
struct UnkNodeLabel_l191d40 : UnkNode_l191d40 {
    u64 inner;  // 0x10
    u32 label;  // 0x18
};
struct UnkArg_l191d40 {
    char* vtable;
};
struct UnkArgValueNode_l191d40 : UnkArg_l191d40 {
    u32 value;  // 0x8
    u8 flag;  // 0xc
};

template <class T>
static inline T* Alloc() {
    return static_cast<T*>(fn_71000839a0(sizeof(T), 0x1a));
}

// The base player: the track, whose zeroed entry buffer is made on first use.
static inline void InitNode(UnkNode_l191d40* node, UnkTrack_l191d40* track) {
    node->vtable = g_unk_7101a1db48 + 0x10;
    node->track = track;
    if (!track->entries) {
        u32 n = fn_710016bad0(track->scene);
        UnkEntry_l191d40* p = static_cast<UnkEntry_l191d40*>(fn_7100083ba0((u64)n * sizeof(UnkEntry_l191d40), 0x1a));
        for (u32 i = 0; i < n; i++)
            new (&p[i]) UnkEntry_l191d40;
        track->entries = p;
    }
}

// The argument node an emitter takes, by the description's argument kind.
static inline UnkArg_l191d40* MakeArg(const UnkDescArgHead_l191d40* desc) {
    switch (desc->argKind) {
    case 0: {
        UnkArg_l191d40* arg = Alloc<UnkArg_l191d40>();
        arg->vtable = g_unk_7101a1dca0 + 0x10;
        return arg;
    }
    case 1: {
        UnkArg_l191d40* arg = Alloc<UnkArg_l191d40>();
        arg->vtable = g_unk_7101a1dcc8 + 0x10;
        return arg;
    }
    case 2: {
        const UnkArgValue_l191d40* value = desc->arg;
        UnkArgValueNode_l191d40* node = Alloc<UnkArgValueNode_l191d40>();
        u32 v = value->value;
        u8 flag = value->flag;
        node->vtable = g_unk_7101a1dcf0 + 0x10;
        node->value = v;
        node->flag = flag;
        return node;
    }
    }
    return nullptr;
}

u64 fn_7100191d40(UnkKeyPlayer_b0197* self, const UnkKeyRequest_b0197* request, void* trackArg) {
    UnkTrack_l191d40* track = static_cast<UnkTrack_l191d40*>(trackArg);
    switch (request->mode_0x4) {
    case 0: {
        const UnkDescPair_l191d40* desc = reinterpret_cast<const UnkDescPair_l191d40*>(request);
        UnkNodePair_l191d40* node = Alloc<UnkNodePair_l191d40>();
        InitNode(node, track);
        node->vtable = g_unk_7101a1de08 + 0x10;
        node->values[0] = desc->values[0];
        node->values[1] = desc->values[1];
        return reinterpret_cast<u64>(node);
    }
    case 1: {
        const UnkDescRange_l191d40* desc = reinterpret_cast<const UnkDescRange_l191d40*>(request);
        UnkNodeRange_l191d40* node = Alloc<UnkNodeRange_l191d40>();
        u8 flag = desc->flag;
        InitNode(node, track);
        node->vtable = g_unk_7101a1de38 + 0x10;
        node->values[0] = desc->values[0];
        node->values[1] = desc->values[1];
        node->values[2] = desc->values[2];
        node->values[3] = desc->values[3];
        node->flag = flag;
        return reinterpret_cast<u64>(node);
    }
    case 2: {
        const UnkDescWrap_l191d40* desc = reinterpret_cast<const UnkDescWrap_l191d40*>(request);
        u64 inner = fn_7100191d40(self, &desc->inner, track);
        UnkNodeWrap_l191d40* node = Alloc<UnkNodeWrap_l191d40>();
        InitNode(node, track);
        node->vtable = g_unk_7101a1de68 + 0x10;
        node->inner = inner;
        return reinterpret_cast<u64>(node);
    }
    case 3: {
        const UnkDescWrap_l191d40* desc = reinterpret_cast<const UnkDescWrap_l191d40*>(request);
        u64 inner = fn_7100191d40(self, &desc->inner, track);
        UnkNodeWrap_l191d40* node = Alloc<UnkNodeWrap_l191d40>();
        InitNode(node, track);
        node->vtable = g_unk_7101a1de98 + 0x10;
        node->inner = inner;
        return reinterpret_cast<u64>(node);
    }
    case 4: {
        const UnkDescLists_l191d40* desc = reinterpret_cast<const UnkDescLists_l191d40*>(request);
        UnkArg_l191d40* arg = MakeArg(&desc->head);
        const UnkValueLists_l191d40* lists = desc->lists;
        const UnkValueList_l191d40* a = lists->a;
        const UnkValueList_l191d40* b = *lists->b;
        UnkValueNode* emitter = static_cast<UnkValueNode*>(fn_71000839a0(0x58, 0x1a));
        f32 scale = desc->head.value;
        fn_710016d59c(emitter, reinterpret_cast<UnkNodeOwner*>(track), arg, a->count, a->values, b->count, b->values,
                      scale);
        return reinterpret_cast<u64>(emitter);
    }
    case 5: {
        const UnkDescEight_l191d40* desc = reinterpret_cast<const UnkDescEight_l191d40*>(request);
        UnkNodeEight_l191d40* node = Alloc<UnkNodeEight_l191d40>();
        u32 v0 = desc->values[0];
        u32 v1 = desc->values[1];
        InitNode(node, track);
        node->vtable = g_unk_7101a1dec8 + 0x10;
        node->values[0] = v0;
        node->values[1] = v1;
        node->values[2] = desc->values[2];
        node->values[3] = desc->values[3];
        node->values[4] = desc->values[4];
        node->values[5] = desc->values[5];
        node->values[6] = desc->values[6];
        node->values[7] = desc->values[7];
        return reinterpret_cast<u64>(node);
    }
    case 6: {
        const UnkDescGrid_l191d40* desc = reinterpret_cast<const UnkDescGrid_l191d40*>(request);
        UnkArg_l191d40* arg = MakeArg(&desc->head);
        UnkGridEmitter_ovb0167* emitter = static_cast<UnkGridEmitter_ovb0167*>(fn_71000839a0(0x30, 0x1a));
        fn_710016cea4(emitter, reinterpret_cast<UnkOwner8_ovb0167*>(track), arg, (s32)(desc->head.value + 0.5f),
                      desc->columns, desc->rows, const_cast<u8*>(desc->cells));
        return reinterpret_cast<u64>(emitter);
    }
    case 13: {
        const UnkDescLabel_l191d40* desc = reinterpret_cast<const UnkDescLabel_l191d40*>(request);
        u64 inner = fn_7100191d40(self, &desc->inner, track);
        UnkNodeLabel_l191d40* node = Alloc<UnkNodeLabel_l191d40>();
        u32 label = desc->label;
        InitNode(node, track);
        node->vtable = g_unk_7101a1d908 + 0x10;
        node->inner = inner;
        node->label = label;
        return reinterpret_cast<u64>(node);
    }
    }
    return 0;
}
