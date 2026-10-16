// Constructor: base, vtable, a name member built from a literal, a -1 default, then
// a virtual initialisation call (slot 3).
#include "types.h"

struct UnkName {
    UnkName(const char* str, bool flag);
    ~UnkName();
    void* data;
};

struct UnkBase_7100393e68 {
    UnkBase_7100393e68();
    virtual ~UnkBase_7100393e68();
    virtual void unk_vf2();
    virtual void Init();
    u8 unk_0x8[0x28];
};

struct Unk_fn_710037415c : UnkBase_7100393e68 {
    Unk_fn_710037415c();
    ~Unk_fn_710037415c() override;
    UnkName name;
    f32 field_0x38;
};

Unk_fn_710037415c::Unk_fn_710037415c() : name("unk", true), field_0x38(-1.0f) {
    Init();
}
