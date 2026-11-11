#include <new>
#include "base/UnkResKey.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkTaggedObject;
struct UnkTaggedWidgetObject;
struct UnkWidgetArg;
void fn_7100147b0c(UnkTaggedWidgetObject* self, UnkWidgetArg* arg);
void fn_7100147b5c(UnkTaggedObject* self);
void fn_71001c3438(void* p);
bool fn_7100e13c10(UnkResHolder2* self, const CFilePathStrId& path);

struct UnkTexObject_b0247 {
    u8 pad_0x0[0x120];
    void* field_0x120;
};

struct UnkPoolNode_b0247 {
    void* key;
    void* value;
    UnkPoolNode_b0247* next;   // 0x10: the list of all nodes
    UnkPoolNode_b0247* prev;   // 0x18
    UnkPoolNode_b0247* chain;  // 0x20: the next node in the bucket
};

struct UnkPoolMap_b0247 {
    using Node = UnkPoolNode_b0247;
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    Node** buckets;       // 0x10
    u32 bucket_count;     // 0x18
    u32 bucket_capacity;  // 0x1c
    u8 pad_0x20[0x18];
    Node* tail;  // 0x38
    Node* head;  // 0x40

    __attribute__((always_inline)) void Insert(void* key, void* value, void*& result) {
        Node** slot = nullptr;
        if (bucket_count) {
            u32 i = (u32)(u64)key % bucket_capacity;
            asm("" : "+r"(i));  // the original computes the index before loading the buckets
            slot = &buckets[i];
        }
        Node* c = *slot;
        auto* n = static_cast<Node*>(allocator->vf_0x40(0x28, 8, flags));
        if (n) {
            n->key = key;
            n->value = value;
            n->next = nullptr;
            n->prev = nullptr;
            n->chain = nullptr;
        }
        n->chain = c;
        *slot = n;
        if (!head) {
            head = n;
        } else if (tail) {
            __builtin_memcpy(&n->prev, &tail, sizeof(Node*));  // the original reloads the tail
            tail->next = n;
        }
        u32 cnt = count;
        tail = n;
        // keeps the result a separate value from the object (the original returns it after the tail store)
        result = key;
        asm("" : "+r"(result));
        count = cnt + 1;
    }
};

void* fn_7100e13abc(UnkPoolMap_b0247* pool, const CFilePathStrId* path, UnkTexObject_b0247** out,
                    UnkWidgetArg* arg) {
    auto* object = static_cast<UnkTexObject_b0247*>(fn_71000839a0(0x158, 0x38));
    fn_7100147b0c(reinterpret_cast<UnkTaggedWidgetObject*>(object), arg);
    void* node = nullptr;
    if (object) {
        fn_71001c3438(object->field_0x120);
        if (path->data != &g_unk_7101d47200->empty &&
            !fn_7100e13c10(reinterpret_cast<UnkResHolder2*>(object), *path)) {
            fn_7100147b5c(reinterpret_cast<UnkTaggedObject*>(object));
            ::operator delete(object);
            return nullptr;
        }
        pool->Insert(object, object, node);
    }
    if (out)
        *out = object;
    return node;
}
