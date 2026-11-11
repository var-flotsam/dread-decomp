// CInventoryComponent's unit (tools/tu_map.py): the component's constructor (fn_710114112c: a map of
// owned values by id at +0x40 with 11 buckets, an empty id at +0x88 and a list after it), deleting the
// map's values and clearing it (fn_7101141378), its release (fn_71011423c4), reading an item amount from
// a stream (fn_7101144178), applying the blackboard section an id names (fn_7101149354) and writing the
// missile amounts to the blackboard (fn_71011406cc). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/CComponent.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU56.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::UnkStrIdMember;

struct UnkInvValue_ovb0094 {
    u8 unk_0x0[0x10];
};

// A map node: a string id and an owned value.
struct UnkInvNode_ovb0094 {
    UnkStrIdMember id;
    UnkInvValue_ovb0094* value;  // 0x8
    UnkInvNode_ovb0094* next;  // 0x10
};

struct UnkInvMap_ovb0094 {
    base::UnkAllocator* allocator;  // 0x0
    u32 flags;                      // 0x8
    u32 count;                      // 0xc
    UnkInvNode_ovb0094** buckets;   // 0x10
    u32 field_0x18;
    u32 bucket_count;  // 0x1c
    u8 unk_0x20[0x18];
    UnkInvNode_ovb0094* tail;  // 0x38
    UnkInvNode_ovb0094* head;  // 0x40

    void Clear() {
        if (count) {
            for (UnkInvNode_ovb0094* node = head; node;) {
                UnkInvNode_ovb0094* next = node->next;
                node->id.~UnkStrIdMember();
                allocator->vf_0x48(node);
                node = next;
            }
            __builtin_memset(buckets, 0, (u64)bucket_count * sizeof(void*));
            count = 0;
            tail = nullptr;
            head = nullptr;
        }
    }
};

struct UnkInvOwner_ovb0094 {
    u8 unk_0x0[0x40];
    UnkInvMap_ovb0094 map;  // 0x40
};

// Deletes the values of the map at +0x40, then clears it.
void fn_7101141378(UnkInvOwner_ovb0094* self) {
    for (UnkInvNode_ovb0094* node = self->map.head; node; node = node->next)
        delete node->value;
    self->map.Clear();
}

struct UnkHandleOwner_71010c4230;
struct UnkSpawnGroupKey;
void fn_7100de9730(UnkGameManager* manager, UnkSpawnGroupKey* key, void* object);
extern u64 g_unk_7172a092b0 __attribute__((visibility("hidden")));

struct UnkInvRelease_ovb0094 {
    u8 unk_0x0[0x98];
    UnkStrIdMember* ids;  // 0x98
    u32 count;            // 0xa0
};

// Unregisters the component from the game manager, runs fn_71010c4230 and releases the ids at +0x98.
void fn_71011423c4(UnkInvRelease_ovb0094* self) {
    fn_7100de9730(g_unk_7101d47310, reinterpret_cast<UnkSpawnGroupKey*>(&g_unk_7172a092b0), self);
    fn_71010c4230(reinterpret_cast<UnkHandleOwner_71010c4230*>(self));
    if (self->ids) {
        for (u64 i = 0; i < self->count; ++i)
            self->ids[i].~UnkStrIdMember();
    }
    self->count = 0;
}

using base::global::CStrId;

struct UnkStrIdHandle;
void fn_7100ae986c(UnkStrIdHandle* self, const CStrId& id);
struct UnkInventory;
void fn_7101142a28(UnkInventory* inventory, const UnkQualifiedId& item, bool a, bool b, bool c, f32 amount,
                   f32 extra);

// A qualified id built from a string id (fn_7100ae986c), its id released when it goes.
struct UnkInvItemId_ovb0094 {
    union {
        UnkQualifiedId item;
    };

    explicit UnkInvItemId_ovb0094(const CStrId& from) { fn_7100ae986c(reinterpret_cast<UnkStrIdHandle*>(this), from); }
    ~UnkInvItemId_ovb0094() { item.id.~CStrId(); }
};

// Reads an item name, an amount and a flag from the stream ("sfb") and adds the amount of the item.
bool fn_7101144178(UnkInventory* self, void* stream) {
    char name[0x200] = {};
    f32 amount = 0.0f;
    bool flag = false;
    fn_710007cf30(stream, "sfb", name, &amount, &flag);
    fn_7101142a28(self, UnkInvItemId_ovb0094(CStrId(name, true)).item, flag, true, true, amount, 0.0f);
    return true;
}

void fn_710114433c(void* self, void* section_data);

// A string id built from a CRntString (fn_7100000850).
struct UnkInvRntId_ovb0094 {
    union {
        CStrId id;
    };

    explicit UnkInvRntId_ovb0094(const base::global::CRntString& str) { fn_7100000850(&id, str, true); }
    ~UnkInvRntId_ovb0094() { id.~CStrId(); }
};

struct UnkInvSection_ovb0094 {
    u8 unk_0x0[0x88];
    CStrId id;  // 0x88
};

