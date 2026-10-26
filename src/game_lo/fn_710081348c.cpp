// Releases an object: returns the handle at field_0x38 (if any) to a lazily created manager,
// with an id taken from the description at field_0x0, then releases the path id at field_0x8.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

struct UnkManager_710081348c {
    u8 pad_0x0[0x1e8];
};
void fn_71011a870c(UnkManager_710081348c* self);
void fn_71011a91c4(UnkManager_710081348c* self, void* handle, u32 id);
extern UnkManager_710081348c* g_unk_71726b0cd8;  // the manager instance (reached through the GOT)

static inline UnkManager_710081348c* GetManager() {
    if (!g_unk_71726b0cd8) {
        UnkManager_710081348c* m = static_cast<UnkManager_710081348c*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

struct UnkStruct_710081348c_Desc {
    u8 pad_0x0[0x40];
    u32 field_0x40;
};

struct UnkStruct_710081348c {
    UnkStruct_710081348c_Desc* field_0x0;
    base::UnkStrIdMemberOf<g_unk_7101d47200> field_0x8;
    u8 pad_0x10[0x28];
    void* field_0x38;
};

void fn_710081348c(UnkStruct_710081348c* self) {
    if (self->field_0x38) {
        fn_71011a91c4(GetManager(), self->field_0x38, self->field_0x0->field_0x40);
        self->field_0x38 = nullptr;
    }
    self->field_0x8.~UnkStrIdMemberOf();
}
