// behaviortree::CScaredAction (tools/tu_map.py): the node's start. After the base routine
// (fn_7100394bd0) it marks itself running (+0x30) and, when its owner (+0x18) has an animation
// component (+0x220), listens to it: the link at +0x38 calls fn_7100390860 with the node, through the weak
// handle at +0x50 (dropped first, then taken from the component). Finally copies +0x2c to +0x34.
// Names and layouts are placeholders.
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkSelf;
void fn_7100394bd0(UnkSelf*);
void fn_7100316018(UnkWeakTarget* self, UnkWeakHandle** out);
void fn_710117b3e8(UnkWeakTarget* target, void* link);
struct UnkHandler;
struct UnkMessage;
u32 fn_7100390860(UnkHandler* self, UnkMessage* message);  // fn_7100390860.cpp

struct UnkScaredLink_b49 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkScaredOwner_b49 {
    u8 pad_0x0[0x220];
    UnkWeakTarget* animation;
};

struct UnkScaredAction_b49 {
    u8 pad_0x0[0x18];
    UnkScaredOwner_b49* owner;
    u8 pad_0x20[0xc];
    u32 field_0x2c;
    u32 state;
    u32 field_0x34;
    UnkScaredLink_b49 link;
    UnkWeakHandle* handle;
};

void fn_710038ff68(UnkScaredAction_b49* self) {
    fn_7100394bd0(reinterpret_cast<UnkSelf*>(self));
    self->state = 1;
    UnkScaredOwner_b49* owner = self->owner;
    if (owner && owner->animation) {
        UnkScaredLink_b49* link = &self->link;
        link->context = self;
        link->callback = reinterpret_cast<void (*)()>(fn_7100390860);
        // Read again after the link's stores (addressed by offset, as the original reloads it).
        UnkWeakTarget* animation = *reinterpret_cast<UnkWeakTarget**>(reinterpret_cast<char*>(owner) + 0x220);
        if (UnkWeakHandle* handle = self->handle) {
            if (handle->target) {
                fn_710117b3f0(handle->target, link);
                if (!self->handle)
                    goto take;
            }
            ResetRef(&self->handle);
        }
    take:
        if (animation) {
            fn_7100316018(animation, reinterpret_cast<UnkWeakHandle**>(reinterpret_cast<char*>(self) + 0x50));
            fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
        }
    }
    self->field_0x34 = self->field_0x2c;
}
