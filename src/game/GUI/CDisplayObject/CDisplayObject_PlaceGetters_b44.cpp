// GUI::CDisplayObject unit (tools/tu_map.py, the name from reflection): the eight reflected getters
// matching the setters in CDisplayObject_PlaceSetters.cpp: each returns the x or y coordinate of the
// object's placement when it is measured by the given mode (0 to 3), NaN otherwise, as a float value.
// Names and layouts are placeholders.
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::reflection::UnkReflValue;

// The placement: the two modes and the position.
struct UnkGuiPlacement_b44a3 {
    u8 pad_0x0[0x78];
    u32 mode[2];
    f32 pos[2];
};

template <u32 Axis, u32 Mode>
static inline __attribute__((always_inline)) UnkReflValue GetPlace_b44a3(char* obj) {
    auto* p = *reinterpret_cast<UnkGuiPlacement_b44a3**>(obj + 8);
    f32 value = p->mode[Axis] == Mode ? p->pos[Axis] : __builtin_nanf("");
    const base::reflection::CType* type = fn_710000abbc();
    UnkReflValue r;
    r.kind = 2;
    r.field_0x0 = nullptr;
    r.type = nullptr;
    fn_71000a18d4(&r, type, &value, false);
    return r;
}

UnkReflValue fn_71011d4178(void*, char* obj) { return GetPlace_b44a3<0, 0>(obj); }
UnkReflValue fn_71011d4668(void*, char* obj) { return GetPlace_b44a3<0, 1>(obj); }
UnkReflValue fn_71011d4318(void*, char* obj) { return GetPlace_b44a3<0, 2>(obj); }
UnkReflValue fn_71011d44c0(void*, char* obj) { return GetPlace_b44a3<0, 3>(obj); }
UnkReflValue fn_71011d4810(void*, char* obj) { return GetPlace_b44a3<1, 0>(obj); }
UnkReflValue fn_71011d4d00(void*, char* obj) { return GetPlace_b44a3<1, 1>(obj); }
UnkReflValue fn_71011d49b0(void*, char* obj) { return GetPlace_b44a3<1, 2>(obj); }
UnkReflValue fn_71011d4b58(void*, char* obj) { return GetPlace_b44a3<1, 3>(obj); }
