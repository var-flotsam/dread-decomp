// Creates a child object (0x110 bytes), constructed with its index in the owner's child array, and
// appends it to that array of owning pointers. Four copies, one per owner type, each with its own
// child constructor.
#include "base/UnkVector.h"

void* fn_71000839a0(u64 size, u32 flags);

struct UnkChild {
    virtual void vf0();
    virtual void vf8();
    virtual void Delete();
};

// Owns its object: moving leaves the source empty, destroying deletes it.
struct UnkOwnedPtr {
    UnkChild* ptr;

    explicit UnkOwnedPtr(UnkChild* p) : ptr(p) {}
    UnkOwnedPtr(UnkOwnedPtr&& other) : ptr(other.Release()) {}
    UnkChild* Release() {
        UnkChild* p = ptr;
        ptr = nullptr;
        return p;
    }
    ~UnkOwnedPtr() {
        UnkChild* p = ptr;
        ptr = nullptr;
        if (p)
            p->Delete();
    }
};

struct UnkOwnedPtrVector {
    UnkOwnedPtr* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    // A capacity that doesn't grow past the count (no step) hangs, as UnkVector::PushBack does.
    // Pointers are stored bitwise: the original reloads `count` after each store.
    __attribute__((always_inline)) void PushBack(UnkChild* child) {
        u32 n = count;
        u32 old_capacity = capacity;
        u32 new_capacity = old_capacity;
        if (old_capacity <= n && step != 0)
            new_capacity = (n / step + 1) * step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        UnkOwnedPtr* p;
        if (old_capacity < new_capacity) {
            p = static_cast<UnkOwnedPtr*>(allocator->vf_0x40((u64)new_capacity * sizeof(UnkOwnedPtr), 8, flags));
            if (data) {
                UnkOwnedPtr* src = data;
                UnkOwnedPtr* dst = p;
                for (u32 i = 0; i < count; ++i, ++src, ++dst) {
                    UnkChild* moved = src->Release();
                    __builtin_memcpy(dst, &moved, sizeof(UnkOwnedPtr));
                    src->~UnkOwnedPtr();
                }
                allocator->vf_0x48(data);
            }
            n = count;
            data = p;
            capacity = new_capacity;
        } else {
            p = data;
        }
        __builtin_memcpy(&p[n], &child, sizeof(UnkOwnedPtr));
        ++count;
    }
};

struct UnkChildOwner {
    u8 field_0x0[0x148];
    UnkOwnedPtrVector children;
};

template <void (*Construct)(UnkChild*, u32, void*, UnkChildOwner*)>
static inline __attribute__((always_inline)) void AddChild(UnkChildOwner* self, void* arg) {
    UnkChild* child = static_cast<UnkChild*>(fn_71000839a0(0x110, 5));
    Construct(child, self->children.count, arg, self);
    self->children.PushBack(child);
}

void fn_71009b88e0(UnkChild* self, u32 index, void* arg, UnkChildOwner* owner);
void fn_71009bd530(UnkChild* self, u32 index, void* arg, UnkChildOwner* owner);
void fn_71009c5a24(UnkChild* self, u32 index, void* arg, UnkChildOwner* owner);
void fn_71009c734c(UnkChild* self, u32 index, void* arg, UnkChildOwner* owner);

void fn_71009b7ddc(UnkChildOwner* self, void* arg) {
    AddChild<fn_71009b88e0>(self, arg);
}

void fn_71009bce64(UnkChildOwner* self, void* arg) {
    AddChild<fn_71009bd530>(self, arg);
}

void fn_71009bf9a8(UnkChildOwner* self, void* arg) {
    AddChild<fn_71009c5a24>(self, arg);
}

void fn_71009c6fa4(UnkChildOwner* self, void* arg) {
    AddChild<fn_71009c734c>(self, arg);
}
