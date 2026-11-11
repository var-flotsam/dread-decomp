// CEmmyWakeUpComponent unit (tools/tu_map.py): the component's destructor (vtable g_unk_7101a72a18): the listener at
// +0xc0 unregisters from its link's owner (fn_710117b3f0) and drops the link, the callback set at +0x80 drops its
// reference and destroys its values, then the base destructor (fn_71007d70d8). Names and layouts are placeholders.
#include "base/UnkErasedVector.h"
#include "base/UnkVector.h"
#include "game/CAbilityGhostAura.h"
#include "game/CEmmyWakeUpComponent.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

template <u32 IdOffset>
struct UnkActivatable_b44a2;
void fn_71007d70d8(UnkActivatable_b44a2<0x48>* self);  // the activatable component's destructor

// A counted link to the object a listener is registered with.
struct UnkWakeUpLink_b0129 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;
    UnkWeakTarget* owner;  // 0x10
};

// The listener: it unregisters from the link's owner and drops the link.
struct UnkWakeUpListener_b0129 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkWakeUpLink_b0129* link;  // 0x18

    __attribute__((always_inline)) ~UnkWakeUpListener_b0129() {
        vtable = g_unk_7101a22d48 + 2;
        UnkWakeUpLink_b0129* l = link;
        if (!l)
            return;
        if (UnkWeakTarget* owner = l->owner) {
            fn_710117b3f0(owner, this);
            l = link;
            if (!l)
                return;
        }
        if (--l->ref_count == 0)
            l->Release();
    }
};

struct UnkWakeUpRef_b0129 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    s32 ref_count;
};

// A callback set (the array of type-erased values) with a counted reference.
struct UnkWakeUpCallbacks_b0129 {
    void** vtable;
    UnkOwnedErasedVector values;  // 0x8
    u32 field_0x30;
    UnkWakeUpRef_b0129* ref;  // 0x38

    __attribute__((always_inline)) ~UnkWakeUpCallbacks_b0129() {
        vtable = g_unk_7101a20950 + 2;
        if (UnkWakeUpRef_b0129* r = ref) {
            if (--r->ref_count == 0)
                r->Release();
        }
        vtable = g_unk_7101a20998 + 2;
    }
};

struct UnkEmmyWakeUp_b0129 {
    void** vtable;
    u8 pad_0x8[0x78];
    UnkWakeUpCallbacks_b0129 callbacks;  // 0x80
    UnkWakeUpListener_b0129 listener;    // 0xc0
};
static_assert(__builtin_offsetof(UnkEmmyWakeUp_b0129, listener) == 0xc0, "layout");

// The component's destructor: the listener, the callback set, then the base destructor.
void fn_71007d6f94(UnkEmmyWakeUp_b0129* self) {
    self->vtable = g_unk_7101a72a18 + 2;
    self->listener.~UnkWakeUpListener_b0129();
    self->callbacks.~UnkWakeUpCallbacks_b0129();
    fn_71007d70d8(reinterpret_cast<UnkActivatable_b44a2<0x48>*>(self));
}
