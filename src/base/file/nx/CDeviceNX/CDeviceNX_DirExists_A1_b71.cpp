// A method of the unit of base::file::nx::CDeviceNX (the lookup fn_710002e0f8 calls): builds the
// table's full path for `path` (fn_710002df80) and tries to open it as a directory: 0 when it opens
// (it is closed again), 1 when the path is not found (result 0x202), 4 for any other failure.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "types.h"

// The SDK calls in the form the imports carry (results as s32, as in src/engine/SystemStartup.cpp).
namespace nn::fs {
struct DirectoryHandle {
    void* handle;
};
s32 OpenDirectory(DirectoryHandle* out, const char* path, s32 mode);
void CloseDirectory(DirectoryHandle handle);
}  // namespace nn::fs

base::global::CRntString fn_710002df80(void* table, base::global::CRntString path);

u32 fn_710002e168(void* table, base::global::CRntString* path) {
    base::global::CRntString full = fn_710002df80(table, *path);
    nn::fs::DirectoryHandle handle;
    s32 result = nn::fs::OpenDirectory(&handle, full.data ? full.data : g_unk_710162e2ed, 1);
    u32 ret;
    if (result != 0) {
        ret = (result & 0x3fffff) == 0x202 ? 1 : 4;
    } else {
        nn::fs::CloseDirectory(handle);
        ret = 0;
    }
    return ret;
}
