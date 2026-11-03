// A list of reference-counted pointers at +0x14740 of an owner (tools/tu_map.py unit 1102, no
// class name). fn_71006eb700 appends one: grow by the step, reserve (fn_71006ebe54), clear the new
// slot, then assign it (take a reference, drop the old one). Names and layouts are placeholders.
#include "types.h"

typedef u32 __attribute__((may_alias)) UnkRefCount_b50;

struct UnkRefObj_b50 {
    virtual void vf_0();
    virtual void vf_1();
    virtual void vf_2();
    u32 ref_count;
};

struct UnkRefPtr_b50 {
    UnkRefObj_b50* ptr;

    UnkRefPtr_b50& operator=(const UnkRefPtr_b50& other) {
        UnkRefObj_b50* p = other.ptr;
        if (p) {
            ++*(UnkRefCount_b50*)&p->ref_count;
            if (UnkRefObj_b50* old = ptr) {
                if (--old->ref_count == 0)
                    old->vf_2();
            }
            ptr = p;
        }
        return *this;
    }
};

struct UnkRefPtrList_b50 {
    UnkRefPtr_b50* data;
    u32 count;
    u32 capacity;
    u32 step;
};

void fn_71006ebe54(UnkRefPtrList_b50* list, u32 capacity);

void fn_71006eb700(char* owner, const UnkRefPtr_b50& value) {
    UnkRefPtrList_b50* list = (UnkRefPtrList_b50*)(owner + 0x14740);
    u32 n = list->count;
    u32 new_capacity = list->capacity;
    if (new_capacity <= n && list->step != 0)
        new_capacity = (n / list->step + 1) * list->step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    fn_71006ebe54(list, new_capacity);
    UnkRefPtr_b50* slot = list->data + list->count;
    __builtin_memset(slot, 0, sizeof(*slot));
    *slot = value;
    ++list->count;
}
