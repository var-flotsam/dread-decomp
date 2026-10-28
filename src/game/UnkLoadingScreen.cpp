// From unit 2557 of tools/tu_map.py (0x7100e0f028-0x7100e140c0, no reflected class; it reflects the
// ELoadingScreen enum and names "LoadingScreen", "LoadinScreenTip%d" and the loading animations):
// the loading screen. Names and layouts are placeholders.
// Holds fn_7100e10aac (closes the loading screen).
#include "base/UnkVector.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkRenderManager.h"
#include "game/sound/CMusicManager.h"
#include "types.h"

// A fade colour (the same definition as src/game/UnkScreenFade.cpp's).
struct UnkFadeColor {
    f32 r;
    f32 g;
    f32 b;
    f32 a;

    UnkFadeColor(f32 r_, f32 g_, f32 b_, f32 a_) : r(r_), g(g_), b(b_), a(a_) {}
};

// A bit mask passed by value (copied into a temporary), and its two guarded constants.
struct UnkLoadingMask {
    u32 bits;
    UnkLoadingMask() : bits(0) {}
    UnkLoadingMask(u32 b) : bits(b) {}
    UnkLoadingMask(const UnkLoadingMask& o) : bits(o.bits) {}
};
template <class T>
struct UnkLoadingMaskOf {
    static const UnkLoadingMask& None() {
        static UnkLoadingMask s;
        return s;
    }
    static const UnkLoadingMask& All() {
        static UnkLoadingMask s(~None().bits & 0x1ff);
        return s;
    }
};
struct UnkLoadingMaskTag;

// The object behind +0x20 whose node at +0x18 shows the fade.
struct UnkLoadingFade {
    u8 unk_0x0[0x18];
    u8 node[1];
};

struct UnkLoadingScreen {
    s32 field_0x0;
    u8 unk_0x4[0x14];
    void* field_0x18;  // a render object handed back to the render manager (pool 3)
    UnkLoadingFade* field_0x20;
    s32 field_0x28;  // the scene state; 0x50 once restored
    u8 unk_0x2c[0x1bc];
    bool field_0x1e8;
};
static_assert(__builtin_offsetof(UnkLoadingScreen, field_0x1e8) == 0x1e8, "UnkLoadingScreen placeholder layout");

// The scene-state singleton (0x150 bytes, created on first use; the type is src/game_hi's
// UnkStruct_0x128).
struct UnkStruct_0x128;
extern UnkStruct_0x128* g_unk_7101d47320;
void fn_7100ef3494(UnkStruct_0x128* self);
void fn_7100f011c4(UnkStruct_0x128* self, u32 id);
void fn_7100f00714(UnkStruct_0x128* self);
static inline UnkStruct_0x128* SceneState() {
    UnkStruct_0x128* s = g_unk_7101d47320;
    if (!s) {
        s = static_cast<UnkStruct_0x128*>(fn_71000839a0(0x150, 0x3f));
        fn_7100ef3494(s);
        g_unk_7101d47320 = s;
    }
    return s;
}

// The music manager (game/sound/CMusicManager.h), created on first use.
extern void* g_unk_71726bc560;
void fn_7100fd7b44(void* manager, UnkLoadingMask channels, UnkLoadingMask mask, f32 time);
void fn_7100fdca28(void* manager, u32 value);
static inline void* MusicManager() {
    void* manager = g_unk_71726bc560;
    if (!manager) {
        manager = fn_71000839a0(0xf18, 0x3f);
        fn_7100fd3804(manager);
        g_unk_71726bc560 = manager;
    }
    return manager;
}

struct UnkItemBase_71011a91c4;
bool fn_71002199d8(UnkRenderManager* self, UnkItemBase_71011a91c4* item, u32 pool);
void fn_71002199b0(UnkRenderManager* self);
void fn_71002199b4(UnkRenderManager* self);
void fn_7100219974(UnkRenderManager* self, u32 value);
void* fn_71000810b0(u32 value);
void fn_7100209968(void* node, f32 duration, const UnkFadeColor* to, const UnkFadeColor* from);
void fn_7100209fec(void* node, bool visible);
void fn_7100e13238(UnkLoadingScreen* self);
void fn_71011b6b5c(UnkGuiRoot* root, u32 value);
extern void* g_unk_71727c0708;  // a render object the loading screen keeps (pool 0)

// Closes the loading screen: the fade node goes from black to clear over 0.3, then is hidden; the
// render objects go back to the render manager, the scene state is restored once (state 0x50) with
// the music brought back on all channels, and the music and the GUI root are reset.
// NON_MATCHING(minor): scheduling only: the fade's 0.3 duration is materialised after the second
// colour's constant and store instead of before them (its constant then lands in x8, not x9).
void fn_7100e10aac(UnkLoadingScreen* self) {
    fn_71002199b0(g_unk_7101d47300);
    void* node = self->field_0x20->node;
    UnkFadeColor to(0.0f, 0.0f, 0.0f, 0.0f);
    UnkFadeColor from(0.0f, 0.0f, 0.0f, 1.0f);
    fn_7100209968(node, 0.3f, &to, &from);
    fn_71002199b4(g_unk_7101d47300);
    fn_71000810b0(1000);
    fn_7100219974(g_unk_7101d47300, 0);
    fn_7100209fec(self->field_0x20->node, false);
    self->field_0x1e8 = false;
    fn_7100e13238(self);
    if (g_unk_71727c0708) {
        fn_71002199d8(g_unk_7101d47300, static_cast<UnkItemBase_71011a91c4*>(g_unk_71727c0708), 0);
        g_unk_71727c0708 = nullptr;
    }
    if (self->field_0x28 != 0x50) {
        fn_7100f011c4(SceneState(), self->field_0x28);
        fn_7100f00714(SceneState());
        self->field_0x28 = 0x50;
        fn_7100fd7b44(MusicManager(), UnkLoadingMask(4), UnkLoadingMaskOf<UnkLoadingMaskTag>::All(), 0.2f);
    }
    if (self->field_0x18) {
        fn_71002199d8(g_unk_7101d47300, static_cast<UnkItemBase_71011a91c4*>(self->field_0x18), 3);
        self->field_0x18 = nullptr;
    }
    fn_7100fd7b44(MusicManager(), UnkLoadingMask(10), UnkLoadingMask(0x100), 0.2f);
    self->field_0x0 = 0;
    fn_7100fdca28(MusicManager(), 0);
    fn_71011b6b5c(g_unk_717271e8f8, 0);
}

