// CDoorLifeComponent (unit 1376): the four kind masks at +0x644..+0x650, parsed from text (a list
// split at "|" by fn_7100414070, else a single name by fn_71008a40c0), then each applied through
// fn_71008e22f8. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "types.h"

using base::global::CRntString;

u32 fn_7100414070(const char* text);
u32 fn_71008a40c0(const char* text);

struct UnkDoorLifeKinds {
    u8 pad_0x0[0x644];
    u32 kinds[4];  // 0x644
};

void fn_71008e22f8(UnkDoorLifeKinds* self, u32 kind);

static inline u32 ParseKind(const char* text) {
    CRntString str(text);
    return fn_710007fb9c(&str, "|") ? fn_7100414070(text) : fn_71008a40c0(text);
}

void fn_71008d3cb4(UnkDoorLifeKinds* self, const char* const* a, const char* const* b, const char* const* c,
                   const char* const* d) {
    self->kinds[0] = ParseKind(*a);
    self->kinds[1] = ParseKind(*b);
    self->kinds[2] = ParseKind(*c);
    self->kinds[3] = ParseKind(*d);
    fn_71008e22f8(self, self->kinds[0]);
    fn_71008e22f8(self, self->kinds[1]);
    fn_71008e22f8(self, self->kinds[2]);
    fn_71008e22f8(self, self->kinds[3]);
}
