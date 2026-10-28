// Translation unit 2661 of data/tu_map.csv (0x71010626f0-0x7101073b80), named after the
// CCutscene::CTunableCutscene reflection data it registers. Holds the cutscene's screen fade
// (fn_710106b2bc): the CFadeScreenTrack event playing at the current frame tints the fade layer
// of the global GUI object; and the choice of a node name two objects share under their
// "DC_Root" nodes (fn_7101065450). Names and layouts are placeholders inferred from the code.
#include <algorithm>

#include "base/UnkVector.h"
#include "base/color/CColor4F.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflStatics.h"
#include "engine/scene/UnkToggle.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

extern const f32 g_unk_710164c384[/* 256; unsized: x86-64 would assume 16-byte alignment */];  // byte -> linear colour component

// A colour packed as 0xAARRGGBB.
struct UnkColor32 {
    u8 b, g, r, a;
};

// A packed colour's alpha scaled to [0, 1].
static inline f32 UnkPackedAlpha(u8 c) {
    f32 alpha = (f32)c / 255.0f;
    return alpha <= 0.0f ? 0.0f : alpha;
}

// A packed colour component through the linearisation table.
static inline f32 UnkPackedLinear(u8 c) {
    f32 v = g_unk_710164c384[c];
    return c ? v : 0.0f;
}

// The colour of four floats (base::color::CColor4F) made from a packed one.
static inline base::color::CColor4F UnkColorFromPacked(const UnkColor32& c) {
    return base::color::CColor4F(UnkPackedLinear(c.r), UnkPackedLinear(c.g), UnkPackedLinear(c.b),
                                 UnkPackedAlpha(c.a));
}

// A timeline event (base::global::timeline): its class, then the frames it spans.
struct UnkTimelineEvent {
    void** vtable;
    const base::reflection::CClass* cls;
    f32 start;
    f32 end;
};

// A CFadeScreenTrack event: the colours the fade goes from and to over the event, packed as
// 0xAARRGGBB.
struct UnkFadeScreenEvent : UnkTimelineEvent {
    UnkColor32 from;
    UnkColor32 to;
};
static_assert(__builtin_offsetof(UnkFadeScreenEvent, from) == 0x18, "UnkFadeScreenEvent placeholder layout");

struct UnkTimelineEventList {
    UnkTimelineEvent** data;
    u32 count;
};

// What a cutscene track's action holds (fn_710070d4e8): its events and frame count.
struct UnkCutsceneTimeline {
    u8 pad_0x0[0x24];
    s32 frame_count;
    u8 pad_0x28[0x148];
    UnkTimelineEventList events;
};
static_assert(__builtin_offsetof(UnkCutsceneTimeline, events) == 0x170, "UnkCutsceneTimeline placeholder layout");

// The GUI object's fade layer (the item at +0x178 of the root's +0x108) and its colour setter.
struct UnkFadeLayer;
struct UnkFadeLayerHolder {
    u8 pad_0x0[0x78];
    UnkFadeLayer* layer;
};
struct UnkGuiScreen {
    u8 pad_0x0[0x18];
    UnkToggle fade;  // shown while a fade event plays
    u8 pad_0x128[0x50];
    UnkFadeLayerHolder* fade_holder;
};
static_assert(__builtin_offsetof(UnkGuiScreen, fade_holder) == 0x178, "UnkGuiScreen placeholder layout");
struct UnkGuiRoot {
    u8 pad_0x0[0x108];
    UnkGuiScreen* screen;
};

struct UnkCutscene {
    u8 pad_0x0[0x68];
    s32 field_0x68;
    u8 pad_0x6c[0xac];
    f32 field_0x118;
    u8 pad_0x11c[0x64];
    UnkTrack track;  // the cutscene's track; its action holds the timeline
    u8 pad_0x1b0[0x308];
    s32 field_0x4b8;
    f32 field_0x4bc;
};
static_assert(__builtin_offsetof(UnkCutscene, track) == 0x180, "UnkCutscene placeholder layout");
static_assert(__builtin_offsetof(UnkCutscene, field_0x4b8) == 0x4b8, "UnkCutscene placeholder layout");

