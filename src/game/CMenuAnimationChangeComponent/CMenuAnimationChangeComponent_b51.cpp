// CMenuAnimationChangeComponent (unit 1423): hooking the component up to the object its owner tracks.
#include "game_s2/UnkWeakHandle.h"

struct UnkMenuAnimOwner {
    u8 pad_0x0[0x220];
    UnkWeakTarget* field_0x220;
};

struct UnkMenuAnimLink {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkMenuAnimChange {
    u8 pad_0x0[0x30];
    UnkMenuAnimOwner* owner;
    u8 pad_0x38[0x8];
    UnkMenuAnimLink link;    // 0x40
    UnkWeakHandle* handle;   // 0x58
};

void fn_7100932ed0();
void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710117b3e8(UnkWeakTarget* target, void* link);

// Registers the change callback with the object the owner tracks (nothing when there is none).
void fn_7100932b48(UnkMenuAnimChange* self) {
    UnkWeakTarget* track = self->owner->field_0x220;
    if (!track)
        return;
    UnkMenuAnimLink* link = &self->link;
    link->context = self;
    link->callback = fn_7100932ed0;
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* target = handle->target)
            fn_710117b3f0(target, link);
        if (UnkWeakHandle* p = self->handle) {
            if (--p->ref_count == 0)
                p->vf_0x10();
            self->handle = nullptr;
        }
    }
    // The slot is addressed by its offset.
    fn_7100316018(track, reinterpret_cast<UnkWeakHandle**>(reinterpret_cast<char*>(self) + 0x58));
    fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
}
