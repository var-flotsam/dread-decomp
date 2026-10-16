// nn::atk::detail::fnd::FileStreamProxy vtable slot 0x60: forwards to the wrapped stream,
// keeping only the low 32 bits of its result.
#include "types.h"

struct UnkStream {
    virtual void unk_vf0();
    virtual void unk_vf1();
    virtual void unk_vf2();
    virtual void unk_vf3();
    virtual void unk_vf4();
    virtual void unk_vf5();
    virtual void unk_vf6();
    virtual void unk_vf7();
    virtual void unk_vf8();
    virtual void unk_vf9();
    virtual void unk_vf10();
    virtual void unk_vf11();
    virtual u64 unk_vf12();
};

struct UnkFileStreamProxy {
    u8 unk_0x0[0x8];
    UnkStream* stream;
};

u64 fn_710123578c(UnkFileStreamProxy* self) {
    return static_cast<u32>(self->stream->unk_vf12());
}
