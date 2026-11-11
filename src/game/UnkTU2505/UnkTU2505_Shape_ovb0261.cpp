// Unit 2505 (tools/tu_map.py, no reliable name): builds a 2D collision shape from the kind name in a
// serialized description and lets it read its own data. Names and layouts are placeholders.
#include <string.h>

#include "base/UnkVector.h"
#include "base/global/CRntFile.h"
#include "game/UnkTU2514.h"
#include "game/UnkTU2517.h"
#include "game/logic/collision/CAABoxShape2D.h"
#include "game/logic/collision/CCapsuleShape2D.h"
#include "game/logic/collision/CCircleShape2D.h"
#include "game/logic/collision/COBoxShape2D.h"
#include "game/logic/collision/CPolygonCollectionShape.h"
#include "types.h"

struct UnkCollisionFileReader;
struct UnkShapeReader_b0261;

using base::global::CRntFile;

// A shape built from a serialized description.
struct UnkShape_b0261 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(CRntFile* file);
};

// The shape handed back to the caller (returned in memory).
struct UnkShapeHandle_b0261 {
    UnkShape_b0261* shape;
    UnkShapeHandle_b0261() : shape(nullptr) {}
    UnkShapeHandle_b0261(const UnkShapeHandle_b0261& other) : shape(other.shape) {}
};

s64 fn_710007cee4(void* reader, const char** out);

template <u32 Size, void (*Ctor)(void*)>
static inline __attribute__((always_inline)) void Build_b0261(UnkShapeHandle_b0261* out, CRntFile* file) {
    void* memory = fn_71000839a0(Size, 7);
    Ctor(memory);
    out->shape = static_cast<UnkShape_b0261*>(memory);
    if (out->shape)
        out->shape->vf_0x20(file);
}

UnkShapeHandle_b0261 fn_7100d46440(UnkCollisionFileReader* self, CRntFile* file) {
    UnkShapeHandle_b0261 result;
    const char* kind;
    fn_710007cee4(file, &kind);
    if (!strcmp(kind, "AABOX2D"))
        Build_b0261<0xc0, fn_7100d5e1b4>(&result, file);
    else if (!strcmp(kind, "SEGMENT2D"))
        Build_b0261<0xc0, fn_7100d6936c>(&result, file);
    else if (!strcmp(kind, "CIRCLE"))
        Build_b0261<0xb0, fn_7100d61664>(&result, file);
    else if (!strcmp(kind, "CAPSULE2D"))
        Build_b0261<0xc0, fn_7100d5ff7c>(&result, file);
    else if (!strcmp(kind, "COMPOUND2D"))
        Build_b0261<0xd0, fn_7100d61d90>(&result, file);
    else if (!strcmp(kind, "OBOX2D"))
        Build_b0261<0xd8, fn_7100d682dc>(&result, file);
    else if (!strcmp(kind, "POLYCOLLECTION2D"))
        Build_b0261<0x120, fn_7100d62b04>(&result, file);
    return result;
}
