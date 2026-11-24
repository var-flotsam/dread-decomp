// CAccessPointComponent's unit (tools/tu_map.py unit 1581): the access point's update. While it waits for the
// GUI (+0x91c) it takes the composition's flag; once that is clear it hands the names kept for the current
// key (the map at +0x5a0) to the manager g_unk_717274ea80 with the callback fn_7100a74650. When the user's
// animation plays the char class's animation it asks the save-station question (callbacks fn_7100a7465c and
// fn_7100a74668); then fn_7100a6fe44 runs, unless there is no composition. Names and layouts are
// placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/GUI/CPopUpComposition.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU83.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

struct UnkAccessPointPair_A6_b62;
struct UnkAp_la6faf0;
void fn_7100a6fe44(UnkAccessPointPair_A6_b62* self);
void fn_7100a74650(UnkAp_la6faf0* self);
void fn_7100a7465c(UnkAp_la6faf0* self);
void fn_7100a74668(UnkAp_la6faf0* self);

// A callback: an object and a function to call on it, taken by value; its copy constructor makes a caller
// pass a copy's address.
struct UnkApCallback_la6faf0 {
    UnkAp_la6faf0* object;
    void (*function)(UnkAp_la6faf0*);
    UnkApCallback_la6faf0(UnkAp_la6faf0* o, void (*f)(UnkAp_la6faf0*)) : object(o), function(f) {}
    UnkApCallback_la6faf0(const UnkApCallback_la6faf0& other) : object(other.object), function(other.function) {}
};

struct UnkGuiComposition_ovb0090;
struct UnkMapCallback_ovb0090;

// Puts a question up on the composition with its accept and back texts and callbacks.
inline void AskQuestion_la6faf0(UnkGuiComposition_ovb0090* gui, const CStrId& text, const CStrId& accept,
                                const CStrId& back, UnkApCallback_la6faf0 on_accept, UnkApCallback_la6faf0 on_back) {
    CPopUpComposition_ShowAcceptBackPrompt(gui, &text, &accept, &back, reinterpret_cast<const UnkMapCallback_ovb0090*>(&on_accept),
                  reinterpret_cast<const UnkMapCallback_ovb0090*>(&on_back), 0, -1.0f);
}

struct UnkApComposition_la6faf0 {
    u8 pad_0x0[0xb4];
    bool busy;  // 0xb4
};

struct UnkGuiRef_la6faf0 {
    u8 pad_0x0[0x10];
    UnkApComposition_la6faf0* composition;  // 0x10
};

// The manager the names go to (0xa0 bytes), created on first use.
extern void* g_unk_717274ea80;
void fn_7100fb790c(void* self);
void fn_7100fb7a1c(f32 a, f32 b, void* manager, const CRntVector<CStrId>* names, u32 c, u32 d, void* e,
                   UnkApCallback_la6faf0 callback);

static inline void* NamesManager() {
    void* manager = g_unk_717274ea80;
    if (!manager) {
        manager = fn_71000839a0(0xa0, 0x3f);
        fn_7100fb790c(manager);
        g_unk_717274ea80 = manager;
    }
    return manager;
}

// The component's tunables: two times at +0x40.
struct UnkApTunable_la6faf0 {
    u8 pad_0x0[0x40];
    f32 field_0x40;
    f32 field_0x44;
};
struct UnkTunable;
extern UnkTunable g_unk_717274ec40;

struct UnkApKey_la6faf0 {
    u8 pad_0x0[0x30];
    u32 id_lo;  // 0x30
    u32 id_hi;  // 0x34
};

struct UnkApMapNode_la6faf0 {
    const UnkApKey_la6faf0* key;
    CRntVector<CStrId> names;  // 0x8
    u8 pad_0x30[0x10];
    UnkApMapNode_la6faf0* next;  // 0x40
};

