// Unit 182 (the device layer): rebuilds a vertex layout's NVN states. Resets the 16 attribute and 16
// stream states, then gives each bound element with an attribute location its attribute (stream,
// format, offset 0) and a stream of its own, with the element's stride (or its size from its format)
// and no divisor. Records the stream count and the highest location. Names and layouts are
// placeholders.
#include "types.h"

struct UnkNvnVertexAttribState {
    u32 data;
};

struct UnkNvnVertexStreamState {
    u64 data;
};


// An element's type (1 is two bytes per component, 2 and 3 four) and component count.
struct UnkVertexFormat_l161fd0 {
    u16 type;
    u16 count;
};

struct UnkVertexElement_l161fd0 {
    u8 pad_0x0[0x18];
    u32 stride;  // 0x18: 0 when the format gives it
    s32 format;  // 0x1c: the NVN format
    u8 pad_0x20[0x8];
    UnkVertexFormat_l161fd0 layout;  // 0x28
};

struct UnkVertexLayout_l161fd0 {
    u8 pad_0x0[0x2c];
    u32 stream_count;                         // 0x2c
    u32 max_location;                         // 0x30
    u8 pad_0x34[0x4];
    UnkVertexElement_l161fd0* elements[16];  // 0x38
    UnkNvnVertexAttribState attribs[16];      // 0xb8
    UnkNvnVertexStreamState streams[16];      // 0xf8
};
static_assert(__builtin_offsetof(UnkVertexLayout_l161fd0, attribs) == 0xb8, "layout");
static_assert(__builtin_offsetof(UnkVertexLayout_l161fd0, streams) == 0xf8, "layout");

// The attribute location of element slot `index`, or a negative value when it has none.
s32 fn_710015e950(u32 index);

// NVN entry points (reached through the GOT).
extern void (*g_unk_716e29b6f0)(UnkNvnVertexAttribState* state);  // set defaults
extern void (*g_unk_716e29b6f8)(UnkNvnVertexAttribState* state, s32 format, s64 offset);
extern void (*g_unk_716e29b700)(UnkNvnVertexAttribState* state, s32 stream);
extern void (*g_unk_716e29b718)(UnkNvnVertexStreamState* state);  // set defaults
extern void (*g_unk_716e29b720)(UnkNvnVertexStreamState* state, s32 stride);
extern void (*g_unk_716e29b728)(UnkNvnVertexStreamState* state, s32 divisor);

// Through a wrapper: the arguments are evaluated before the entry point is loaded.
template <class R, class... A, class... B>
static inline R CallNvn(R (*&fn)(A...), B... args) {
    return fn(args...);
}

// An element's stride: its own, or else the size of its components (their count times their size).
static inline u32 Stride(const UnkVertexElement_l161fd0* element) {
    if (element->stride)
        return element->stride;
    UnkVertexFormat_l161fd0 format = element->layout;
    switch (format.type) {
    case 2:
    case 3:
        return format.count * 4;
    case 1:
        return format.count * 2;
    default:
        return 0;
    }
}

void fn_7100161fd0(UnkVertexLayout_l161fd0* self) {
    for (u32 i = 0; i < 16; ++i) {
        CallNvn(g_unk_716e29b6f0, &self->attribs[i]);
        CallNvn(g_unk_716e29b718, &self->streams[i]);
    }
    self->max_location = 0;
    u32 streams = 0;
    for (u32 i = 0; i < 16; ++i) {
        if (UnkVertexElement_l161fd0* element = self->elements[i]) {
            s32 location = fn_710015e950(i);
            if (location >= 0) {
                UnkNvnVertexAttribState* attrib = &self->attribs[location];
                CallNvn(g_unk_716e29b6f0, attrib);
                CallNvn(g_unk_716e29b700, attrib, (s32)streams);
                CallNvn(g_unk_716e29b6f8, attrib, element->format, (s64)0);
                if (self->max_location < (u32)location)
                    self->max_location = location;
                UnkNvnVertexStreamState* stream = &self->streams[streams];
                CallNvn(g_unk_716e29b718, stream);
                CallNvn(g_unk_716e29b720, stream, (s32)Stride(element));
                CallNvn(g_unk_716e29b728, stream, 0);
                ++streams;
            }
        }
        self->stream_count = streams;
    }
}