extern void* g_unk_7101d47330;

UnkCutsceneTimeline* fn_710070d4e8(UnkTrack* track);
UnkTimelineEvent* fn_710006bb80(f32 frame, UnkTimelineEventList* events, const base::reflection::CClass* cls,
                                s32 frame_count);
void fn_710019c4fc(UnkFadeLayer* layer, const base::color::CColor4F* color);

// Shows the fade layer and sets its colour while a fade event spans the current position, hides
// it otherwise. At the last frame (within 1e-5) the position steps back by one frame length.
// NON_MATCHING(minor): instruction scheduling and register allocation in the colour conversion
// (the same instructions, the alpha divide issued later); everything else is identical.
void fn_710106b2bc(UnkCutscene* self, f32 frame) {
    if (!fn_710070d500(&self->track))
        return;
    f32 pos = self->field_0x4bc * (f32)self->field_0x68;
    f32 last = self->field_0x118 * (f32)self->field_0x4b8;
    bool not_after = frame - 1e-05f <= last;
    bool not_before = frame + 1e-05f >= last;
    pos = not_before & not_after ? pos - self->field_0x118 : pos;
    UnkCutsceneTimeline* timeline = fn_710070d4e8(&self->track);
    const base::reflection::CClass* cls = fn_7100c6595c();
    UnkFadeScreenEvent* ev =
        static_cast<UnkFadeScreenEvent*>(fn_710006bb80(pos, &timeline->events, cls, timeline->frame_count));
    UnkGuiRoot* root = static_cast<UnkGuiRoot*>(g_unk_7101d47330);
    if (!ev) {
        fn_710020ac34(&root->screen->fade, false);
        return;
    }
    fn_710020ac34(&root->screen->fade, true);
    base::color::CColor4F color = fn_710003088c((pos - ev->start) / (ev->end - ev->start),
                                                UnkColorFromPacked(ev->from), UnkColorFromPacked(ev->to));
    fn_710019c4fc(static_cast<UnkGuiRoot*>(g_unk_7101d47330)->screen->fade_holder->layer, &color);
}

// A node of an object's named tree (0x98 bytes): its payload, name and child nodes.
struct UnkNamedNode {
    u8 pad_0x0[0x60];
    void* payload;
    base::global::CStrId name;
    base::UnkVector<UnkNamedNode*> children;
};
static_assert(sizeof(UnkNamedNode) == 0x98, "UnkNamedNode placeholder layout");

// The nodes of a tree by the key (CRC-64) of their names.
struct UnkNamedNodeMap {
    u8 pad_0x0[8];
    UnkNamedNode* values;
    u64* keys;
    u32 count;

    UnkNamedNode* End() { return &values[count]; }
    UnkNamedNode* Find(u64 key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return End();
    }
    UnkNamedNode* Get(const base::global::CStrId& name) {
        UnkNamedNode* node = Find(UnkKeyOf(name));
        if (node == End())
            return nullptr;
        return node;
    }

    // The CRC-64 of an id's string (the key of the CRntString at +0x10 of its data).
    static u64 UnkKeyOf(const base::global::CStrId& id) {
        return *reinterpret_cast<const u64*>(reinterpret_cast<const u8*>(id.data) + 0x30);
    }
};

struct UnkNamedTreeOwner3 {
    u8 pad_0x0[0x80];
    UnkNamedNodeMap* nodes;
};
struct UnkNamedTreeOwner2 {
    u8 pad_0x0[0x78];
    UnkNamedTreeOwner3* field_0x78;
};
struct UnkNamedTreeOwner1 {
    u8 pad_0x0[0x138];
    UnkNamedTreeOwner2* field_0x138;
};
struct UnkNamedTreeOwner {
    u8 pad_0x0[0x50];
    UnkNamedTreeOwner1* field_0x50;
};

// The order the children are sorted in: fn_7101071bc0 is libc++'s __stable_sort instantiated for
// it (declared below, so that std::stable_sort calls it out of line).
struct UnkNamedNodeOrder {
    bool operator()(const UnkNamedNode* a, const UnkNamedNode* b) const;
};

