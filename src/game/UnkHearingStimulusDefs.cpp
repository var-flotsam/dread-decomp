// The hearing stimulus definitions (tools/tu_map.py unit 2579): a table of CHearingStimulusDef keyed
// by stimulus name, on top of a base class (fn_7100f02c84) that holds two arrays and the table.
// fn_7100f02740 is the constructor: it adds the definitions of Samus's footsteps and of the Morph
// Ball's movement. The definition's fields come from reflection (CStimulusDef: sId, fMaxRange,
// fMaxTTL); the table's layout is base::UnkStrIdHashMap's (base/UnkCloneHelpers.h). Other names and
// layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_7101caa248[];  // the table's vtable
extern char g_unk_7101caa2a8[];  // CHearingStimulusDef's vtable

// A definition's name: a string id that is copied from a CStrId and moved between definitions
// (the moved-from one is left with the empty id).
struct UnkStimulusId {
    base::global::UnkStringData* data;

    explicit UnkStimulusId(const CStrId& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    UnkStimulusId(UnkStimulusId&& other) : data(other.data) { other.data = &g_unk_7101d47108->empty; }
    ~UnkStimulusId() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

// A hearing stimulus: its name, how far it carries and how long it lasts.
struct CHearingStimulusDef {
    const char* vtable;
    UnkStimulusId sId;
    f32 fMaxRange;
    f32 fMaxTTL;

    explicit CHearingStimulusDef(const CStrId& id)
        : vtable(g_unk_7101caa2a8 + 0x10), sId(id), fMaxRange(0.0f), fMaxTTL(-1.0f) {}
};

// A node of the table: in its bucket's chain and in the list of all nodes in insertion order.
struct UnkHearingStimulusNode {
    CStrId key;
    CHearingStimulusDef def;
    // Union members, as in base::UnkStrIdHashNode: the original reloads the list's tail after a
    // link store.
    union {
        UnkHearingStimulusNode* next;
    };
    union {
        UnkHearingStimulusNode* prev;
    };
    UnkHearingStimulusNode* chain;

    UnkHearingStimulusNode(const CStrId& key, CHearingStimulusDef&& def)
        : key(key), def(static_cast<CHearingStimulusDef&&>(def)), next(nullptr), prev(nullptr), chain(nullptr) {}
};

struct UnkHearingStimulusMap {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    UnkHearingStimulusNode** buckets;
    u32 field_0x18;
    u32 bucket_count;
    u8 unk_0x20[0x18];
    UnkHearingStimulusNode* tail;
    UnkHearingStimulusNode* head;

    // Adds `def` under `key`, at the head of its bucket's chain (hashed by the definition's name) and
    // the tail of the list.
    __attribute__((always_inline)) CHearingStimulusDef* Add(const CStrId& key, CHearingStimulusDef&& def) {
        UnkHearingStimulusNode** slot = nullptr;
        if (field_0x18) {
            const char* raw = reinterpret_cast<const char*>(def.sId.data);
            u32 h = *reinterpret_cast<const u32*>(raw + 0x30) ^ *reinterpret_cast<const u32*>(raw + 0x34);
            slot = buckets + h % bucket_count;
        }
        UnkHearingStimulusNode* chain_head = *slot;
        auto* n = static_cast<UnkHearingStimulusNode*>(allocator->vf_0x40(sizeof(UnkHearingStimulusNode), 8, flags));
        if (n)
            new (n) UnkHearingStimulusNode(key, static_cast<CHearingStimulusDef&&>(def));
        n->chain = chain_head;
        *slot = n;
        if (!head) {
            head = n;
        } else if (tail) {
            n->prev = tail;
            tail->next = n;
        }
        tail = n;
        ++count;
        return n == nullptr ? nullptr : &n->def;
    }
};
static_assert(sizeof(UnkHearingStimulusNode) == 0x38, "UnkHearingStimulusNode layout");

struct UnkHearingStimulusDefs {
    const char* vtable;
    u8 unk_0x8[0x50];
    UnkHearingStimulusMap defs;
};
static_assert(__builtin_offsetof(UnkHearingStimulusDefs, defs) == 0x58, "UnkHearingStimulusDefs layout");

void fn_7100f02c84(UnkHearingStimulusDefs* self);  // the base class's constructor

// The constructor.
// NON_MATCHING(minor): register allocation and scheduling (the moved-from name of each temporary
// definition lives in x20 and is copied to x1 for its release, where the original computes it in x1;
// the other values shift by one register; the bucket's modulo is scheduled differently) and block
// layout (the original duplicates the list append of the third definition into the path without a
// node); the same loads, stores and calls.
void fn_7100f02740(UnkHearingStimulusDefs* self) {
    fn_7100f02c84(self);
    self->vtable = g_unk_7101caa248 + 0x10;
    {
        static CStrId s_id("SamusLeftFoot", true);
        CHearingStimulusDef* def = self->defs.Add(s_id, CHearingStimulusDef(s_id));
        def->fMaxRange = 650.0f;
        def->fMaxTTL = 0.35f;
    }
    {
        static CStrId s_id("SamusRightFoot", true);
        CHearingStimulusDef* def = self->defs.Add(s_id, CHearingStimulusDef(s_id));
        def->fMaxRange = 650.0f;
        def->fMaxTTL = 0.35f;
    }
    {
        static CStrId s_id("MorphBallMovement", true);
        CHearingStimulusDef* def = self->defs.Add(s_id, CHearingStimulusDef(s_id));
        def->fMaxRange = 650.0f;
        def->fMaxTTL = 0.5f;
    }
}
