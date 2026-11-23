// Unit 211 of tools/tu_map.py (no name hint): fn_710019199c builds the player for one curve
// description of a track, by the description's kind: a constant, a range, two wrappers and a
// labelled wrapper around a nested description, a five-value node, or an emitter over a value list
// (fn_710016d154) with its argument node. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkKeyRequest_b0197.h"
#include "game/UnkTU208.h"
#include "game/UnkTU211.h"
#include "types.h"

struct UnkKeyPlayer_b0197;
struct UnkOwner4_ovb0167;
struct UnkCurveEmitter_ovb0167;

void fn_710016d154(UnkCurveEmitter_ovb0167* self, UnkOwner4_ovb0167* owner, void* arg, u32 count, const f32* values,
                   f32 scale);

extern char g_unk_7101a1dd18[];
extern char g_unk_7101a1dd48[];
extern char g_unk_7101a1dd78[];
extern char g_unk_7101a1dda8[];
extern char g_unk_7101a1ddd8[];
extern char g_unk_7101a1d8d8[];

struct UnkTrack_l19199c {
    void* vtable;
    void* scene;  // 0x8
    u8 pad_0x10[0x8];
    f32* entries;  // 0x18
};

// The descriptions: a common header whose mode_0x4 is the kind, then the kind's data.
struct UnkDescValue_l19199c {
    UnkKeyRequest_b0197 header;
    u32 value;  // 0x8
};
struct UnkDescRange_l19199c {
    UnkKeyRequest_b0197 header;
    u32 first;  // 0x8
    u32 second;  // 0xc
    u8 flag;  // 0x10
};
struct UnkDescWrap_l19199c {
    UnkKeyRequest_b0197 header;
    UnkKeyRequest_b0197 inner;  // 0x8, the nested description
};
struct UnkDescFive_l19199c {
    UnkKeyRequest_b0197 header;
    u32 values[5];  // 0x8
};
struct UnkDescLabel_l19199c {
    UnkKeyRequest_b0197 header;
    u32 label;  // 0x8
    UnkKeyRequest_b0197 inner;  // 0xc, the nested description
};
struct UnkArgValue_l19199c {
    u32 value;
    u8 flag;
};
struct UnkValueList_l19199c {
    u32 count;
    f32 values[1];
};
struct UnkDescEmitter_l19199c {
    UnkKeyRequest_b0197 header;
    u32 argKind;  // 0x8
    f32 scale;  // 0xc
    const UnkArgValue_l19199c* arg;  // 0x10
    UnkValueList_l19199c* const* list;  // 0x18
};

// The players.
struct UnkNode_l19199c {
    char* vtable;
    UnkTrack_l19199c* track;  // 0x8
};
struct UnkNodeValue_l19199c : UnkNode_l19199c {
    u32 value;  // 0x10
};
struct UnkNodeRange_l19199c : UnkNode_l19199c {
    u32 first;  // 0x10
    u32 second;  // 0x14
    u8 flag;  // 0x18
};
struct UnkNodeWrap_l19199c : UnkNode_l19199c {
    u64 inner;  // 0x10
};
struct UnkNodeFive_l19199c : UnkNode_l19199c {
    u32 values[5];  // 0x10
};
struct UnkNodeLabel_l19199c : UnkNode_l19199c {
    u64 inner;  // 0x10
    u32 label;  // 0x18
};
struct UnkArg_l19199c {
    char* vtable;
};
struct UnkArgValueNode_l19199c : UnkArg_l19199c {
    u32 value;  // 0x8
    u8 flag;  // 0xc
};

template <class T>
static inline T* Alloc() {
    return static_cast<T*>(fn_71000839a0(sizeof(T), 0x1a));
}

// The base player: the track, whose entry buffer is made on first use.
static inline void InitNode(UnkNode_l19199c* node, UnkTrack_l19199c* track) {
    node->vtable = g_unk_7101a1db78 + 0x10;
    node->track = track;
    if (!track->entries)
        track->entries = static_cast<f32*>(fn_7100083ba0((u64)fn_710016bad0(track->scene) * sizeof(f32), 0x1a));
}

