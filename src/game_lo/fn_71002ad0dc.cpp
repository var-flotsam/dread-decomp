// Destructor of a large object: runs its shutdown helper, releases two refcounted pointers and
// nine registrations (each removes itself from the set it was added to), then destroys the
// secondary base at +0x10, which clears its inline array of 0x120-byte polymorphic elements.
#include "base/UnkDestructors.h"
#include "types.h"

extern char g_unk_7101a211b8[];  // vtable of the object
extern char g_unk_7101a21168[];  // vtable of the base at +0x10

void fn_71002adc8c(void* self);
void fn_71002aed2c(void* set, void* item);
void fn_71002aeb9c(void* set, void* item);
void fn_71002aea0c(void* set, void* item);
void fn_71002ae87c(void* set, void* item);

// A registration in a deferred set; removes itself on destruction.
template <void (*Remove)(void*, void*)>
struct UnkRegistration_71002ad0dc {
    void* set;
    void* field_0x8;
    void* field_0x10;

    ~UnkRegistration_71002ad0dc() {
        if (set) {
            Remove(set, this);
            set = nullptr;
        }
        field_0x10 = nullptr;
        field_0x8 = nullptr;
    }
};

// A refcounted pointer that is cleared after the release. The count is decremented through a
// plain int pointer: the original reloads the array count after the decrement.
struct UnkRefPtr_71002ad0dc {
    base::UnkRefCounted* ptr;

    ~UnkRefPtr_71002ad0dc() {
        if (ptr) {
            s32* count = &ptr->ref_count;
            if (--*count == 0)
                delete ptr;
            ptr = nullptr;
        }
    }
};

class UnkElem_71002ad0dc {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
};

struct UnkObject_71002ad0dc {
    char* vtable;
    u8 pad_0x8[0x8];
    char* base_vtable;                  // 0x10
    u32 count;                          // 0x18
    u8 elems[0x11978 - 0x1c];           // 0x1c, stride 0x120
    UnkRefPtr_71002ad0dc field_0x11978;
    u32 field_0x11980;
    u8 pad_0x11984[0x12988 - 0x11984];
    UnkRegistration_71002ad0dc<fn_71002ae87c> field_0x12988;
    UnkRegistration_71002ad0dc<fn_71002ae87c> field_0x129a0;
    UnkRegistration_71002ad0dc<fn_71002aea0c> field_0x129b8;
    UnkRegistration_71002ad0dc<fn_71002aea0c> field_0x129d0;
    UnkRegistration_71002ad0dc<fn_71002aeb9c> field_0x129e8;
    UnkRegistration_71002ad0dc<fn_71002aeb9c> field_0x12a00;
    UnkRegistration_71002ad0dc<fn_71002aeb9c> field_0x12a18;
    UnkRegistration_71002ad0dc<fn_71002aed2c> field_0x12a30;
    UnkRegistration_71002ad0dc<fn_71002aed2c> field_0x12a48;
    base::UnkRefPtrMember field_0x12a60;
    base::UnkRefPtrMember field_0x12a68;

    void ClearElems() {
        for (u64 i = 0; i < count; i++)
            reinterpret_cast<UnkElem_71002ad0dc*>(elems + i * 0x120)->vf_0x8();
        count = 0;
    }
};

void fn_71002ad0dc(UnkObject_71002ad0dc* self) {
    self->vtable = g_unk_7101a211b8 + 0x10;
    fn_71002adc8c(self);
    self->field_0x12a68.~UnkRefPtrMember();
    self->field_0x12a60.~UnkRefPtrMember();
    self->field_0x12a48.~UnkRegistration_71002ad0dc();
    self->field_0x12a30.~UnkRegistration_71002ad0dc();
    self->field_0x12a18.~UnkRegistration_71002ad0dc();
    self->field_0x12a00.~UnkRegistration_71002ad0dc();
    self->field_0x129e8.~UnkRegistration_71002ad0dc();
    self->field_0x129d0.~UnkRegistration_71002ad0dc();
    self->field_0x129b8.~UnkRegistration_71002ad0dc();
    self->field_0x129a0.~UnkRegistration_71002ad0dc();
    self->field_0x12988.~UnkRegistration_71002ad0dc();
    self->field_0x11980 = 0;
    self->base_vtable = g_unk_7101a21168 + 0x10;
    self->ClearElems();
    self->field_0x11978.~UnkRefPtr_71002ad0dc();
    self->ClearElems();
}
