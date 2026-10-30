// The render manager's unit (tools/tu_map.py: unit 315; include/game/UnkRenderManager.h): its
// creation with the second render singleton, the pass-parameter and view accessors that forward
// to the pass list at +0x220, and the sweep that releases the scene nodes whose renderable has
// finished.
#include <new>

#include "base/UnkSList.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "game/UnkRenderManager.h"
#include "types.h"

extern char unk_7101a20260[];  // the render manager's vtable (reached through the GOT)
extern char unk_7101a202c0[];  // its node list's vtable (reached through the GOT)

void fn_7100159900(u32 value);  // empty

namespace {

struct UnkPendingTag;
using UnkPendingList = base::UnkSList<UnkPendingTag>;

// The list at +0x228 (vtable unk_7101a202c0; destroyed by fn_7100218a2c): the scene nodes waiting
// to be released.
struct UnkPendingNodeList : UnkPendingList {
    __attribute__((always_inline)) UnkPendingNodeList() {
        field_0x0 = reinterpret_cast<u64>(unk_7101a202c0 + 0x10);
        count = 0;
        head = nullptr;
        tail = nullptr;
        allocator = fn_7100083990();
        flags = 0x13;
    }
};

// The render manager as its creation lays it out (the 0x270 bytes fn_71002182c0 allocates);
// include/game/UnkRenderManager.h names only some of these members.
struct UnkRenderManagerData {
    char* vtable;
    u8 field_0x8[0x200];
    u32 field_0x208;
    s32 field_0x20c;
    f32 field_0x210;
    u32 pad_0x214;
    void* field_0x218;  // the pool (UnkRenderManager::field_0x218)
    void* field_0x220;  // the pass list
    UnkPendingNodeList field_0x228;
    void* field_0x258;
    s32 field_0x260;
    u32 field_0x264;
    bool field_0x268;
    bool field_0x269;
    bool field_0x26a;

    __attribute__((always_inline)) UnkRenderManagerData()
        : vtable(unk_7101a20260 + 0x10), field_0x8(), field_0x208(0), field_0x20c(-1), field_0x210(1.0f), field_0x218(nullptr),
          field_0x220(nullptr), field_0x258(nullptr), field_0x260(-1), field_0x264(1), field_0x268(false),
          field_0x269(false), field_0x26a(false) {
        fn_7100159900(1);
    }
};
static_assert(__builtin_offsetof(UnkRenderManagerData, field_0x228) == 0x228, "UnkRenderManagerData");
static_assert(sizeof(UnkRenderManagerData) == 0x270, "UnkRenderManagerData");

// The second render singleton (0x1278 bytes, vtable unk_7101a20280): a base built by
// fn_71002236ac and a second vtable at +0x1270.
struct UnkPlayerData {
    char* vtable;
    u8 field_0x8[0x1268];
    char* vtable_0x1270;
};
static_assert(sizeof(UnkPlayerData) == 0x1278, "UnkPlayerData");

}  // namespace

struct UnkPlayer;
extern UnkPlayer* g_unk_716e554378;
extern char unk_7101a20280[];  // reached through the GOT
void fn_71002236ac(UnkPlayer* self);

struct UnkRenderSetup;  // src/engine/HostArguments.cpp
using base::global::CFilePathStrId;
void fn_71002183e0(UnkRenderManager* self, const UnkRenderSetup* setup, const CFilePathStrId* root);

// Creates the render manager when there is none, sets it up from `setup` and the root path, then
// creates the second render singleton.
void fn_71002182c0(const UnkRenderSetup* setup, const CFilePathStrId* root) {
    UnkRenderManager* manager = g_unk_7101d47300;
    if (!manager) {
        manager = reinterpret_cast<UnkRenderManager*>(new (fn_71000839a0(0x270, 0x3f)) UnkRenderManagerData);
        g_unk_7101d47300 = manager;
    }
    fn_71002183e0(manager, setup, root);
    if (!g_unk_716e554378) {
        UnkPlayer* player = static_cast<UnkPlayer*>(fn_71000839a0(0x1278, 0x3f));
        fn_71002236ac(player);
        UnkPlayerData* data = reinterpret_cast<UnkPlayerData*>(player);
        data->vtable = unk_7101a20280 + 0x10;
        data->vtable_0x1270 = unk_7101a20280 + 0x30;
        g_unk_716e554378 = player;
    }
}

// The pass list's forwarders (clones of base::refl_FindByIdCall, src/clones/): they find the
// pass whose id is the given argument and hand it the rest.
void fn_71001fc7bc(void* passes, u64 hash, const f32* value, u32 count);
void fn_71001fd8dc(void* passes, f32* rect, f32* x, f32* y, u32 view);
void fn_71001fd868(void* passes, const f32* rect, u32 view, f32 x, f32 y);

void fn_7100219294(UnkRenderManager* self, u64 hash, const f32* value, u32 count) {
    fn_71001fc7bc(self->field_0x220, hash, value, count);
}

