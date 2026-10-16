// Constructor: base constructor, then this class's vtable (external, so via the GOT under PIC).
#include "types.h"

struct UnkBase_7100416db8 {
    UnkBase_7100416db8();
    virtual ~UnkBase_7100416db8();
};

struct Unk_fn_71005a2dd8 : UnkBase_7100416db8 {
    Unk_fn_71005a2dd8();
    ~Unk_fn_71005a2dd8() override;
};

Unk_fn_71005a2dd8::Unk_fn_71005a2dd8() {}
