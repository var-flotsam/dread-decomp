// Unit 71 of tools/tu_map.py (0x7100094910-0x7100094db4; no reliable class name): a block that is either
// owned or borrowed (copy assignment, and decoding base64 text into it), and the reflection type
// descriptor for a value-type id. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "types.h"

// A block that is either owned (copied on assignment) or borrowed.
struct UnkBlob_u71 {
    u32 size;
    void* data;  // 0x8
    bool owns;   // 0x10
};

UnkBlob_u71* fn_7100094910(UnkBlob_u71* self, const UnkBlob_u71* other) {
    if (self->owns) {
        u32 size = self->size;
        u32 other_size = other->size;
        if (size != other_size) {
            if (self->data) {
                fn_7100083c30(self->data);
                self->data = nullptr;
            }
            self->data = nullptr;
            self->size = 0;
            self->owns = true;
            self->data = fn_7100083ba0(other_size, 0xf);
            self->size = other_size;
            size = other->size;
        }
        __builtin_memcpy(self->data, other->data, size);
    } else {
        self->data = other->data;
        self->size = other->size;
        self->owns = false;
    }
    return self;
}

// The reflection type descriptor for a value-type id (0x49 to 0x79), or null.
#include "base/reflection/UnkReflAttributes.h"

using base::reflection::CType;

const CType* fn_7100024324();
const CType* fn_7100010e58();
void* fn_710000ffd0();
extern CType g_unk_7101d4dad8;

// Not a tail call in the original (probably a conversion in the source); disable_tail_calls stands in.
__attribute__((disable_tail_calls)) const CType* fn_7100094cf0(u32 id) {
    const CType* type = nullptr;
    switch (id) {
    case 0x49:
        type = fn_7100024324();
        break;
    case 0x55:
        type = fn_7100010e58();
        break;
    case 0x62:
        type = fn_710000d228();
        break;
    case 0x63:
        type = static_cast<const CType*>(fn_710000ffd0());
        break;
    case 0x66:
        type = fn_710000abbc();
        break;
    case 0x69:
        type = fn_7100014768();
        break;
    case 0x73:
        type = fn_7100007f84();
        break;
    case 0x75:
        type = fn_7100005178();
        break;
    case 0x76:
        type = fn_71000246e4();
        break;
    case 0x77:
        type = fn_71000248c4();
        break;
    case 0x79:
        type = fn_7100024504();
        break;
    case 0x61:
        type = &g_unk_7101d4dad8;
        break;
    }
    return type;
}

u8 fn_71000943c0(u8 c);

// Replaces the block with a new owned one of `size` bytes.
static inline __attribute__((always_inline)) void Alloc_u71(UnkBlob_u71* self, u32 size) {
    self->data = nullptr;
    self->size = 0;
    self->owns = true;
    self->data = fn_7100083ba0(size, 0xf);
    self->size = size;
}

// Decodes base64 text into the block.
void fn_7100094b98(UnkBlob_u71* self, const char* text) {
    u32 length = __builtin_strlen(text);
    u32 padding = 0;
    if (length != 0) {
        padding = text[length - 1] == '=';
        if (text[length - 2] == '=')
            padding = padding + 1;
    }
    u32 size = length / 4 * 3 - padding;
    if (self->owns) {
        if (self->data) {
            fn_7100083c30(self->data);
            self->data = nullptr;
        }
    }
    Alloc_u71(self, size);
    u32 out = 1;
    for (u32 i = 0; i < length;) {
        u32 first = out - 1;
        u32 i1 = i + 1;
        u32 a = fn_71000943c0(text[i]);
        u32 i2 = i + 2;
        u32 b = fn_71000943c0(text[i1]);
        u32 i3 = i + 3;
        u32 c = fn_71000943c0(text[i2]);
        i += 4;
        u32 d = fn_71000943c0(text[i3]);
        u32 v = (b << 12) + (a << 18) + (c << 6) + d;
        static_cast<u8*>(self->data)[first] = v >> 16;
        static_cast<u8*>(self->data)[out] = v >> 8;
        static_cast<u8*>(self->data)[out + 1] = v;
        out += 3;
    }
}
