// The pack destructor and deleting destructor of tools/tu_map.py unit 65 (the packs of
// src/base/file/CPackManagerDevice_3.cpp; the constructor is fn_7100088dfc,
// src/base/file/UnkMountedPack_Ctor_A1_b71.cpp): sets the vtable, destroys the reader at +0x28, drops the
// file (its vf_0x8), takes the pack off the serial counter and releases the path id. Names and layouts are
// placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

extern void* g_unk_7101a1b058[];                                    // the pack's vtable
extern s32 g_unk_7101d5954c __attribute__((visibility("hidden")));  // the next pack serial number
void fn_710008c91c(char* reader);
void fn_710008c85c(char* reader);

struct UnkPackFile_b0231 {
    virtual void vf_0x0();
    virtual void vf_0x8();
};

struct UnkPackDtor_b0231 {
    void* vtable;
    u8 pad_0x8[0x10];
    base::UnkStrIdMemberOf<g_unk_7101d47200> path;  // 0x18
    UnkPackFile_b0231* file;                       // 0x20
    char reader[0xa0];                             // 0x28

    __attribute__((always_inline)) void Destroy() {
        vtable = reinterpret_cast<char*>(g_unk_7101a1b058) + 0x10;
        fn_710008c91c(reader);
        if (file)
            file->vf_0x8();
        --g_unk_7101d5954c;
        fn_710008c85c(reader);
        path.~UnkStrIdMemberOf();
    }
};

void fn_7100088e8c(UnkPackDtor_b0231* self) {
    self->Destroy();
}

void fn_7100088f3c(UnkPackDtor_b0231* self) {
    self->Destroy();
    __builtin_operator_delete(self);
}
