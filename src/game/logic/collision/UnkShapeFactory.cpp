// From unit 2505 of data/tu_map.csv (0x7100d45df0-0x7100d46804, no class hint), in the 2D collision
// code (game::logic::collision): fn_7100d462b4 reads a collision shape from a stream, creating the
// shape its type name names (an aligned or oriented box, a segment, a circle, a capsule, a compound
// or a polygon collection), handing it to its owner and letting it read the rest.
// Names and layouts are placeholders.
#include <string.h>

#include "base/UnkVector.h"
#include "types.h"

// The next string of the stream, in place (src/game_s7/UnkPropS7.h).
s64 fn_710007cee4(void* stream, const char** out);

struct UnkCollisionShape {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20(void* stream);  // reads the shape
};

// The shapes' constructors.
void fn_7100d5e1b4(void* self);  // AABOX2D
void fn_7100d6936c(void* self);  // SEGMENT2D
void fn_7100d61664(void* self);  // CIRCLE
void fn_7100d5ff7c(void* self);  // CAPSULE2D
void fn_7100d61d90(void* self);  // COMPOUND2D
void fn_7100d682dc(void* self);  // OBOX2D
void fn_7100d62b04(void* self);  // POLYCOLLECTION2D

struct UnkShapeOwner;
// Gives the shape to its owner (which then owns it).
void fn_7100d36da4(UnkShapeOwner* self, UnkCollisionShape* shape, bool owned);

template <void (*Construct)(void*)>
static inline UnkCollisionShape* NewShape(u64 size) {
    void* p = fn_71000839a0(size, 7);
    Construct(p);
    return static_cast<UnkCollisionShape*>(p);
}

// Reads a shape of the type named in the stream into `owner`; an unknown name reads nothing more.
void fn_7100d462b4(void*, UnkShapeOwner* owner, void* stream) {
    const char* type;
    fn_710007cee4(stream, &type);
    UnkCollisionShape* shape;
    if (strcmp(type, "AABOX2D") == 0)
        shape = NewShape<fn_7100d5e1b4>(0xc0);
    else if (strcmp(type, "SEGMENT2D") == 0)
        shape = NewShape<fn_7100d6936c>(0xc0);
    else if (strcmp(type, "CIRCLE") == 0)
        shape = NewShape<fn_7100d61664>(0xb0);
    else if (strcmp(type, "CAPSULE2D") == 0)
        shape = NewShape<fn_7100d5ff7c>(0xc0);
    else if (strcmp(type, "COMPOUND2D") == 0)
        shape = NewShape<fn_7100d61d90>(0xd0);
    else if (strcmp(type, "OBOX2D") == 0)
        shape = NewShape<fn_7100d682dc>(0xd8);
    else if (strcmp(type, "POLYCOLLECTION2D") == 0)
        shape = NewShape<fn_7100d62b04>(0x120);
    else
        return;
    fn_7100d36da4(owner, shape, true);
    shape->vf_0x20(stream);
}
