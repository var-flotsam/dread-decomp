#include "types.h"

struct UnkAtkLoader;
struct UnkAtkArchive;
u32 fn_71012069f8(UnkAtkArchive* archive, u32 id);
void* fn_7101211d30(UnkAtkLoader* self, u32 file_id, void* heap, void* arg, u32 unk);
bool fn_7101206d0c(UnkAtkArchive* archive, u32 id, u32* out);

struct UnkAtkLoaderVt_b0149 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void* vf_0x18();
    virtual void* vf_0x20(u32 file);
    UnkAtkArchive* archive;  // 0x8
};

bool fn_71012117ac(UnkAtkLoaderVt_b0149* self, u32 id, void* heap, u32 flags, void* arg) {
    u32 file = fn_71012069f8(self->archive, id);
    if (flags & 2) {
        if (!fn_7101211d30((UnkAtkLoader*)self, file, heap, arg, 0))
            return false;
    }
    if (flags & 8) {
        if (!self->vf_0x20(file))
            return false;
        alignas(8) u32 out;
        if (!fn_7101206d0c(self->archive, id, &out))
            return false;
        u32 next = out;
        if (!fn_7101211d30((UnkAtkLoader*)self, fn_71012069f8(self->archive, next), heap, arg, 1))
            return false;
    }
    return true;
}

void fn_71012392d0(void* lock);
void fn_71012392e0(void* lock);

struct UnkAtkRingNode_b0149 {
    u8 pad_0x0[0x8];
    UnkAtkRingNode_b0149* next;  // 0x8
    u8 pad_0x10[0x8];
    u8* buf;  // 0x18
    s32 capacity;  // 0x20
    s32 write;  // 0x24
    u8 pad_0x28[4];
    s32 count;  // 0x2c
};
struct UnkAtkRingHead_b0149 {
    u8 pad_0x0[0x8];
    UnkAtkRingNode_b0149* next;  // 0x8
    u8 lock[8];  // 0x10
};
struct UnkAtkEntry_b0149 {
    u64 w[11];
};

void fn_7101238c14(UnkAtkRingHead_b0149* self, const UnkAtkEntry_b0149* entry) {
    fn_71012392d0(self->lock);
    for (UnkAtkRingNode_b0149* n = self->next; (void*)self != (void*)n; n = n->next) {
        if (__atomic_load_n(&n->count, __ATOMIC_ACQUIRE) >= n->capacity)
            continue;
        s32 i = n->write;
        s32 next = i + 1;
        UnkAtkEntry_b0149* slot = (UnkAtkEntry_b0149*)(n->buf + (s64)i * 0x58);
        n->write = next;
        *slot = *entry;
        if (n->write == n->capacity)
            n->write = 0;
        __atomic_fetch_add(&n->count, 1, __ATOMIC_SEQ_CST);
    }
    fn_71012392e0(self->lock);
}

struct UnkAtkLink_b0149 {
    UnkAtkLink_b0149* prev;
    UnkAtkLink_b0149* next;
};
struct UnkAtkChain_b0149 {
    u8 pad_0x0[0x18];
    UnkAtkLink_b0149 link;  // 0x18
    u8 pad_0x28[8];
    void* field_0x30;
    void* field_0x38;
    void* field_0x40;
    u8 field_0x48[8];
};
struct UnkAtkObj_b0149 {
    u8 pad_0x0[0x130];
    void* field_0x130;
    u8 pad_0x138[0x2a0];
    UnkAtkLink_b0149 link;  // 0x3d8
};
void* fn_710122ade8(void* pool);
void fn_710122ae00(void* pool, void* item);
void fn_710122b710(void* item);
void fn_710122b868(void* item);
void fn_710120f188(void* a, void* b, void* c, void* d);

UnkAtkObj_b0149* fn_710122ca14(UnkAtkChain_b0149* self) {
    UnkAtkObj_b0149* obj = (UnkAtkObj_b0149*)fn_710122ade8(self);
    if (!obj)
        return nullptr;
    fn_710122b710(obj);
    u64* a = nullptr;
    if (self->field_0x30) {
        a = (u64*)fn_710122ade8(self->field_0x30);
        if (!a) {
            fn_710122b868(obj);
            fn_710122ae00(self, obj);
            return nullptr;
        }
        a[1] = 0;
        a[2] = 0;
        a[0] = 0;
        void* b = fn_710122ade8(self->field_0x38);
        if (!b) {
            fn_710122b868(obj);
            fn_710122ae00(self, obj);
            fn_710122ae00(self->field_0x30, a);
            return nullptr;
        }
        fn_710120f188(a, b, self->field_0x40, self->field_0x48);
        obj->field_0x130 = a;
    }
    UnkAtkLink_b0149* node = &self->link;
    UnkAtkLink_b0149* head = &obj->link;
    UnkAtkLink_b0149* old = head->prev;
    head->prev = node->prev;
    old->next = node;
    node->prev->next = head;
    node->prev = old;
    return obj;
}
