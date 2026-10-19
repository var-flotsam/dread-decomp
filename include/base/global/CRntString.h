#pragma once

// base::global::CRntString: a string value type (0x30 bytes). The name comes from reflection
// (fn_7100007f84 describes it). Reflected default values and descriptions build one from a
// literal (fn_710007e704) or copy a global one (fn_710007ea60), and destroy it with
// fn_710007ecd4. The same three functions build the keys of the named registry
// (UnkNamedEntry in base/UnkNamedRegistry.h, the same layout). Its members are unknown.

#include "types.h"

namespace base::global {

class CRntString {
public:
    CRntString(const char* str);           // fn_710007e704
    CRntString(const CRntString& other);   // fn_710007ea60
    ~CRntString();                         // fn_710007ecd4

private:
    u8 unk_0x0[0x30];
};
static_assert(sizeof(CRntString) == 0x30, "CRntString layout");

}  // namespace base::global