u64 fn_710019199c(UnkKeyPlayer_b0197* self, const UnkKeyRequest_b0197* request, void* trackArg) {
    UnkTrack_l19199c* track = static_cast<UnkTrack_l19199c*>(trackArg);
    switch (request->mode_0x4) {
    case 0: {
        const UnkDescValue_l19199c* desc = reinterpret_cast<const UnkDescValue_l19199c*>(request);
        UnkNodeValue_l19199c* node = Alloc<UnkNodeValue_l19199c>();
        InitNode(node, track);
        node->vtable = g_unk_7101a1dd18 + 0x10;
        node->value = desc->value;
        return reinterpret_cast<u64>(node);
    }
    case 1: {
        const UnkDescRange_l19199c* desc = reinterpret_cast<const UnkDescRange_l19199c*>(request);
        UnkNodeRange_l19199c* node = Alloc<UnkNodeRange_l19199c>();
        u8 flag = desc->flag;
        InitNode(node, track);
        node->vtable = g_unk_7101a1dd48 + 0x10;
        node->first = desc->first;
        node->second = desc->second;
        node->flag = flag;
        return reinterpret_cast<u64>(node);
    }
    case 2: {
        const UnkDescWrap_l19199c* desc = reinterpret_cast<const UnkDescWrap_l19199c*>(request);
        u64 inner = fn_710019199c(self, &desc->inner, track);
        UnkNodeWrap_l19199c* node = Alloc<UnkNodeWrap_l19199c>();
        InitNode(node, track);
        node->vtable = g_unk_7101a1dd78 + 0x10;
        node->inner = inner;
        return reinterpret_cast<u64>(node);
    }
    case 3: {
        const UnkDescWrap_l19199c* desc = reinterpret_cast<const UnkDescWrap_l19199c*>(request);
        u64 inner = fn_710019199c(self, &desc->inner, track);
        UnkNodeWrap_l19199c* node = Alloc<UnkNodeWrap_l19199c>();
        InitNode(node, track);
        node->vtable = g_unk_7101a1dda8 + 0x10;
        node->inner = inner;
        return reinterpret_cast<u64>(node);
    }
    case 4: {
        const UnkDescEmitter_l19199c* desc = reinterpret_cast<const UnkDescEmitter_l19199c*>(request);
        UnkArg_l19199c* arg;
        switch (desc->argKind) {
        case 0:
            arg = Alloc<UnkArg_l19199c>();
            arg->vtable = g_unk_7101a1dca0 + 0x10;
            break;
        case 1:
            arg = Alloc<UnkArg_l19199c>();
            arg->vtable = g_unk_7101a1dcc8 + 0x10;
            break;
        case 2: {
            const UnkArgValue_l19199c* value = desc->arg;
            UnkArgValueNode_l19199c* node = Alloc<UnkArgValueNode_l19199c>();
            u32 v = value->value;
            u8 flag = value->flag;
            node->vtable = g_unk_7101a1dcf0 + 0x10;
            node->value = v;
            node->flag = flag;
            arg = node;
            break;
        }
        default:
            arg = nullptr;
            break;
        }
        const UnkValueList_l19199c* list = *desc->list;
        UnkCurveEmitter_ovb0167* emitter = static_cast<UnkCurveEmitter_ovb0167*>(fn_71000839a0(0x40, 0x1a));
        f32 scale = desc->scale;
        fn_710016d154(emitter, reinterpret_cast<UnkOwner4_ovb0167*>(track), arg, list->count, list->values, scale);
        return reinterpret_cast<u64>(emitter);
    }
    case 5: {
        const UnkDescFive_l19199c* desc = reinterpret_cast<const UnkDescFive_l19199c*>(request);
        UnkNodeFive_l19199c* node = Alloc<UnkNodeFive_l19199c>();
        u32 v0 = desc->values[0];
        u32 v1 = desc->values[1];
        InitNode(node, track);
        node->vtable = g_unk_7101a1ddd8 + 0x10;
        node->values[0] = v0;
        node->values[1] = v1;
        node->values[2] = desc->values[2];
        node->values[3] = desc->values[3];
        node->values[4] = desc->values[4];
        return reinterpret_cast<u64>(node);
    }
    case 13: {
        const UnkDescLabel_l19199c* desc = reinterpret_cast<const UnkDescLabel_l19199c*>(request);
        u64 inner = fn_710019199c(self, &desc->inner, track);
        UnkNodeLabel_l19199c* node = Alloc<UnkNodeLabel_l19199c>();
        u32 label = desc->label;
        InitNode(node, track);
        node->vtable = g_unk_7101a1d8d8 + 0x10;
        node->inner = inner;
        node->label = label;
        return reinterpret_cast<u64>(node);
    }
    }
    return 0;
}
