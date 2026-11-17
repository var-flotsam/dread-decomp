// Unit 2514 (the COMPOUND2D collision shape): reads the shape from a file. After the base part,
// reads a count and that many child shapes, each created from its type name, added to the
// compound with the compound as its owner, and read in turn; then lets the shape update itself.
// Names and layouts are placeholders.
#include <string.h>

#include <new>

#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "game/UnkTU2517.h"
#include "game/UnkTU56.h"
#include "game/logic/collision/CAABoxShape2D.h"
#include "game/logic/collision/CCapsuleShape2D.h"
#include "game/logic/collision/CCircleShape2D.h"
#include "game/logic/collision/COBoxShape2D.h"
#include "game/logic/collision/CShape.h"
#include "types.h"

using base::global::CRntFile;

struct UnkCompoundShape2D;

// A 2D collision shape as the compound sees its children.
struct UnkChildShape2D {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual bool Load(CRntFile* file);  // vf_0x20
    u8 pad_0x8[0x70];
    UnkCompoundShape2D* owner;                    // 0x78
    void (*on_changed)(UnkCompoundShape2D* self);  // 0x80
};
static_assert(__builtin_offsetof(UnkChildShape2D, owner) == 0x78, "UnkChildShape2D layout");

// What the children call when they change: the compound's own update (its vf_0x80).
void fn_7100d62560(UnkCompoundShape2D* self);

extern void* g_unk_7101aa8ad0[];

// A list of up to four children in inline storage (4-byte aligned, right after the count).
struct UnkChildShapeList {
    u32 count;
    u32 storage[8];

    UnkChildShape2D** Data() { return reinterpret_cast<UnkChildShape2D**>(storage); }
    // Constructs a new empty slot when the count can grow; returns the last slot.
    UnkChildShape2D*& Add() {
        u32 i = count;
        if (i != 0xffffffff) {
            new (&Data()[i]) UnkChildShape2D*(nullptr);
            count = i + 1;
        }
        return Data()[i];
    }
};

struct UnkCompoundShape2D {
    void** vtable;
    u8 pad_0x8[0xa0];
    UnkChildShapeList children;  // 0xa8

    void AddChild(UnkChildShape2D* shape) {
        children.Add() = shape;
        shape->owner = this;
        shape->on_changed = fn_7100d62560;
    }
    void Update() { reinterpret_cast<void (*)(UnkCompoundShape2D*)>(vtable[0x80 / 8])(this); }
};
static_assert(__builtin_offsetof(UnkCompoundShape2D, children) == 0xa8, "UnkCompoundShape2D layout");

bool fn_7100d61e94(UnkCompoundShape2D* self, CRntFile* file);

template <void (*Construct)(void*)>
static inline UnkChildShape2D* NewShape(u64 size) {
    void* p = fn_7100083b60(size);
    Construct(p);
    return static_cast<UnkChildShape2D*>(p);
}

static inline UnkCompoundShape2D* NewCompoundShape() {
    UnkCompoundShape2D* shape = static_cast<UnkCompoundShape2D*>(fn_7100083b60(0xd0));
    fn_7100d6bfd8(shape);
    shape->vtable = &g_unk_7101aa8ad0[2];
    shape->children.count = 0;
    return shape;
}

bool fn_7100d61e94(UnkCompoundShape2D* self, CRntFile* file) {
    fn_7100d6c288(reinterpret_cast<UnkPolygonCollectionShape*>(self), file);
    s32 count = 0;
    if (file->position < file->size)
        fn_710007c714(file, &count, sizeof(count), 1);
    for (s32 i = 0; i < count; ++i) {
        const char* type;
        fn_710007cee4(file, &type);
        UnkChildShape2D* shape;
        if (strcmp(type, "AABOX2D") == 0) {
            shape = NewShape<fn_7100d5e1b4>(0xc0);
        } else if (strcmp(type, "SEGMENT2D") == 0) {
            shape = NewShape<fn_7100d6936c>(0xc0);
        } else if (strcmp(type, "CIRCLE") == 0) {
            shape = NewShape<fn_7100d61664>(0xb0);
        } else if (strcmp(type, "CAPSULE2D") == 0) {
            shape = NewShape<fn_7100d5ff7c>(0xc0);
        } else if (strcmp(type, "COMPOUND2D") == 0) {
            UnkCompoundShape2D* compound = NewCompoundShape();
            self->AddChild(reinterpret_cast<UnkChildShape2D*>(compound));
            fn_7100d61e94(compound, file);
            continue;
        } else if (strcmp(type, "OBOX2D") == 0) {
            shape = NewShape<fn_7100d682dc>(0xd8);
        } else {
            continue;
        }
        self->AddChild(shape);
        shape->Load(file);
    }
    self->Update();
    return true;
}
