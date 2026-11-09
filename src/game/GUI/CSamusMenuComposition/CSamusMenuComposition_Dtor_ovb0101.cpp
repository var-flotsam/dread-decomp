// GUI::CSamusMenuComposition's unit (tools/tu_map.py): the destructor of a named list of 0x20-byte
// entries (fn_7100b6fd18) and the composition's destructor (fn_7100b6d26c): its deferred-set
// registration, string ids, the sub-object at +0x288 (fn_7100b6d41c) and the chained map of named
// entry lists at +0x240, then the display-object container's destructor. Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkGuiDisplayObject.h"
#include "types.h"

using base::global::CStrId;

void fn_7100b6d174(void* self);

// An entry (0x20 bytes) whose destructor is out of line.
struct UnkMenuEntry_b0101 {
    u64 unk_0x0[4];
    ~UnkMenuEntry_b0101() { fn_7100b6d174(this); }
};

// A named list of entries.
struct UnkMenuEntryList_b0101 {
    CStrId id;
    base::UnkVector<UnkMenuEntry_b0101> entries;  // 0x8
};

// The named list's destructor.
void fn_7100b6fd18(UnkMenuEntryList_b0101* self) {
    self->entries.Free();
    self->id.Reset();
}

void fn_71004567dc(void* set, void** item);
extern void* g_unk_7101a9e6b8[];  // the class vtable

// A registration in a deferred set ({set, obj, fn}; removed through fn_71004567dc).
struct UnkMenuReg_b0101 {
    void* set;
    void* obj;
    void (*fn)();
    ~UnkMenuReg_b0101() {
        if (set) {
            fn_71004567dc(set, reinterpret_cast<void**>(&set));
            set = nullptr;
        }
        obj = nullptr;
        fn = nullptr;
    }
};

// A node of the entry-list map (its next node at +0x30).
struct UnkMenuMapNode_b0101 {
    UnkMenuEntryList_b0101 value;
    UnkMenuMapNode_b0101* next;  // 0x30
};

// A chained map: its nodes come from the allocator at +0x240 and hang off one list (+0x280), the
// buckets are an array of node pointers.
struct UnkMenuMap_b0101 {
    base::UnkAllocator* allocator;  // 0x240
    u32 field_0x248;
    u32 count;  // 0x24c
    base::UnkVector<UnkMenuMapNode_b0101*> buckets;  // 0x250
    u64 field_0x278;
    UnkMenuMapNode_b0101* head;  // 0x280
};

struct UnkMenuSub_b0101;
void fn_7100b6d41c(UnkMenuSub_b0101* self);

struct UnkSamusMenuComposition_b0101 {
    void** vtable;
    u8 pad_0x8[0x228];
    CStrId field_0x230;
    CStrId field_0x238;
    UnkMenuMap_b0101 map;  // 0x240
    u8 field_0x288[0x48];
    CStrId field_0x2d0;
    u64 field_0x2d8;
    UnkMenuReg_b0101 reg;  // 0x2e0
};
static_assert(__builtin_offsetof(UnkSamusMenuComposition_b0101, reg) == 0x2e0, "UnkSamusMenuComposition_b0101 layout");

void fn_7100b6d26c(UnkSamusMenuComposition_b0101* self) {
    self->vtable = g_unk_7101a9e6b8 + 2;
    self->reg.~UnkMenuReg_b0101();
    self->field_0x2d0.Reset();
    fn_7100b6d41c(reinterpret_cast<UnkMenuSub_b0101*>(self->field_0x288));
    UnkMenuMap_b0101* map = &self->map;
    if (map->count) {
        for (UnkMenuMapNode_b0101* node = map->head; node;) {
            UnkMenuMapNode_b0101* next = node->next;
            fn_7100b6fd18(&node->value);
            map->allocator->vf_0x48(node);
            node = next;
        }
        __builtin_memset(map->buckets.data, 0, (u64)map->buckets.capacity * sizeof(void*));
        map->count = 0;
        map->field_0x278 = 0;
        map->head = nullptr;
    }
    map->buckets.Free();
    self->field_0x238.Reset();
    self->field_0x230.Reset();
    fn_71011d8e90(reinterpret_cast<UnkGuiDisplayObject*>(self));
}
