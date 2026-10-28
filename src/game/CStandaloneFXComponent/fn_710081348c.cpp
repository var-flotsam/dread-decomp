// Releases an object: returns the handle at field_0x38 (if any) to a lazily created manager,
// with an id taken from the description at field_0x0, then releases the path id at field_0x8.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

#include "game/UnkModelManager.h"

// Defined in game_hi with its own view of the manager (its container half).
void fn_71011a91c4(UnkModelManager* self, void* handle, u32 id);

static inline UnkModelManager* GetManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
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
