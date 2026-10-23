// Constructor of a class (vtable unk_7101a1a478) above the base built by fn_7100023838: three
// empty string ids at 8, 0x10 and 0x18 (fn_71000003d4), a cleared flag at 0x20, then a call of
// its own slot 0x18. fn_710039b35c is a relative. It has no direct callers.

#include "types.h"

extern char unk_7101a1a478[];
void fn_7100023838(void* self);
void fn_71000003d4(void* self, const char* s, bool flag);

struct UnkObject_7100138090 {
    void (**vtable)(UnkObject_7100138090*);
    u64 id_0x8;
    u64 id_0x10;
    u64 id_0x18;
    bool flag_0x20;
};

void fn_7100138090(UnkObject_7100138090* self) {
    fn_7100023838(self);
    self->vtable = reinterpret_cast<void (**)(UnkObject_7100138090*)>(unk_7101a1a478 + 0x10);
    fn_71000003d4(&self->id_0x8, "", true);
    fn_71000003d4(&self->id_0x10, "", true);
    fn_71000003d4(&self->id_0x18, "", true);
    self->flag_0x20 = false;
    self->vtable[3](self);
}
