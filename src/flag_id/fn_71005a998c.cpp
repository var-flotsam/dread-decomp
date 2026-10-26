// Shutdown step, then release two intrusively reference-counted members.
#include "game/UnkBehaviorTree.h"
#include "types.h"

struct UnkRefCounted {
    virtual void unk_vf0();
    virtual void unk_vf1();
    virtual void Destroy();
    s32 ref_count;
};

template <class T>
struct UnkRefPtr {
    T* ptr;

    void Reset() {
        if (ptr) {
            if (--ptr->ref_count == 0)
                ptr->Destroy();
            ptr = nullptr;
        }
    }
};

struct UnkStruct_0x147e8 : UnkBehaviorTreeAI {
    u8 unk_0x38[0x147d8 - sizeof(UnkBehaviorTreeAI)];
    UnkRefPtr<UnkRefCounted> first;
    UnkRefPtr<UnkRefCounted> second;
};

void fn_71005a998c(UnkStruct_0x147e8* self) {
    fn_71003695b8(self);
    self->first.Reset();
    self->second.Reset();
}
