// nn::atk-ish unit 2772: loads a sound-archive file into memory from the given heap, unless the loader
// already holds it, and registers it with the loader. Names and layouts are placeholders.
#include "game/UnkTU2772.h"
#include "types.h"

// The heap a file is loaded into: the size an allocation of `size` bytes takes (vf_0x20), then the
// allocation itself (vf_0x10).
struct UnkAtkHeap_l211d30 {
    virtual void vf_0x0() = 0;
    virtual void vf_0x8() = 0;
    virtual void* Alloc(u64 size) = 0;  // 0x10
    virtual void vf_0x18() = 0;
    virtual u64 AllocSize(u64 size, bool flag) = 0;  // 0x20
};

struct UnkAtkLoader {
    virtual void vf_0x0() = 0;
    virtual void vf_0x8() = 0;
    virtual void Register(u32 file_id, void* data) = 0;  // 0x10
    virtual void vf_0x18() = 0;
    virtual void* Find(u32 file_id) = 0;  // 0x20
    UnkAtkArchive* archive;  // 0x8
};

// A file's entry in the archive: its size (all ones when unknown).
struct UnkAtkFileInfo_l211d30 {
    u32 size;
    u32 field_0x4;
    void* field_0x8;
};

bool fn_7101207020(UnkAtkArchive* archive, u32 file_id, UnkAtkFileInfo_l211d30* info);
u64 fn_710121212c(UnkAtkLoader* self, u32 file_id, void* dst, u64 size, u32 flag, void* arg);
void fn_7101215460(void* data, u64 size);

void* fn_7101211d30(UnkAtkLoader* self, u32 file_id, void* heap_arg, void* arg, u32 unk) {
    UnkAtkHeap_l211d30* heap = static_cast<UnkAtkHeap_l211d30*>(heap_arg);
    void* data = self->Find(file_id);
    if (!data) {
        UnkAtkFileInfo_l211d30 info;
        info.size = 0xffffffff;
        info.field_0x4 = 0xffffffff;
        info.field_0x8 = nullptr;
        if (!fn_7101207020(self->archive, file_id, &info))
            return nullptr;
        u32 size = info.size;
        if (size == 0 || size == 0xffffffff)
            return nullptr;
        data = heap->Alloc(heap->AllocSize(size, unk & 1));
        if (!data)
            return nullptr;
        // The read takes the size as a signed int.
        if (fn_710121212c(self, file_id, data, static_cast<s32>(size), 0, arg) != static_cast<u64>(static_cast<s32>(size)))
            return nullptr;
        fn_7101215460(data, size);
        self->Register(file_id, data);
    }
    return data;
}