// Creating a scene object and keeping it in a set (fn_7100e13928).
#include "base/UnkCountedResource.h"
#include "base/global/CStrId.h"

using base::global::CStrId;

// The created object (0x210 bytes), seen through the members used here; the set keys it by its +8
// (src/game/CCameraComponent/fn_71010e2278.cpp's UnkSceneObj_71010e2278 is the same object).
struct UnkSceneObj_71010e2278;
struct UnkCullView;
// A counted reference's assignment (the old object is reloaded after the new one's increment).
static inline void AssignCounted(UnkCountedResource*& self, UnkCountedResource* other) {
    if (self != other) {
        if (other)
            ++other->ref_count;
        if (UnkCountedResource* old = self) {
            if (--old->ref_count == 0)
                fn_710001737c(old);
        }
        self = other;
    }
}

// The object's part at +0x20: a polymorphic object showing a model.
struct UnkSceneObjPart {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();  // the model changed

    u8 pad_0x8[0x70];
    UnkCountedResource* model;  // +0x78

    void SetModel(UnkCountedResource* m) {
        AssignCounted(model, m);
        vf_0x90();
    }
};
static_assert(sizeof(UnkSceneObjPart) == 0x80, "UnkSceneObjPart placeholder layout");
struct UnkCreatedSceneObj {
    u8 pad_0x0[0x8];
    u8 key[0x8];  // +0x8: what the set keys
    UnkCountedResource* field_0x10;
    u8 pad_0x18[0x8];
    UnkSceneObjPart part;  // +0x20
    u8 pad_0xa0[0x88];
    UnkCullView* field_0x128;
};
static_assert(__builtin_offsetof(UnkCreatedSceneObj, field_0x128) == 0x128, "UnkCreatedSceneObj placeholder layout");
void fn_710014520c(UnkCreatedSceneObj* self, const CStrId* name);  // the constructor
void fn_71001b7a60(UnkCullView* self);

// The set: buckets of nodes by key, the nodes also in a list in insertion order.
struct UnkSceneObjNode {
    void* key;
    UnkCreatedSceneObj* value;
    UnkSceneObjNode* next;
    UnkSceneObjNode* prev;
    UnkSceneObjNode* bucket_next;
};
struct UnkSceneObjSet {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    UnkSceneObjNode** buckets;
    u32 field_0x18;  // the buckets exist
    u32 capacity;
    u8 pad_0x20[0x18];
    // A union member: the original reloads the tail after storing the new node's link to it.
    union {
        UnkSceneObjNode* tail;
    };
    UnkSceneObjNode* head;
};

// Creates a scene object named `name` showing `model` (when there is one), adds it to `set` and
// stores it in `*out` (when given). The original also returns the object's key (its +8; null when
// the allocation failed), which none of its four callers reads.
// NON_MATCHING(minor): returns nothing, as the only declaration (src/game/CCameraComponent/fn_71010e2278.cpp's)
// has it, so the key is not kept for x0 and the registers around it differ; returning the key as a
// void* makes the function byte-identical.
void fn_7100e13928(void* set_, void* model_, UnkSceneObj_71010e2278** out, const CStrId* name) {
    auto* set = static_cast<UnkSceneObjSet*>(set_);
    auto* model = static_cast<UnkCountedResource*>(model_);
    auto* obj = static_cast<UnkCreatedSceneObj*>(fn_71000839a0(0x210, 0x38));
    fn_710014520c(obj, name);
    if (obj) {
        fn_71001b7a60(obj->field_0x128);
        AssignCounted(obj->field_0x10, model);
        void* key = obj->key;
        if (model)
            obj->part.SetModel(model);
        UnkSceneObjNode** bucket = nullptr;
        if (set->field_0x18) {
            u32 index = (u32)(u64)key % set->capacity;
            bucket = set->buckets + index;
        }
        UnkSceneObjNode* first = *bucket;
        auto* node = static_cast<UnkSceneObjNode*>(set->allocator->vf_0x40(sizeof(UnkSceneObjNode), 8, set->flags));
        if (node) {
            node->key = key;
            node->value = obj;
            node->bucket_next = nullptr;
            node->next = nullptr;
            node->prev = nullptr;
        }
        node->bucket_next = first;
        *bucket = node;
        if (!set->head)
            set->head = node;
        else if (set->tail) {
            node->prev = set->tail;
            set->tail->next = node;
        }
        set->tail = node;
        ++set->count;
    }
    if (out)
        *out = reinterpret_cast<UnkSceneObj_71010e2278*>(obj);
}
