// CSpawnPointComponent unit (tools/tu_map.py): equality of two base::UnkVectors of 0x40-byte links.
// A link whose state (+0x30) is 1 is resolved first: its key (+0x8) is looked up (fn_71003df480,
// a counted reference), the reference stored in the handle slot at +0x38 (fn_71006eb8cc) and the
// state set to 2. Links are equal when their handles' targets (+0x10) are. Names and layouts are
// placeholders.
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "game/CBatalloonAIComponent.h"
#include "types.h"

struct UnkRefObj_c3 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // destroys the object
    s32 refs;
};

struct UnkRef_c3 {
    UnkRefObj_c3* p;
    ~UnkRef_c3() {
        if (p && --p->refs == 0)
            p->vf_0x10();
    }
};

struct UnkLinkHandle_c3 {
    u8 pad_0x0[0x10];
    void* target;
};

base::UnkRefHandle* fn_71006eb8cc(base::UnkRefHandle*, const base::UnkRefHandle*);  // as src/game_s7/fn_7100245b60.cpp

struct UnkLink_c3 {
    u8 pad_0x0[0x8];
    u8 key[0x28];
    u32 state;
    u32 pad_0x34;
    UnkLinkHandle_c3* handle;

    void* Get() {
        if (state == 1) {
            {
                UnkRef_c3 ref = fn_71003df480(key);
                fn_71006eb8cc(reinterpret_cast<base::UnkRefHandle*>(&handle), reinterpret_cast<const base::UnkRefHandle*>(&ref));
            }
            state = 2;
        }
        return handle ? handle->target : nullptr;
    }
};
static_assert(sizeof(UnkLink_c3) == 0x40, "UnkLink_c3 layout");

bool fn_710113192c(base::UnkVector<UnkLink_c3>* self, base::UnkVector<UnkLink_c3>* other) {
    if (self->count != other->count)
        return false;
    for (u64 i = 0; i < self->count; ++i) {
        UnkLink_c3& a = self->data[i];
        UnkLink_c3& b = other->data[i];
        void* target = a.Get();
        if (b.Get() != target)
            return false;
    }
    return true;
}
