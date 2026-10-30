// Binding the render targets (unit 179 of tools/tu_map.py, part of the graphics layer that
// fn_71001587f0 starts): a set of up to 16 colour targets and a depth target goes to the
// command buffer and is remembered as the current set. The names are placeholders.
#include "types.h"

void* fn_7100153d50();                 // the command buffer
void* fn_71001611e0(void* object);     // a target's NVN texture

// A target: the texture is at +8.
struct UnkBoundTarget {
    u8 pad_0x0[0x8];
    void* field_0x8;
};

// Up to 16 targets, stored 4-byte aligned (a fixed-capacity vector).
typedef UnkBoundTarget* UnkBoundTargetPtr __attribute__((aligned(4)));
struct UnkBoundTargetList {
    u32 count;
    UnkBoundTargetPtr items[16];

    UnkBoundTargetPtr* begin() { return items; }
    // Computed on the address as an integer: the original compares the byte count, not the count,
    // with zero before the first element.
    UnkBoundTargetPtr* end() {
        return reinterpret_cast<UnkBoundTargetPtr*>(reinterpret_cast<u64>(items) + count * sizeof(UnkBoundTargetPtr));
    }

    UnkBoundTargetList& operator=(const UnkBoundTargetList& other) {
        if (this != &other) {
            __builtin_memcpy(items, other.items, (u64)other.count * sizeof(UnkBoundTargetPtr));
            count = other.count;
        }
        return *this;
    }
};

// The colour targets and the depth target (the layout of fn_71001587f0's handle sets).
struct UnkBoundTargetSet {
    UnkBoundTargetList colors;
    UnkBoundTarget* depth;  // +0x88
};
static_assert(sizeof(UnkBoundTargetSet) == 0x90, "UnkBoundTargetSet placeholder layout");

// The current set (reached through the GOT).
extern UnkBoundTargetSet g_unk_716e29d270;
// NVN's command-buffer SetRenderTargets entry point (reached through the GOT).
extern void (*g_unk_716e29ba88)(void* cmd, s32 count, void* const* colors, void* const* color_views,
                                void* depth, void* depth_view);

// The set type the one caller (src/game_lo/fn_71001cdba4.cpp) declares this function with.
struct UnkEntry_71001cdba4;

// Binds `set`'s non-null colour targets and its depth target, and makes it the current set.
void fn_710015c2d0(UnkEntry_71001cdba4* entry) {
    UnkBoundTargetSet* set = reinterpret_cast<UnkBoundTargetSet*>(entry);
    u32 n = 0;
    void* colors[16] = {};
    UnkBoundTargetList& list = set->colors;
    for (UnkBoundTargetPtr* it = list.begin(); it != list.end(); ++it) {
        if (*it)
            colors[n++] = fn_71001611e0((*it)->field_0x8);
    }
    void* depth = set->depth ? fn_71001611e0(set->depth->field_0x8) : nullptr;
    void* cmd = fn_7100153d50();
    g_unk_716e29ba88(cmd, n, colors, nullptr, depth, nullptr);
    g_unk_716e29d270 = *set;
}

// A target's texture (+0x8), or null without a target.
void* fn_710015c450(UnkBoundTarget* target) {
    return target ? target->field_0x8 : nullptr;
}