// Reads the view's rectangle and sets it again with the two values (the two words the read also
// gives are dropped).
void fn_7100219e88(UnkRenderManager* self, u32 view, f32 x, f32 y) {
    f32 unused;
    f32 rect[4] = {};
    fn_71001fd8dc(self->field_0x220, rect, &unused, &unused, view);
    fn_71001fd868(self->field_0x220, rect, view, x, y);
}

// The other way round: sets the view's rectangle to `rect`, keeping the two values it had.
void fn_7100219ef4(UnkRenderManager* self, const f32* rect, u32 view) {
    f32 old[4];
    f32 x = 0;
    f32 y = 0;
    __builtin_memset(old, 0, sizeof(old));
    fn_71001fd8dc(self->field_0x220, old, &x, &y, view);
    fn_71001fd868(self->field_0x220, rect, view, x, y);
}

namespace {

// The part of a scene node's payload the release test reads: its flag at +0xfd says the
// renderable is still in use. Each node type keeps it at its own offset (as in fn_7100219ae0).
struct UnkRenderable {
    u8 pad_0x0[0xfd];
    bool field_0xfd;
};

// The payload of each node type, with its renderable at the type's offset.
template <int Type, u32 Offset>
struct UnkRenderPayload {
    u8 pad_0x0[Offset];
    UnkRenderable renderable;
};

template <int Type, u32 Offset>
UnkRenderable* RenderableAt(const UnkSceneNode* node) {
    return &static_cast<UnkRenderPayload<Type, Offset>*>(node->field_0x10)->renderable;
}

UnkRenderable* GetRenderable(const UnkSceneNode* node) {
    UnkRenderable* r = nullptr;
    switch (node->type_at_3()) {
#define C(t, o) case t: r = RenderableAt<t, o>(node); break;
    C(3, 0x20) C(4, 0x20) C(5, 0x28) C(6, 0x28) C(8, 0x18) C(9, 0x18) C(10, 0x18) C(11, 0x18)
    C(12, 0x20) C(13, 0x20) C(14, 0x28) C(15, 0x20) C(16, 0x20) C(17, 0x20) C(18, 0x18)
    C(19, 0x20) C(21, 0x18) C(22, 0x18) C(23, 0x18) C(24, 0x28)
#undef C
    }
    return r;
}

// Unlinks `node` and frees it; returns the node after it, or null when it isn't in the list.
UnkPendingList::Node* Erase(UnkPendingList* list, UnkPendingList::Node* node) {
    UnkPendingList::Node* prev = nullptr;
    UnkPendingList::Node* it = list->head;
    while (it) {
        UnkPendingList::Node* cur = it;
        it = it->next;
        if (cur == node) {
            if (!prev) {
                list->head = it;
            } else {
                prev->next = it;
                if (list->tail == node)
                    list->tail = prev;
            }
            if (--list->count == 0) {
                list->head = nullptr;
                list->tail = nullptr;
            }
            list->allocator->vf_0x48(node);
            return it;
        }
        prev = cur;
    }
    return nullptr;
}

}  // namespace

struct UnkKeyOwner;
bool fn_710014239c(UnkKeyOwner* self, u64 key);
void fn_7100219a34(UnkRenderManager* self, UnkSceneNode* node);

// Releases the pending scene nodes whose renderable is no longer in use: takes each off its layer
// (fn_7100219a34), hands it back to the pool and drops it from the list.
// NON_MATCHING(minor): register allocation only: the next node of the skip path is loaded into x21
// and copied to x20 (the original loads it into x20), and the search loop keeps its cursor in x9
// with one more copy per step (the original reuses x21); 356 bytes instead of 348.
void fn_7100219340(UnkRenderManager* self) {
    UnkRenderManagerData* view = reinterpret_cast<UnkRenderManagerData*>(self);
    UnkPendingList::Node* n = view->field_0x228.head;
    while (n) {
        UnkSceneNode* node = reinterpret_cast<UnkSceneNode*>(n->value);
        if (!GetRenderable(node)->field_0xfd) {
            fn_7100219a34(self, node);
            // The original passes a third argument (0) that fn_710014239c doesn't read.
            reinterpret_cast<bool (*)(void*, UnkSceneNode*, u32)>(fn_710014239c)(view->field_0x218, node, 0);
            n = Erase(&view->field_0x228, n);
        } else {
            n = n->next;
        }
    }
}

// The items' scene nodes taken off their layers (game/UnkRenderManager.h).
bool fn_71001fc69c(void* passes, u32 layer);

bool fn_71002199d8(UnkRenderManager* self, UnkItemBase_71011a91c4* item, u32 layer) {
    UnkSceneNode* node = reinterpret_cast<UnkSceneNode*>(item);
    if (!node || !fn_71001fc69c(self->field_0x220, layer))
        return false;
    fn_7100219a34(self, node);
    // The original passes a third argument (0) that fn_710014239c doesn't read.
    return reinterpret_cast<bool (*)(void*, UnkSceneNode*, u32)>(fn_710014239c)(self->field_0x218, node, 0);
}
