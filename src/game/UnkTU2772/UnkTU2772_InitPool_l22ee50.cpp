// nn::atk unit: carves a buffer into voice objects (each optionally followed by an extension block whose size
// fn_710120f124 gives), constructs each one and appends it to the pool's free list, once.
// Names and layouts are placeholders.
#include "game/UnkTU2772.h"
#include "types.h"

struct UnkAtkInitObj;
void fn_710122cc20(UnkAtkInitObj* self, void* owner);

struct UnkAtkLink_l22ee50 {
    UnkAtkLink_l22ee50* prev;
    UnkAtkLink_l22ee50* next;
};

// The voice object as the pool sees it: its list link.
struct UnkAtkPoolEntry_l22ee50 {
    u8 pad_0x000[0x288];
    UnkAtkLink_l22ee50 link;
};
static_assert(sizeof(UnkAtkPoolEntry_l22ee50) == 0x298, "entry size");

// Header of an entry's extension block, right after the entry.
struct UnkAtkExtension_l22ee50 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
};

struct UnkAtkPool_l22ee50 {
    bool initialized;
    u8 pad_0x1[0x17];
    UnkAtkLink_l22ee50 freeList;
};

// Links node in at the back of the list whose head is head.
static inline void PushBack(UnkAtkLink_l22ee50* head, UnkAtkLink_l22ee50* node) {
    UnkAtkLink_l22ee50* last = node->prev;
    node->prev = head->prev;
    last->next = head;
    head->prev->next = node;
    head->prev = last;
}

void fn_710122ee50(UnkAtkPool_l22ee50* self, void* buffer, u64 size, void* param) {
    if (self->initialized)
        return;
    u64 extra = fn_710120f124(param);
    u64 entrySize = extra != 0 ? extra + sizeof(UnkAtkPoolEntry_l22ee50) + sizeof(UnkAtkExtension_l22ee50)
                               : sizeof(UnkAtkPoolEntry_l22ee50);
    s32 count = size / entrySize;
    u8* cur = static_cast<u8*>(buffer);
    for (s32 i = 0; i < count; ++i) {
        UnkAtkPoolEntry_l22ee50* entry = reinterpret_cast<UnkAtkPoolEntry_l22ee50*>(cur);
        cur += sizeof(UnkAtkPoolEntry_l22ee50);
        u64 extensionSize = fn_710120f124(param);
        UnkAtkExtension_l22ee50* extension = nullptr;
        if (extensionSize != 0) {
            extension = reinterpret_cast<UnkAtkExtension_l22ee50*>(cur);
            extension->field_0x0 = 0;
            extension->field_0x8 = 0;
            extension->field_0x10 = 0;
            u8* work = cur + sizeof(UnkAtkExtension_l22ee50);
            fn_710120f188(extension, work, reinterpret_cast<void*>(extensionSize), param);
            cur = work + extensionSize;
        }
        fn_710122cc20(reinterpret_cast<UnkAtkInitObj*>(entry), extension);
        PushBack(&self->freeList, &entry->link);
    }
    self->initialized = true;
}
