// DRAFT (not matching): the original reloads the found element before releasing it (ours reuses the
// compared value) and swaps the index/offset registers (x28/x19). Breaking out of the search and erasing
// after the loop was worse (control flow).
// Unit 2577 (CSubAreaManager): releases the file paths a sub-area setup lists, from the file device,
// the sound manager and the manager's own list of loaded paths, then goes on to the setup's next list.
// Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/file/UnkFileDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using UnkPathIdMember = base::UnkStrIdMemberOf<g_unk_7101d47200>;  // a file-path id

struct UnkSubAreaMgrPaths {
    u8 pad_0x0[0x388];
    base::UnkVector<UnkPathIdMember> field_0x388;  // the loaded paths
};

struct UnkSubAreaSetupLists {
    u8 pad_0x0[0xa0];
    base::UnkVector<UnkPathIdMember> field_0xa0;
    u8 pad_0xc8[8];
    u32 field_0xd0;
};
static_assert(__builtin_offsetof(UnkSubAreaSetupLists, field_0xd0) == 0xd0);

extern base::file::UnkFileDevice* g_unk_7101d4d828;
struct UnkSoundMgr;
extern UnkSoundMgr* g_unk_7101d4d820;
void fn_710008987c(base::file::UnkFileDevice* self, const CFilePathStrId* path, u32 flags);
void fn_7100108c94(UnkSoundMgr* self, const CFilePathStrId* path);
bool fn_7100089848(base::file::UnkFileDevice* self, const CFilePathStrId* path);
void fn_710008985c(base::file::UnkFileDevice* self, const CFilePathStrId* path);
UnkPathIdMember* fn_710008c174(base::UnkVector<UnkPathIdMember>* self, u32 index);
void fn_7100ee9f1c(UnkSubAreaMgrPaths* self, UnkSubAreaSetupLists* lists);

// Both report whether the path was released; the other users ignore the result.
static inline bool ReleaseFromDevice(base::file::UnkFileDevice* device, const CFilePathStrId* path) {
    return reinterpret_cast<bool (*)(base::file::UnkFileDevice*, const CFilePathStrId*, u32)>(fn_710008987c)(
        device, path, 0);
}
static inline bool ReleaseFromSound(const CFilePathStrId* path) {
    return reinterpret_cast<bool (*)(UnkSoundMgr*, const CFilePathStrId*)>(fn_7100108c94)(g_unk_7101d4d820, path);
}

// Destroys the loaded path at `index` and moves the later ones down.
static inline void EraseLoaded(UnkSubAreaMgrPaths* self, u64 index) {
    if ((u32)index < self->field_0x388.count) {
        self->field_0x388.data[index].~UnkPathIdMember();
        for (; index < self->field_0x388.count - 1; ++index) {
            UnkPathIdMember* data = self->field_0x388.data;
            new (&data[index]) UnkPathIdMember(static_cast<UnkPathIdMember&&>(data[index + 1]));
            self->field_0x388.data[index + 1].~UnkPathIdMember();
        }
        self->field_0x388.count = self->field_0x388.count - 1;
    }
}

void fn_7100ee9488(UnkSubAreaMgrPaths* self, UnkSubAreaSetupLists* lists) {
    base::file::UnkFileDevice* device = g_unk_7101d4d828;
    if (!device) {
        device = static_cast<base::file::UnkFileDevice*>(fn_71000839a0(0x1348, 0x3f));
        fn_71000893a8(device);
        g_unk_7101d4d828 = device;
    }

    for (u32 i = lists->field_0xa0.count; i != 0;) {
        --i;
        UnkPathIdMember* id = fn_710008c174(&lists->field_0xa0, i);
        const CFilePathStrId* path = reinterpret_cast<const CFilePathStrId*>(id);
        bool released = ReleaseFromDevice(device, path);
        released &= ReleaseFromSound(path);

        if (released || !fn_7100089848(device, path)) {
            u32 count = self->field_0x388.count;
            for (u64 j = 0; j != count; ++j) {
                if (self->field_0x388.data[j] == *id) {
                    EraseLoaded(self, j);
                    break;
                }
            }
        }
        if (!released && fn_7100089848(device, path))
            fn_710008985c(device, path);
    }

    if (lists->field_0xd0 != 0)
        fn_7100ee9f1c(self, lists);
}
