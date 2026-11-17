// CSluggerSpitAttack's unit (unit 1060): its copy assignment: CAttack's fields and the plain ones copied,
// the weak references made to share the source target's weak handle, and the targets member assigned
// unless it is assigned to itself. Names and layouts are placeholders.
#include "game/CAttack.h"
#include "game/UnkTU365.h"
#include "base/math/CVector.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// Two words copied together.
struct UnkSpitPair {
    u64 field_0x0;
    u64 field_0x8;
};

// A polymorphic member with an owner.
struct UnkSpitListener {
    virtual ~UnkSpitListener();
    UnkSpitPair field_0x8;
    UnkWeakHandle* owner;
};

// Words assigned one by one.
template <int N>
struct UnkSpitWords {
    u32 w[N];

    UnkSpitWords& operator=(const UnkSpitWords& other) {
        for (int i = 0; i < N; i++)
            w[i] = other.w[i];
        return *this;
    }
};

// A slot assigned by fn_71002a5d1c.
struct UnkSpitSlot {
    void* ptr;

    UnkSpitSlot& operator=(const UnkSpitSlot& other) {
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&other));
        return *this;
    }
};

// The 0x48-byte member at +0x88.
struct UnkSpitTargets {
    UnkSpitSlot field_0x0;
    UnkSpitSlot field_0x8;
    UnkSpitWords<13> field_0x10;

    UnkSpitTargets& operator=(const UnkSpitTargets& other) {
        if (this != &other) {
            field_0x0 = other.field_0x0;
            field_0x8 = other.field_0x8;
            field_0x10 = other.field_0x10;
        }
        return *this;
    }
};
static_assert(sizeof(UnkSpitTargets) == 0x48, "UnkSpitTargets layout");

struct UnkSluggerSpitAttack_l6c3e40 : CAttack {
    ~UnkSluggerSpitAttack_l6c3e40() override;
    base::math::CVector3D field_0x28;
    UnkSpitListener field_0x38;
    UnkSpitWords<10> field_0x58;
    bool field_0x80;
    UnkSpitTargets field_0x88;
    UnkSpitPair field_0xd0;
    UnkWeakHandle* field_0xe0;
};
static_assert(sizeof(UnkSluggerSpitAttack_l6c3e40) == 0xe8, "CSluggerSpitAttack layout");

UnkSluggerSpitAttack_l6c3e40* fn_71006c3e40(UnkSluggerSpitAttack_l6c3e40* self,
                                            const UnkSluggerSpitAttack_l6c3e40* other) {
    static_cast<CAttack&>(*self) = *other;
    self->field_0x28 = other->field_0x28;
    self->field_0x38.field_0x8 = other->field_0x38.field_0x8;
    if (self != other)
        CopyWeakRef(&self->field_0x38.owner, other->field_0x38.owner);
    self->field_0x58 = other->field_0x58;
    self->field_0x80 = other->field_0x80;
    self->field_0x88 = other->field_0x88;
    self->field_0xd0 = other->field_0xd0;
    if (self != other)
        CopyWeakRef(&self->field_0xe0, other->field_0xe0);
    return self;
}
