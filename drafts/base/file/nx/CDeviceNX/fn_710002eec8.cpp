// base::file::nx::CDeviceNX's unit (tools/tu_map.py unit 38): writes a whole file at the device's full path,
// first creating its directory when asked (and committing the save data after each step when asked); 0 on
// success, 0x7fffffff when the directory could not be made, 4 for any other failure. Names and layouts are
// placeholders.
#include "base/file/nx/CDeviceNX.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "game/UnkTU57.h"
#include "types.h"

// The SDK calls in the form the imports carry (results as s32).
namespace nn::fs {
struct FileHandle {
    void* handle;
};
struct WriteOption {
    s32 flags;
};
s32 CreateDirectory(const char* path);
s32 CreateFile(const char* path, s64 size);
s32 OpenFile(FileHandle* out, const char* path, s32 mode);
s32 WriteFile(FileHandle handle, s64 offset, const void* buffer, u64 size, const WriteOption& option);
s32 SetFileSize(FileHandle handle, s64 size);
void CloseFile(FileHandle handle);
s32 CommitSaveData(const char* name);
}  // namespace nn::fs

using base::global::CRntString;

CRntString CDeviceNX_BuildFullPath(void* table, CRntString path);
u32 CDeviceNX_CheckDirectoryExists(void* table, CRntString* path);
bool fn_710008e374(const CRntString* path, CRntString* dir, CRntString* name, CRntString* ext);

// A string built empty out of line (fn_710007e6c4).
struct UnkEmptyString_l02eec8 {
    u8 str[0x30];

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    UnkEmptyString_l02eec8() { fn_710007e6c4(str); }
    ~UnkEmptyString_l02eec8() { Str()->~CRntString(); }
};

static inline const char* Text_l02eec8(const CRntString& s) { return s.data ? s.data : g_unk_710162e2ed; }

// The directory part of a path.
inline void SplitDirectory_l02eec8(const CRntString& path, CRntString* dir) {
    CRntString name(Text_l02eec8(path));
    fn_710008e374(&name, dir, nullptr, nullptr);
}

// Creates the device's directory `dir` when it is not there yet; 0 when it is there (or was made).
inline s32 EnsureDirectory_l02eec8(void* table, CRntString* dir, bool commit) {
    s32 ret = 0;
    if (CDeviceNX_CheckDirectoryExists(table, dir) == 1) {
        CRntString full = CDeviceNX_BuildFullPath(table, *dir);
        bool failed = nn::fs::CreateDirectory(Text_l02eec8(full)) != 0;
        if (!failed && commit)
            failed |= nn::fs::CommitSaveData("savedata") != 0;
        ret = failed ? 0x7fffffff : 0;
    }
    return ret;
}

s32 fn_710002eec8(void* table, void* path, void* data, u64 size, bool commit, bool create) {
    CRntString full = CDeviceNX_BuildFullPath(table, *static_cast<CRntString*>(path));
    if (create) {
        UnkEmptyString_l02eec8 dir;
        SplitDirectory_l02eec8(full, dir.Str());
        if (s32 ret = EnsureDirectory_l02eec8(table, dir.Str(), commit))
            return ret;
    }
    s32 result = nn::fs::CreateFile(Text_l02eec8(full), size);
    if (result == 0 || (result & 0x3fffff) == 0x402) {
        nn::fs::FileHandle file;
        result = nn::fs::OpenFile(&file, Text_l02eec8(full), 6);
        if (result == 0) {
            nn::fs::WriteOption option = {1};
            result = nn::fs::WriteFile(file, 0, data, size, option);
            if (result != 0) {
                nn::fs::CloseFile(file);
            } else {
                result = nn::fs::SetFileSize(file, size);
                nn::fs::CloseFile(file);
                if (result == 0 && commit)
                    result = nn::fs::CommitSaveData("savedata");
            }
        }
    }
    return result != 0 ? 4 : 0;
}
