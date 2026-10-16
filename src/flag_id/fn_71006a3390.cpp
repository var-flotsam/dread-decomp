// Constructor: base constructor, vtable, then zero one field.
#include "types.h"

struct UnkBase_71003641fc {
    UnkBase_71003641fc();
    virtual ~UnkBase_71003641fc();
    u8 unk_0x8[0x48];
};

struct Unk_fn_71006a3390 : UnkBase_71003641fc {
    Unk_fn_71006a3390();
    ~Unk_fn_71006a3390() override;
    u32 field_0x50;
};

Unk_fn_71006a3390::Unk_fn_71006a3390() : field_0x50(0) {}
