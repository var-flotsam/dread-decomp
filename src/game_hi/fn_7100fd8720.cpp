// Stops every active entry in the list at the given slot (growing the slot array on demand),
// then frees the list's nodes.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

struct UnkHandle;
struct UnkMgr;

struct UnkNode_0x68 {
    u8 pad_0x0[0x28];
    UnkHandle* field_0x28;
    u32 field_0x30;
    u8 pad_0x34[0x2c];
    UnkNode_0x68* next;
};

extern char g_unk_7101cac270[];  // base vtable
extern char g_unk_7101cac250[];  // vtable
extern UnkMgr* g_unk_7101d4d820;

void fn_7100fde6c4(UnkNode_0x68*);  // node destructor
void fn_7100108a84(UnkMgr*, UnkHandle*, f32);

struct UnkNodeList_0x38 {
    char* vtable;
    u32 count;
    UnkNode_0x68* head;
    UnkNode_0x68* tail;
    u32 flags;
    base::UnkAllocator* allocator;
    u64 field_0x30;

    UnkNodeList_0x38() {
        vtable = g_unk_7101cac270 + 0x10;
        count = 0;
        head = nullptr;
        tail = nullptr;
        flags = 0x10;
        base::UnkAllocator* a = fn_7100083990();
        vtable = g_unk_7101cac250 + 0x10;
        allocator = a;
        field_0x30 = 0;
    }

    UnkNode_0x68* Begin() { return head; }
    UnkNode_0x68* End() { return nullptr; }

    void Clear() {
        for (UnkNode_0x68* n = head; n;) {
            UnkNode_0x68* next = n->next;
            fn_7100fde6c4(n);
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

struct UnkListArray {
    UnkNodeList_0x38* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool autoGrow;

    UnkNodeList_0x38& operator[](u32 index);
};

void fn_7100fde040(UnkListArray*, u32);  // reserve

inline UnkNodeList_0x38& UnkListArray::operator[](u32 index) {
    if (autoGrow && count <= index) {
        u32 n = capacity;
        if (capacity <= index && step != 0)
            n = (index / step + 1) * step;
        fn_7100fde040(this, n);
        fn_7100fde040(this, index + 1);
        for (u32 i = count; i < index + 1; ++i)
            new (&data[i]) UnkNodeList_0x38();
        count = index + 1;
    }
    return data[index];
}

struct UnkStruct_0xf8 {
    u8 pad_0x0[0xe0];
    UnkListArray field_0xe0;
};

void fn_7100fd8720(UnkStruct_0xf8* self, u32 index) {
    for (UnkNode_0x68* n = self->field_0xe0[index].Begin(); n != self->field_0xe0[index].End(); n = n->next) {
        if (n->field_0x30)
            fn_7100108a84(g_unk_7101d4d820, n->field_0x28, 0.0f);
    }
    self->field_0xe0[index].Clear();
}
