// base::file::nx::CDeviceNX's unit (tools/tu_map.py unit 38): lists a directory of the device into a string
// list (directories, files or both, as asked), descending `depth` levels; 1 when the directory does not open.
// Names and layouts are placeholders.
#include <new>
#include <string.h>

#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU57.h"
#include "types.h"

// The SDK calls in the form the imports carry (results as s32).
namespace nn::fs {
struct DirectoryHandle {
    void* handle;
};
enum DirectoryEntryType_l02e6e0 : s8 { DirectoryEntryType_Directory = 0, DirectoryEntryType_File = 1 };
struct DirectoryEntry {
    char name[0x301];
    u8 pad_0x301[3];
    DirectoryEntryType_l02e6e0 type;  // 0x304
    u8 pad_0x305[3];
    s64 size;  // 0x308
};
static_assert(sizeof(DirectoryEntry) == 0x310, "entry");
s32 OpenDirectory(DirectoryHandle* out, const char* path, s32 mode);
s32 ReadDirectory(s64* count, DirectoryEntry* entries, DirectoryHandle handle, s64 max);
void CloseDirectory(DirectoryHandle handle);
}  // namespace nn::fs

using base::global::CRntString;

CRntString CDeviceNX_BuildFullPath(void* table, CRntString path);

// A list of strings: singly linked, allocated through its allocator.
struct UnkStringNode_l02e6e0 {
    CRntString value;
    UnkStringNode_l02e6e0* next;

    explicit UnkStringNode_l02e6e0(const CRntString& v) : value(v), next(nullptr) {}
};

struct UnkStringList_l02e6e0 {
    u8 pad_0x0[0x8];
    u32 count;  // 0x8
    u8 pad_0xc[0x4];
    UnkStringNode_l02e6e0* head;  // 0x10
    UnkStringNode_l02e6e0* tail;  // 0x18
    base::UnkAllocator* allocator;  // 0x20
    u32 flags;                      // 0x28

    void PushBack(const CRntString& value) {
        void* mem = allocator->vf_0x40(sizeof(UnkStringNode_l02e6e0), 8, flags);
        if (!mem)
            return;
        UnkStringNode_l02e6e0* node = new (mem) UnkStringNode_l02e6e0(value);
        if (count == 0) {
            head = node;
            tail = node;
            count = 1;
        } else {
            tail->next = node;
            node->next = nullptr;
            tail = node;
            ++count;
        }
    }
};

static inline const char* Text_l02e6e0(const CRntString& s) { return s.data ? s.data : g_unk_710162e2ed; }

// A buffer from the default allocator, freed when it goes out of scope.
template <class T>
struct UnkScopedArray_l02e6e0 {
    T* ptr;

    explicit UnkScopedArray_l02e6e0(u64 count) : ptr(static_cast<T*>(fn_7100083ba0(count * sizeof(T), 0x16))) {}
    ~UnkScopedArray_l02e6e0() {
        if (ptr)
            fn_7100083c30(ptr);
    }
};

s32 fn_710002e6e0(void* table, CRntString* path, s32 depth, bool dirs, bool files, UnkStringList_l02e6e0* out) {
    if (depth < 0)
        return 0;
    CRntString full = CDeviceNX_BuildFullPath(table, *path);
    UnkScopedArray_l02e6e0<char> name(0x100);
    UnkScopedArray_l02e6e0<nn::fs::DirectoryEntry> entries(0x80);
    nn::fs::DirectoryHandle dir;
    if (nn::fs::OpenDirectory(&dir, Text_l02e6e0(full), 3) != 0)
        return 1;
    s64 count = 0;
    s32 ret = 0;
    if (nn::fs::ReadDirectory(&count, entries.ptr, dir, 0x80) == 0) {
        for (s64 i = 0; i < count; ++i) {
            nn::fs::DirectoryEntry entry = entries.ptr[i];
            strcpy(name.ptr, entry.name);
            CRntString child(*path);
            if (path->data && path->data[0])
                CRntString_AppendChar(&child, '/');
            CRntString_AppendCString(&child, name.ptr);
            CRntString item(child);
            bool is_dir = entry.type == nn::fs::DirectoryEntryType_Directory;
            if ((is_dir && dirs) || (!is_dir && files))
                out->PushBack(item);
            if (is_dir && depth - 1 >= 0)
                ret = fn_710002e6e0(table, &item, depth - 1, dirs, files, out);
            if (ret == 0x7fffffff) {
                nn::fs::CloseDirectory(dir);
                return 0x7fffffff;
            }
        }
    }
    nn::fs::CloseDirectory(dir);
    return ret;
}
