// Destructor of a class (vtable unk_7101a1b1b8) that, when active (flag at 8), clears the
// flag and stops the object at 0x18 through its slot 0x28; when it owns a mutex (flag at 0x20),
// finalizes it (fn_7100066918) and frees it (pointer at 0x28); then releases the string id at
// 0x10. It has no base destructor. Called only through the vtable.

#include <new>

#include "base/UnkDefaultAllocator.h"
#include "base/UnkDestructors.h"
#include "types.h"

extern char unk_7101a1b1b8[];

struct UnkStoppable_710008e8cc {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
};

struct UnkObject_710008e8cc {
    char* vtable;
    bool active;
    base::UnkStrIdMember id;
    UnkStoppable_710008e8cc* target;
    bool owns_mutex;
    nn::os::MutexType* mutex;
};

void fn_710008e8cc(UnkObject_710008e8cc* self) {
    self->vtable = unk_7101a1b1b8 + 0x10;
    if (self->active) {
        self->active = false;
        self->target->vf_0x28();
    }
    if (self->owns_mutex) {
        fn_7100066918(self->mutex);
        if (self->mutex) {
            operator delete(self->mutex);
            self->mutex = nullptr;
        }
    }
    using Id = base::UnkStrIdMember;
    self->id.~Id();
}
