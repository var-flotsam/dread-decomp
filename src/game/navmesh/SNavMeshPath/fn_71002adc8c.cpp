// Shutdown helper of the large object destroyed by fn_71002ad0dc: destroys its inline array of
// 0x120-byte polymorphic elements, releases a refcounted pointer, resets some state, runs
// fn_71002ad688, and when the handle at +0x12a68 points at an object whose +0x10 is set, removes
// three registrations from their set; then releases that handle, clears two words and
// continues with fn_71002addd4.
#include "base/UnkDestructors.h"
#include "game/UnkNavMeshPath.h"
#include "types.h"

void fn_71002aeb9c(void* set, void* item);
void fn_71002addd4(void* self);

// A registration in a deferred set (layout as in fn_71002ad0dc).
struct UnkRegistration_71002adc8c {
    void* set;
    void* field_0x8;
    void* field_0x10;

    void Unregister() {
        if (set) {
            fn_71002aeb9c(set, this);
            set = nullptr;
        }
    }
};

// A refcounted pointer that is cleared after the release.
struct UnkRefPtr_71002adc8c {
    base::UnkRefCounted* ptr;

    void Reset() {
        if (ptr) {
            if (--ptr->ref_count == 0)
                delete ptr;
            ptr = nullptr;
        }
    }
};

class UnkElem_71002adc8c {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
};

struct UnkObject_71002adc8c {
    char* vtable;
    bool field_0x8;
    u8 pad_0x9[0x7];
    char* base_vtable;                  // 0x10
    u32 count;                          // 0x18
    u8 elems[0x11978 - 0x1c];           // 0x1c, stride 0x120
    UnkRefPtr_71002adc8c field_0x11978;
    u32 field_0x11980;
    u8 pad_0x11984[0x129e8 - 0x11984];
    UnkRegistration_71002adc8c field_0x129e8;
    UnkRegistration_71002adc8c field_0x12a00;
    UnkRegistration_71002adc8c field_0x12a18;
    u8 pad_0x12a30[0x12a68 - 0x12a30];
    UnkRefPtr_71002adc8c field_0x12a68;
    u64 field_0x12a70;
    u64 field_0x12a78;
};

void fn_71002adc8c(UnkObject_71002adc8c* self) {
    for (u64 i = 0; i < self->count; i++)
        reinterpret_cast<UnkElem_71002adc8c*>(self->elems + i * 0x120)->vf_0x8();
    self->count = 0;
    self->field_0x11978.Reset();
    self->field_0x11980 = 0;
    self->field_0x8 = false;
    fn_71002ad688(reinterpret_cast<UnkNavMeshPath*>(self), nullptr);
    if (base::UnkRefCounted* h = self->field_0x12a68.ptr) {
        if (*reinterpret_cast<void**>(reinterpret_cast<char*>(h) + 0x10)) {
            self->field_0x129e8.Unregister();
            self->field_0x12a00.Unregister();
            self->field_0x12a18.Unregister();
        }
        self->field_0x12a68.Reset();
    }
    self->field_0x12a70 = 0;
    self->field_0x12a78 = 0;
    fn_71002addd4(self);
}