// Sets the id at +0x88 from the string and applies the section it names in the game manager's blackboard.
void fn_7101149354(UnkInvSection_ovb0094* self, const base::global::CRntString& name) {
    UnkInvRntId_ovb0094 id(name);
    self->id = id.id;
    if (self->id.data != g_unk_7101d470e0.data && fn_7101047208(&g_unk_7101d47310->field_0x968, self->id))
        fn_710114433c(self, &fn_71010472e0(&g_unk_7101d47310->field_0x968, self->id)->data);
}

// The id table (fn_7100031894 builds it; the same definition as src/game/CGameManager.cpp's).
struct UnkGameManagerIdTable {
    u8 pad_0x0[0x17e0];
    u8 field_0x17e0[8];
    u8 pad_0x17e8[0x2578 - 0x17e8];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

// A typed value as the section setter takes it (as in src/game/CDoorLifeComponent.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection*, UnkPropertyName*, const UnkTypedValue*);

// The item's qualified id, built on first use (as src/game_s7/fn_7100fc4b34.cpp's).
static inline UnkQualifiedId& Qid_ovb0094(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if ((u64)qid.id.data == (u64)g_unk_7101d47108 + 0x10)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
    return qid;
}

__attribute__((always_inline)) inline void SetF32_ovb0094(CBlackboard* blackboard, const CStrId& section,
                                                          UnkQualifiedId& name, f32 amount) {
    UnkTypedValue v;
    f32 value = amount;
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            v.type = fn_710000abbc();
            v.value = &value;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(&name), &v);
        }
    }
}

struct UnkInvMissiles_ovb0094 {
    u8 unk_0x0[0x24];
    s32 amount;  // 0x24
};

// Writes the missile amount as both the current and the maximum missile count.
void fn_71011406cc(UnkInvMissiles_ovb0094* self) {
    CBlackboard* blackboard = &g_unk_7101d47310->field_0x968;
    SetF32_ovb0094(blackboard, *reinterpret_cast<const CStrId*>(&GameManagerIdTable().pad_0x0[0x1bd8]),
                   Qid_ovb0094(g_unk_717275a218), (f32)self->amount);
    SetF32_ovb0094(blackboard, *reinterpret_cast<const CStrId*>(&GameManagerIdTable().pad_0x0[0x1bd8]),
                   Qid_ovb0094(g_unk_717275a1d8), (f32)self->amount);
}

struct UnkStateObject;
extern void* g_unk_7101cb33c8[];

// The component the destructor fn_7101141558 tears down, with its fields written out flat.
struct UnkInvComponent_ovb0094 {
    void** vtable;
    u8 unk_0x8[0x38];
    base::UnkAllocator* node_allocator;  // 0x40
    u32 node_flags;                      // 0x48
    u32 node_count;                      // 0x4c
    void** buckets;                      // 0x50
    u32 bucket_count;                    // 0x58
    u32 bucket_capacity;                 // 0x5c
    u32 bucket_step;                     // 0x60
    bool field_0x64;
    u32 bucket_flags;                    // 0x68
    base::UnkAllocator* bucket_allocator;  // 0x70
    void* tail;                          // 0x78
    void* head;                          // 0x80
    CStrId id;                           // 0x88
    void* field_0x90;
    void* field_0x98;
    void* field_0xa0;
    u32 field_0xa8;
    u32 field_0xac;
    base::UnkAllocator* field_0xb0;

    // Sets the bucket count to n, clearing the new buckets.
    void ResizeBuckets(u32 n) {
        if (bucket_count < n) {
            void** p;
            if (bucket_capacity >= n) {
                p = buckets;
            } else {
                p = static_cast<void**>(bucket_allocator->vf_0x40((u64)n * sizeof(void*), 8, bucket_flags));
                if (buckets) {
                    __builtin_memcpy(p, buckets, (u64)bucket_count * sizeof(void*));
                    bucket_allocator->vf_0x48(buckets);
                }
                bucket_capacity = n;
                buckets = p;
            }
            __builtin_memset(&p[bucket_count], 0, (u64)(n - bucket_count) * sizeof(void*));
            bucket_count = n;
        } else if (bucket_count != n) {
            bucket_count = n;
        }
    }
};
static_assert(__builtin_offsetof(UnkInvComponent_ovb0094, field_0xb0) == 0xb0, "UnkInvComponent_ovb0094 layout");

void fn_710114112c(UnkInvComponent_ovb0094* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = &g_unk_7101cb33c8[2];
    self->node_allocator = fn_7100083990();
    self->node_flags = 0xe;
    self->node_count = 0;
    self->buckets = nullptr;
    self->bucket_count = 0;
    self->bucket_capacity = 0;
    self->bucket_step = 10;
    self->field_0x64 = true;
    self->bucket_flags = 0xe;
    self->bucket_allocator = fn_7100083990();
    self->tail = nullptr;
    self->head = nullptr;
    self->ResizeBuckets(11);
    self->id.data = nullptr;
    {
        base::global::UnkStrIdBase empty(base::global::UnkStrIdInline(), nullptr, true);
        self->id.data = empty.data;
    }
    self->field_0x90 = nullptr;
    self->field_0xac = 0xe;
    self->field_0x98 = nullptr;
    self->field_0xa0 = nullptr;
    self->field_0xa8 = 10;
    self->field_0xb0 = fn_7100083990();
}
