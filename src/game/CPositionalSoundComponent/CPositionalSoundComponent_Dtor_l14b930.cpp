// CPositionalSoundComponent unit (tools/tu_map.py, the name from reflection): the destructor. Both
// sound handles are stopped, the shared object at +0xc8 is released, then the ids, then the base.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/snd/UnkSound.h"
#include "game/CPositionalSoundComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkTU101.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkSoundRef {
    UnkSound* object;
    s64 serial;
};

static inline UnkSound* GetSound_l14b930(const UnkSoundHandle* handle) {
    if (handle->ref && handle->ref->serial == handle->serial && handle->serial != -1)
        return handle->ref->object;
    return nullptr;
}

// A reference-counted object: released through its third virtual when the count reaches zero.
struct UnkRefObject_l14b930 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();
    s32 ref_count;
};

struct UnkPositionalSound_l14b930 {
    void** vtable;
    u8 unk_0x8[0x38];
    void** vtable_0x40;
    u8 unk_0x48[8];
    CFilePathStrId paths[4];  // 0x50
    u8 unk_0x70[0x18];
    UnkSoundHandle handles[2];  // 0x88
    u8 unk_0xa8[8];
    CStrId field_0xb0;
    u8 unk_0xb8[4];
    f32 field_0xbc;
    u8 unk_0xc0[8];
    UnkRefObject_l14b930* field_0xc8;
};
static_assert(__builtin_offsetof(UnkPositionalSound_l14b930, field_0xc8) == 0xc8, "layout");

void fn_710114b930(UnkPositionalSound_l14b930* self) {
    self->vtable = g_unk_7101cb3570 + 2;
    self->vtable_0x40 = g_unk_7101cb3570 + 52;
    if (UnkSound* sound = GetSound_l14b930(&self->handles[0])) {
        fn_7100105a80(sound, self->field_0xbc, 0.0f);
        self->handles[0] = g_unk_7108b6c460;
    }
    if (UnkSound* sound = GetSound_l14b930(&self->handles[1])) {
        fn_7100105034(sound);
        self->handles[1] = g_unk_7108b6c460;
    }
    if (UnkRefObject_l14b930* object = self->field_0xc8) {
        if (--object->ref_count == 0)
            object->Release();
    }
    self->field_0xb0.~CStrId();
    for (int i = 4; i-- > 0;)
        self->paths[i].~CFilePathStrId();
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}
