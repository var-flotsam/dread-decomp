// Copies of fn_71002d5a88 (src/game_s2/fn_71002d5a88.cpp) for other owners and arguments: each
// child object of an array is called through a virtual slot with the argument passed on. A float
// to slot 0x30, the array at 0x40 (fn_710014be78) or 0x38 (fn_710014bec8, fn_710014bf18), and a
// bool to slot 0x118, the array at 0xa0 with its count at 0xb0 (fn_7100233584).
#include "types.h"

struct UnkFloatChild {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30(f32 arg);
};

// base::UnkVector's leading fields.
template <class T>
struct UnkChildArray {
    T** data;
    u32 count;
};

template <class T>
__attribute__((always_inline)) static inline void CallEach(UnkChildArray<T>* children, f32 arg) {
    T** end = children->data + children->count;
    for (T** p = children->data; p != end; ++p)
        (*p)->vf_0x30(arg);
}

struct UnkFloatOwner40 {
    u8 pad_0x0[0x40];
    UnkChildArray<UnkFloatChild> children;
};

struct UnkFloatOwner38 {
    u8 pad_0x0[0x38];
    UnkChildArray<UnkFloatChild> children;
};

void fn_710014be78(UnkFloatOwner40* self, f32 arg) { CallEach(&self->children, arg); }

void fn_710014bec8(UnkFloatOwner38* self, f32 arg) { CallEach(&self->children, arg); }

void fn_710014bf18(UnkFloatOwner38* self, f32 arg) { CallEach(&self->children, arg); }

struct UnkBoolChild {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8();
    virtual void vf_0x100();
    virtual void vf_0x108();
    virtual void vf_0x110();
    virtual void vf_0x118(bool arg);
};

struct UnkBoolOwner {
    u8 pad_0x0[0xa0];
    UnkBoolChild** children;
    u8 pad_0xa8[8];
    u32 count;
};

void fn_7100233584(UnkBoolOwner* self, bool arg) {
    UnkBoolChild** end = self->children + self->count;
    for (UnkBoolChild** p = self->children; p != end; ++p)
        (*p)->vf_0x118(arg);
}

// An object of the same owner layout that calls one of its own slots and then another slot on
// each child: slot 0x58 then children's 0x50 (fn_7100232938), slot 0x68 then children's 0x58
// (fn_710023298c).
struct UnkTreeNode {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    u8 pad_0x8[0x98];
    UnkTreeNode** children;
    u8 pad_0xa8[8];
    u32 count;
};

void fn_7100232938(UnkTreeNode* self) {
    self->vf_0x58();
    UnkTreeNode** end = self->children + self->count;
    for (UnkTreeNode** p = self->children; p != end; ++p)
        (*p)->vf_0x50();
}

void fn_710023298c(UnkTreeNode* self) {
    self->vf_0x68();
    UnkTreeNode** end = self->children + self->count;
    for (UnkTreeNode** p = self->children; p != end; ++p)
        (*p)->vf_0x58();
}

// Passes each child of an array at 0x60 (count at 0x68) to fn_710021b8e0 (fn_7100224ebc,
// fn_7100224ef4).
void fn_710021b8e0(void* child);

struct UnkChildOwner60 {
    u8 pad_0x0[0x60];
    UnkChildArray<void> children;
};

__attribute__((always_inline)) static inline void PassEach(UnkChildOwner60* self) {
    void** end = self->children.data + self->children.count;
    for (void** p = self->children.data; p != end; ++p)
        fn_710021b8e0(*p);
}

void fn_7100224ebc(UnkChildOwner60* self) { PassEach(self); }

void fn_7100224ef4(UnkChildOwner60* self) { PassEach(self); }

// Passes each 0x48-byte element of an array (base::UnkVector's leading fields) to fn_7100119d3c
// with a flag (fn_7100125338).
struct UnkElem_0x48 {
    u8 data[0x48];
};
void fn_7100119d3c(UnkElem_0x48* elem, bool flag);

struct UnkElemArray_7100125338 {
    UnkElem_0x48* data;
    u32 count;
};

void fn_7100125338(UnkElemArray_7100125338* self, bool flag) {
    UnkElem_0x48* end = self->data + self->count;
    for (UnkElem_0x48* p = self->data; p != end; ++p)
        fn_7100119d3c(p, flag);
}
