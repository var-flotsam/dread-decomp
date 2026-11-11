// Two copies of one function, in the behaviortree::CEmmySearchingAction unit (fn_710051cdd8) and the
// CEmmyAIComponent unit (fn_7100560d14, its own node vtable and query): empties a node list, asks the
// owner's source (+0x8) for its items into a temporary array (flags 0xf), and appends one node per
// item that records the item, the owner and the owner's +0x18 value. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU811.h"
#include "game/behaviortree/CEmmySearchingAction.h"
#include "types.h"

struct UnkCollectNode_A3_b70 {
    void* vtable;
    void* item;
    void* owner;
    float field_0x18;
    float field_0x1c;
    UnkCollectNode_A3_b70* next;

    explicit UnkCollectNode_A3_b70(void* vt)
        : vtable(vt), item(nullptr), owner(nullptr), field_0x18(3.4028235e38f), field_0x1c(3.4028235e38f), next(nullptr) {}
};

struct UnkCollectList_A3_b70 {
    void* vtable;
    u32 count;
    u32 pad_0xc;
    UnkCollectNode_A3_b70* head;
    UnkCollectNode_A3_b70* tail;
    base::UnkAllocator* allocator;
    u32 tag;
};

struct UnkCollectOwner_A3_b70 {
    void* vtable;
    void* source;
    u8 pad_0x10[8];
    float field_0x18;
};

void fn_7100ad0380(void* source, bool flag, u32 mode, base::UnkVector<void*>* out);
void fn_71006fb09c(void* source, bool flag, u32 mode, base::UnkVector<void*>* out);

template <void (*Query)(void*, bool, u32, base::UnkVector<void*>*)>
static inline __attribute__((always_inline)) void Collect_A3_b70(UnkCollectOwner_A3_b70* self, bool flag, u32 mode,
                                                                 UnkCollectList_A3_b70* list, void* vtable) {
    UnkCollectNode_A3_b70** ends = &list->head;
    for (UnkCollectNode_A3_b70* node = ends[0]; node;) {
        UnkCollectNode_A3_b70* next = node->next;
        list->allocator->vf_0x48(node);
        node = next;
    }
    asm("" : "+r"(ends));
    list->count = 0;
    ends[0] = nullptr;
    ends[1] = nullptr;
    base::UnkVector<void*> items(0xf);
    Query(self->source, flag, mode, &items);
    for (void **it = items.data, **end = items.data + items.count; it != end; ++it) {
        void* item = *it;
        auto* node = static_cast<UnkCollectNode_A3_b70*>(list->allocator->vf_0x40(0x28, 8, list->tag));
        if (node)
            new (node) UnkCollectNode_A3_b70(vtable);
        if (list->count == 0) {
            list->head = node;
            list->tail = node;
            list->count = 1;
        } else {
            // Stored bytewise: the original reloads the count after this store.
            __builtin_memcpy(&list->tail->next, &node, sizeof(node));
            node->next = nullptr;
            list->tail = node;
            list->count = list->count + 1;
        }
        node->item = item;
        node->owner = self;
        node->field_0x18 = self->field_0x18;
        node->field_0x1c = 0;
    }
    items.Free();
}

void fn_710051cdd8(UnkCollectOwner_A3_b70* self, bool flag, u32 mode, UnkCollectList_A3_b70* list) {
    Collect_A3_b70<fn_7100ad0380>(self, flag, mode, list, reinterpret_cast<char*>(g_unk_7101a47fa8) + 0x10);
}
void fn_7100560d14(UnkCollectOwner_A3_b70* self, bool flag, u32 mode, UnkCollectList_A3_b70* list) {
    Collect_A3_b70<fn_71006fb09c>(self, flag, mode, list, reinterpret_cast<char*>(g_unk_7101a48c20) + 0x10);
}
