// base::file::CThreadSafeDevice's unit (unit 68 of tools/tu_map.py): file paths. fn_710008dd5c
// joins a device name and a path as "device:path" into a file-path id (the path alone without a
// device); fn_710008e374 splits a path into its directory (with the slash), name and extension.
// Names and layouts are placeholders.
#include "../../../game_aca57c/UnkBufferString.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/CStrId.h"
#include "base/input/CRumbleManager/CTunableRumbleManager.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;

void fn_710007f564(void* self, const char* str);  // appends a string

// A device path: the device's name and the path on it.
struct UnkDevicePath_ovb0109 {
    CStrId device;        // 0x0
    CFilePathStrId path;  // 0x8
};

// A file-path id, either copied or built from a string out of line (fn_710001a270).
struct UnkPathId_ovb0109 {
    union {
        CFilePathStrId id;
    };

    UnkPathId_ovb0109(const CFilePathStrId& other) : id(other) {}
    explicit UnkPathId_ovb0109(const CRntString& str) { fn_710001a270(&id, str, true); }
    ~UnkPathId_ovb0109() { id.~CFilePathStrId(); }
};

// The text of an id's pooled data (a CRntString at +0x10), the way the string data is shared: its use
// count (+0x44) is raised. The count is written untyped: the original reads the id again afterwards.
template <class Id>
static inline const char* IdChars(const Id& id) {
    u8* data = reinterpret_cast<u8*>(id.data);
    u32 uses;
    __builtin_memcpy(&uses, data + 0x44, sizeof(uses));
    ++uses;
    __builtin_memcpy(data + 0x44, &uses, sizeof(uses));
    const char* s = reinterpret_cast<const CRntString*>(reinterpret_cast<u8*>(id.data) + 0x10)->data;
    return s ? s : g_unk_710162e2ed;
}

// The full path, "device:path", or the path alone without a device.
UnkPathId_ovb0109 fn_710008dd5c(const UnkDevicePath_ovb0109* self) {
    if (self->device.data == &g_unk_7101d47108->empty)
        return UnkPathId_ovb0109(self->path);
    CRntString text(IdChars(self->device));
    fn_710007f5ac(&text, ':');
    fn_710007f564(&text, IdChars(self->path));
    return UnkPathId_ovb0109(text);
}

s32 fn_710007facc(const CRntString* self, char c, u32 start);
s32 fn_710007fb10(const base::global::CRntString* self, char c, s32 start);

// A substring, on 0x200 bytes of inline storage.
struct UnkPathPart_ovb0109 : UnkBufferString<0x200, g_unk_7101a1a5a8> {
    __attribute__((always_inline)) UnkPathPart_ovb0109(const char* str, u32 length) {
        fn_710007e7f8(Str(), str, length);
    }
};

// The empty id's text (its read count at +0x44 raised).
static inline const CRntString* EmptyText() {
    base::global::UnkStringData* data = g_unk_7101d470e0.data;
    ++*reinterpret_cast<s32*>(reinterpret_cast<u8*>(data) + 0x44);
    return reinterpret_cast<const CRntString*>(reinterpret_cast<u8*>(data) + 0x10);
}

// Splits "device:dir/name.ext" into the directory (with its slash), the name and the extension; each
// output may be null.
bool fn_710008e374(const CRntString* path, CRntString* dir, CRntString* name, CRntString* ext) {
    s32 colon = fn_710007facc(path, ':', 0);
    s32 slash = fn_710007fb10(path, '/', -1);
    s32 dot = fn_710007fb10(path, '.', -1);
    if (dir) {
        if (slash < 0)
            fn_710007eaa0(dir, EmptyText());
        else
            fn_710007eaa0(dir, UnkPathPart_ovb0109(path->data + (u32)(colon + 1), slash - colon).Str());
    }
    if (ext) {
        if (dot < 0)
            fn_710007eaa0(ext, EmptyText());
        else
        {
            u32 start = dot + 1;
            u32 length = path->length - start;
            fn_710007eaa0(ext, UnkPathPart_ovb0109(path->data + start, length).Str());
        }
    }
    if (name) {
        u32 start = slash + 1;
        s32 end = dot == -1 ? (s32)path->length : dot;
        u32 length = end - start;
        const char* text = path->data;
        fn_710007eaa0(name, UnkPathPart_ovb0109(text + start, length).Str());
    }
    return true;
}
