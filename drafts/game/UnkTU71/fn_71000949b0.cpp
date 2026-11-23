// Unit 71 of tools/tu_map.py: encodes an owned-or-borrowed block as base64 text, appended to a string.
// Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "types.h"

// A block that is either owned (copied on assignment) or borrowed.
struct UnkBlob_u71 {
    u32 size;
    void* data;  // 0x8
    bool owns;   // 0x10
};

// Appends the base64 form of the block to `out`; false when it would be 512 characters or more.
bool fn_71000949b0(const UnkBlob_u71* self, base::global::CRntString* out) {
    const char* alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    u32 groups = self->size / 3 + (self->size % 3 != 0 ? 1 : 0);
    u32 length = groups * 4;
    u32 pos = 0;
    if (length < 512) {
        u16 count = length;
        for (u32 i = 0; i < count / 3; ++i) {
        const u8* bytes = static_cast<const u8*>(self->data);
        u32 v = (bytes[pos] << 16) | (bytes[pos + 1] << 8) | bytes[pos + 2];
        pos += 3;
        CRntString_AppendChar(out, alphabet[(v >> 18) & 0x3f]);
        CRntString_AppendChar(out, alphabet[(v >> 12) & 0x3f]);
        CRntString_AppendChar(out, alphabet[(v >> 6) & 0x3f]);
        CRntString_AppendChar(out, alphabet[v & 0x3f]);
        }
    } else {
        return false;
    }

    switch (self->size % 3) {
    case 1: {
        const u8* bytes = static_cast<const u8*>(self->data);
        u8 last = bytes[pos];
        CRntString_AppendChar(out, alphabet[last >> 2]);
        CRntString_AppendChar(out, alphabet[(last & 3) << 4]);
        CRntString_AppendChar(out, '=');
        CRntString_AppendChar(out, '=');
        break;
    }
    case 2: {
        const u8* bytes = static_cast<const u8*>(self->data);
        u32 v = (bytes[pos] << 16) | (bytes[pos + 1] << 8);
        CRntString_AppendChar(out, alphabet[(v >> 18) & 0x3f]);
        CRntString_AppendChar(out, alphabet[(v >> 12) & 0x3f]);
        CRntString_AppendChar(out, alphabet[(v >> 6) & 0x3f]);
        CRntString_AppendChar(out, '=');
        break;
    }
    }
    return true;
}
