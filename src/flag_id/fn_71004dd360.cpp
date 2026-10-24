// Initialisation: base init, copy values from a source object, then finish.
#include "types.h"

struct UnkTriple {
    u8 unk_0x0[0x70];
    u32 a, b, c;
};

struct UnkSource {
    u8 unk_0x0[0x44];
    u32 a;
    u8 unk_0x48[0xc];
    u32 b;
    u8 unk_0x58[0xc];
    u32 c;
    u8 unk_0x68[0x1f0 - 0x68];
    UnkTriple* triple;
};

struct UnkGlobal;
struct UnkStruct_0x68;

// Stored into the object below as a callback (the GOT slot 0x7101cdddd8 holds its address; the
// sample had named that slot). Its real signature is unknown: it takes the same object and reads
// its source and w0..w2.
void fn_71004dd73c(UnkStruct_0x68* self);

struct UnkVec3u {
    u32 x, y, z;
};

struct UnkStruct_0x68 {
    u8 unk_0x0[0x18];
    UnkSource* source;
    u8 unk_0x20[0xc];
    UnkVec3u v;
    u32 w0, w1, w2;
    u8 unk_0x44[0x4];
    UnkStruct_0x68* self_ref;
    UnkGlobal* global;
    u64 field_0x58;
    u32 field_0x60;
};

void fn_7100394bd0(UnkStruct_0x68* self);
void fn_71004dd3d8(UnkStruct_0x68* self);

void fn_71004dd360(UnkStruct_0x68* self) {
    fn_7100394bd0(self);
    UnkSource* src = self->source;
    self->v = UnkVec3u{src->a, src->b, src->c};
    self->w0 = 0;
    self->w1 = 0;
    self->w2 = 0;
    if (UnkTriple* t = src->triple) {
        self->w0 = t->a;
        self->w1 = t->b;
        self->w2 = t->c;
    }
    self->self_ref = self;
    self->global = reinterpret_cast<UnkGlobal*>(&fn_71004dd73c);
    self->field_0x58 = 0;
    self->field_0x60 = 0;
    fn_71004dd3d8(self);
}
