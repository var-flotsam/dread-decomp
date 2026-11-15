// nn::atk unit 2772: opens a stream on an archive entry, seeks to a position and reads a number of bytes into a
// buffer, in chunks of at most a given size when one is set. Answers the size, or 0 when it could not be read.
// Names and layouts are placeholders.
#include "types.h"

struct UnkAtkStream_l21212c {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual u64 Read(void* buffer, u64 size, s32* result);  // 0x20
    virtual void vf_0x28();
    virtual void Seek(s64 position, s32 origin);  // 0x30
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual bool vf_0x48();
    virtual void vf_0x50();
    virtual bool vf_0x58();
};

struct UnkAtkReader_l21212c {
    u8 pad_0x0[0x8];
    void* archive;  // 0x8
    u8 name[1];     // 0x10
};

UnkAtkStream_l21212c* fn_7101207098(void* archive, u64 arg, void* name, u32 size, u64 p4, u64 p5);

u64 fn_710121212c(UnkAtkReader_l21212c* self, u64 arg, u8* buffer, u64 size, s32 position, u64 chunk) {
    UnkAtkStream_l21212c* stream = fn_7101207098(self->archive, arg, self->name, 0x200, 0, 0);
    if (!stream || !stream->vf_0x58() || !stream->vf_0x48())
        return 0;
    stream->Seek(position, 0);
    s32 whole_result;
    s32 chunk_result;
    if (chunk == 0) {
        whole_result = 0;
        stream->Read(buffer, size, &whole_result);
        if (whole_result < 0)
            return 0;
        return size;
    }
    u64 remaining = size;
    while (remaining != 0) {
        chunk_result = 0;
        u64 n = stream->Read(buffer, remaining < chunk ? remaining : chunk, &chunk_result);
        if (chunk_result < 0)
            return 0;
        buffer += n;
        remaining = remaining > n ? remaining - n : 0;
    }
    return size;
}