namespace std {
inline namespace __1 {
template <>
void __stable_sort<UnkNamedNodeOrder&, UnkNamedNode**>(UnkNamedNode** first, UnkNamedNode** last,
                                                       UnkNamedNodeOrder& order, ptrdiff_t len, UnkNamedNode** buf,
                                                       ptrdiff_t buf_size) asm("fn_7101071bc0");
}
}  // namespace std

// The child at `index`, and the first node at or under `node` with a payload (or null).
UnkNamedNode* fn_7100195304(UnkNamedNode* node, u32 index);
UnkNamedNode* fn_7100195310(UnkNamedNode* node);

// A copy of a node array: the same capacity from the same allocator.
struct UnkNodeArrayCopy {
    UnkNamedNode** data;
    u32 count;
    base::UnkAllocator* allocator;

    explicit UnkNodeArrayCopy(const base::UnkVector<UnkNamedNode*>& other) {
        allocator = other.allocator;
        data = other.capacity ? static_cast<UnkNamedNode**>(
                                    allocator->vf_0x40((u64)other.capacity * sizeof(UnkNamedNode*), 8, other.flags))
                              : nullptr;
        __builtin_memcpy(data, other.data, (u64)other.count * sizeof(UnkNamedNode*));
        count = other.count;
    }
    UnkNamedNode** begin() { return data; }
    UnkNamedNode** end() { return data + count; }
    ~UnkNodeArrayCopy() {
        if (data)
            allocator->vf_0x48(data);
    }
};

static inline const base::global::CStrId& UnkDcRoot() {
    return *reinterpret_cast<const base::global::CStrId*>(
        reinterpret_cast<const char*>(&base::reflection::refl_GetSingleton()) + 0xd40);  // "DC_Root"
}

// The name of a node under `a`'s "DC_Root" that `b`'s tree also has: among the root's children
// (a sorted copy), the first with children and a payload, else the first payload node under each
// child. Failing that, the root's first child if `b` has it, or `b`'s own "DC_Root" when the root
// has no children (the root is read even when `a` has none, as in the game). The empty id without
// both trees.
// NON_MATCHING(minor): register allocation and block order: the original keeps the address of the
// root's child count in a register across the copy (0x78 on the no-root path) and spills the
// copy's allocator; the instructions are otherwise the same.
base::global::CStrId fn_7101065450(UnkNamedTreeOwner* a, UnkNamedTreeOwner* b) {
    UnkNamedTreeOwner1* owner_a = a->field_0x50;
    UnkNamedTreeOwner1* owner_b = b->field_0x50;
    if (owner_a && owner_b) {
        UnkNamedNodeMap* nodes_a = owner_a->field_0x138->field_0x78->nodes;
        UnkNamedNodeMap* nodes_b = owner_b->field_0x138->field_0x78->nodes;
        if (nodes_a && nodes_b) {
            UnkNamedNode* root = nodes_a->Get(UnkDcRoot());
            if (root) {
                UnkNodeArrayCopy children(root->children);
                std::stable_sort(children.begin(), children.end(), UnkNamedNodeOrder());
                if (children.count) {
                    UnkNamedNode** p = children.begin();
                    do {
                        UnkNamedNode* child = *p;
                        if (child->children.count && child->payload && nodes_b->Get(child->name))
                            return child->name;
                    } while (++p != children.end());
                }
                if (children.count) {
                    UnkNamedNode** p = children.begin();
                    do {
                        UnkNamedNode* leaf = fn_7100195310(*p);
                        if (leaf && nodes_b->Get(leaf->name))
                            return leaf->name;
                    } while (++p != children.end());
                }
            }
            if (root->children.count == 0)
                return nodes_b->Get(UnkDcRoot())->name;
            if (nodes_b->Get(fn_7100195304(root, 0)->name))
                return fn_7100195304(root, 0)->name;
        }
    }
    return g_unk_7101d470e0;
}
