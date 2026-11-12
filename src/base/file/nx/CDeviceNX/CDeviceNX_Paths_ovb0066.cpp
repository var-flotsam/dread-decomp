// base::file::nx::CDeviceNX's unit (tools/tu_map.py unit 38): the device's full path for a relative
// one (fn_710002df80: the root at the start of the table is prepended unless the path already holds
// it, and backslashes become slashes), a file's existence and size (fn_710002eb04, fn_710002ec40),
// reading a whole file into a new buffer (fn_710002ed98) and deleting a file, optionally committing
// the save data (fn_710002f210). nn::fs results are taken as s32 (0 on success). Names and layouts
// are placeholders.
#include "base/UnkBuffer.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "types.h"

#include "../../../../game_aca57c/UnkBufferString.h"

namespace nn::fs {
struct FileHandle {
    void* handle;
};
s32 OpenFile(FileHandle* out, const char* path, s32 mode);
s32 GetFileSize(s64* out, FileHandle handle);
s32 ReadFile(u64* out, FileHandle handle, s64 offset, void* buffer, u64 size);
void CloseFile(FileHandle handle);
s32 DeleteFile(const char* path);
s32 CommitSaveData(const char* name);
}  // namespace nn::fs

using base::global::CRntString;

s32 fn_710007facc(const CRntString* self, char c, u32 start);

// The device's root path (a string at the start of the table).
CRntString fn_710002df80(void* table, CRntString path) {
    const CRntString* root = static_cast<const CRntString*>(table);
    if (!fn_710007fb9c(&path, root->data ? root->data : g_unk_710162e2ed)) {
        CRntString full = fn_710007eee0(root, &path);
        fn_710007eaa0(&path, &full);
    }
    if (fn_710007facc(&path, '\\', 0) != -1) {
        char text[0x200];
        u32 length = path.length;
        text[length] = '\0';
        for (u32 i = 0; i < length; i++) {
            if (path.data[i] == '\\')
                text[i] = '/';
            else
                text[i] = path.data[i];
        }
        UnkBufferString<0x100, g_unk_7101a194b8> converted(text);
        fn_710007eaa0(&path, converted.Str());
    }
    return static_cast<CRntString&&>(path);
}

struct UnkDeviceFile_ovb0066 {
    u8 unk_0x0[0x10];
    CRntString path;  // 0x10
    u8 unk_0x40[0x4];
    s32 use_count;  // 0x44
};

// The holder is read through a union, which keeps the file reloaded after the count's store.
union UnkDeviceFileRef_ovb0066 {
    UnkDeviceFile_ovb0066* file;
    u8* bytes;
};

static inline const CRntString& Use_ovb0066(UnkDeviceFileRef_ovb0066* ref) {
    ref->file->use_count++;
    return *reinterpret_cast<const CRntString*>(ref->bytes + 0x10);
}

s32 fn_710002f210(u8* self, UnkDeviceFileRef_ovb0066* handle, bool commit) {
    void* table = self + 0x10;
    CRntString full = fn_710002df80(table, Use_ovb0066(handle));
    s32 result = nn::fs::DeleteFile(full.data ? full.data : g_unk_710162e2ed);
    if (result == 0 && commit)
        result = nn::fs::CommitSaveData("savedata");
    return result == 0 ? 0 : 0x7fffffff;
}

struct UnkFileInfo_ovb0066 {
    bool exists;
    u32 mask;  // 0x4
    s64 size;  // 0x8
    u64 field_0x10;
    u64 field_0x18;
    u64 field_0x20;
    u64 field_0x28;
};

u32 fn_710002eb04(void* table, CRntString* path, UnkFileInfo_ovb0066* info, const u32* wanted) {
    if (!path->data || !path->data[0]) {
        info->exists = false;
        return 0;
    }
    CRntString full = fn_710002df80(table, *path);
    nn::fs::FileHandle file;
    if (nn::fs::OpenFile(&file, full.data ? full.data : g_unk_710162e2ed, 1) == 0) {
        s64 size = 0;
        nn::fs::GetFileSize(&size, file);
        info->exists = size > 0;
        if (size >= 1) {
            if (*wanted & 1) {
                info->size = size;
                info->mask |= 1;
            }
            if (*wanted & 2) {
                info->field_0x10 = 0;
                info->mask |= 2;
            }
            if (*wanted & 4) {
                info->field_0x18 = 0;
                info->mask |= 4;
            }
            if (*wanted & 8) {
                info->field_0x20 = 0;
                info->mask |= 8;
            }
        }
        nn::fs::CloseFile(file);
    }
    return 0;
}

struct UnkWanted_ovb0066 {
    u32 bits;
    UnkWanted_ovb0066() { bits = 0; }
};

inline const UnkWanted_ovb0066& NoneWanted_ovb0066() {
    static UnkWanted_ovb0066 s_none;
    return s_none;
}

u32 fn_710002ec40(void* table, CRntString* path) {
    UnkFileInfo_ovb0066 info;
    info.exists = false;
    info.mask = 0;
    alignas(8) u32 wanted = NoneWanted_ovb0066().bits;
    fn_710002eb04(table, path, &info, &wanted);
    return info.exists ^ 1;
}

class UnkFileBuffer_ovb0066 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void* vf_0x38();
};

void fn_71000810cc(base::UnkBuffer* self, u64 size, base::UnkAllocator* allocator, s32 align, u32 flags);

u32 fn_710002ed98(void* table, CRntString* path, void* out, u32 b, void* allocator, u32 align, u32 flags) {
    CRntString full = fn_710002df80(table, *path);
    nn::fs::FileHandle file;
    u32 ret = 1;
    if (nn::fs::OpenFile(&file, full.data ? full.data : g_unk_710162e2ed, 1) == 0) {
        s64 size = 0;
        nn::fs::GetFileSize(&size, file);
        void* mem = fn_71000839a0(0x28, 3);
        fn_71000810cc(static_cast<base::UnkBuffer*>(mem), size, static_cast<base::UnkAllocator*>(allocator),
                      align, flags);
        UnkFileBuffer_ovb0066* buffer = static_cast<UnkFileBuffer_ovb0066*>(mem);
        ret = 0x7fffffff;
        if (buffer) {
            void* data = buffer->vf_0x38();
            u64 read = 0;
            if (nn::fs::ReadFile(&read, file, 0, data, size) == 0 && read == (u64)size) {
                ret = 0;
                *static_cast<UnkFileBuffer_ovb0066**>(out) = buffer;
            } else {
                buffer->vf_0x8();
            }
        }
        nn::fs::CloseFile(file);
    }
    return ret;
}