// Names by key, chained in buckets (base/UnkChainedMap.h's layout).
struct UnkApMap_la6faf0 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    CRntVector<UnkApMapNode_la6faf0*> buckets;  // 0x10
    UnkApMapNode_la6faf0* head;
    UnkApMapNode_la6faf0* tail;

    UnkApMapNode_la6faf0* Find(const UnkApKey_la6faf0* key) {
        if (!buckets.count)
            return nullptr;
        UnkApMapNode_la6faf0** bucket = &buckets.data[(key->id_lo ^ key->id_hi) % buckets.capacity];
        if (!bucket)
            return nullptr;
        for (UnkApMapNode_la6faf0* node = *bucket; node; node = node->next)
            if (node->key == key)
                return node;
        return nullptr;
    }
    bool Contains(const UnkApKey_la6faf0* key) { return Find(key) != nullptr; }
    CRntVector<CStrId>& At(const UnkApKey_la6faf0* key) { return Find(key)->names; }
};
static_assert(sizeof(UnkApMap_la6faf0) == 0x48, "layout");

struct UnkApAnimation_la6faf0 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};

struct UnkApCharClass_la6faf0 {
    u8 pad_0x0[0x1d0];
    UnkApAnimation_la6faf0* animation;  // 0x1d0
};

struct UnkApUser_la6faf0 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkApUserHandle_la6faf0 {
    u8 pad_0x0[0x10];
    UnkApUser_la6faf0* target;  // 0x10
};

struct UnkAp_la6faf0 {
    u8 pad_0x0[0xe8];
    UnkApUserHandle_la6faf0* user;  // 0xe8
    u8 pad_0xf0[0x598 - 0xf0];
    const UnkApKey_la6faf0* key;  // 0x598
    UnkApMap_la6faf0 names;       // 0x5a0
    u8 pad_0x5e8[0x698 - 0x5e8];
    UnkApCharClass_la6faf0* char_class;  // 0x698
    u8 pad_0x6a0[0x91c - 0x6a0];
    bool waiting;  // 0x91c
};
static_assert(__builtin_offsetof(UnkAp_la6faf0, waiting) == 0x91c, "layout");

void fn_7100a6faf0(UnkAccessPointPair_A6_b62* pair) {
    UnkAp_la6faf0* self = reinterpret_cast<UnkAp_la6faf0*>(pair);
    if (self->waiting) {
        UnkGuiRef_la6faf0* ref = static_cast<UnkGuiRef_la6faf0*>(g_unk_7101d47310->field_0x2258);
        if (ref && ref->composition) {
            self->waiting = ref->composition->busy;
            if (!self->waiting) {
                CRntVector<CStrId> names(0xf);
                if (self->names.Contains(self->key))
                    fn_71000ce19c(reinterpret_cast<UnkStruct_0x28*>(&names),
                                  reinterpret_cast<const UnkStruct_0x28*>(&self->names.At(self->key)));
                void* manager = NamesManager();
                const UnkApTunable_la6faf0* tunable = reinterpret_cast<const UnkApTunable_la6faf0*>(&g_unk_717274ec40);
                fn_7100fb7a1c(tunable->field_0x44, tunable->field_0x40, manager, &names, 0, 1, nullptr,
                              UnkApCallback_la6faf0(self, fn_7100a74650));
                names.Free();
            }
        }
    }
    UnkAnimationComponent* animation = self->user->target->animation;
    if (animation && fn_710117ae38(animation, self->char_class->animation->id, false)) {
        UnkGuiRef_la6faf0* ref = static_cast<UnkGuiRef_la6faf0*>(g_unk_7101d47310->field_0x2258);
        if (!ref)
            return;
        UnkGuiComposition_ovb0090* gui = reinterpret_cast<UnkGuiComposition_ovb0090*>(ref->composition);
        if (!gui)
            return;
        CStrId back("#GUI_B_BACK", true);
        CStrId accept("#GUI_A_ACCEPT", true);
        CStrId text("#GUI_SAVESTATION_QUESTION", true);
        AskQuestion_la6faf0(gui, text, accept, back, UnkApCallback_la6faf0(self, fn_7100a7465c),
                            UnkApCallback_la6faf0(self, fn_7100a74668));
    }
    fn_7100a6fe44(pair);
}
