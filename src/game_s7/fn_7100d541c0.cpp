// Picks the pair handler for two shapes from the sum of their type bits, passing the shapes
// (and their transforms) in the order the handler expects. Unhandled pairs give an empty
// result. Names and layouts are placeholders.
#include "game/logic/collision/UnkColliderResult.h"
#include "types.h"
#include "UnkLeftoverReg.h"

struct UnkShape {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual u32 GetType();
};

// The empty result (game/logic/collision/UnkColliderResult.h).
inline const UnkColliderResult& NoContact() {
    static UnkColliderResult s_none;
    return s_none;
}

struct UnkShapeTester_A5_b68;
// An empty argument (its register is left as it is).
struct UnkNoContext : UnkLeftoverReg {};

UnkColliderResult fn_7100d54f28(UnkShapeTester_A5_b68* self, UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5, void* x6, bool swapped, bool flag);
UnkColliderResult fn_7100d55920(UnkNoContext, UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5, void* x6, bool flag);
UnkColliderResult fn_7100d551c8(UnkShapeTester_A5_b68* self, UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5, void* x6, bool swapped, bool flag);
UnkColliderResult fn_7100d55b80(UnkNoContext, UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5, void* x6, bool swapped, bool flag);
UnkColliderResult fn_7100d556d0(UnkNoContext, UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5, void* x6, bool flag);
UnkColliderResult fn_7100d5635c(UnkNoContext, UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5, void* x6, bool swapped, bool flag);
UnkColliderResult fn_7100d560bc(UnkNoContext, UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5, void* x6, bool swapped, bool flag);
UnkColliderResult fn_7100d54d78(UnkShapeTester_A5_b68* self, UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5, void* x6, bool swapped, bool flag);
UnkColliderResult fn_7100d54bc0(UnkShapeTester_A5_b68* self, UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5, void* x6, bool swapped, bool flag);
UnkColliderResult fn_7100d54a08(UnkShapeTester_A5_b68* self, UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5, void* x6, bool swapped, bool flag);
UnkColliderResult fn_7100d55484(UnkShapeTester_A5_b68* self, UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5, void* x6, bool swapped, bool flag);
UnkColliderResult fn_7100d55df4(UnkNoContext, UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5, void* x6, bool swapped, bool flag);

UnkColliderResult fn_7100d541c0(UnkShapeTester_A5_b68* self, UnkShape* a, void* ta, UnkShape* b, void* tb, void* x5, void* x6,
                         bool flag) {
    switch (a->GetType() + b->GetType()) {
    case 3: {
        UnkShape* first = a->GetType() == 0x2 ? a : b;
        UnkShape* second = a->GetType() == 0x2 ? b : a;
        void* tfirst = a->GetType() == 0x2 ? ta : tb;
        void* tsecond = a->GetType() == 0x2 ? tb : ta;
        return fn_7100d54f28(self, first, tfirst, second, tsecond, x5, x6, first != a, flag);
    }
    case 4: {
        UnkShape* first = a->GetType() == 0x2 ? a : b;
        UnkShape* second = a->GetType() == 0x2 ? b : a;
        void* tfirst = a->GetType() == 0x2 ? ta : tb;
        void* tsecond = a->GetType() == 0x2 ? tb : ta;
        return fn_7100d55920(UnkNoContext(), first, tfirst, second, tsecond, x5, x6, flag);
    }
    case 5: {
        UnkShape* first = a->GetType() == 0x4 ? a : b;
        UnkShape* second = a->GetType() == 0x4 ? b : a;
        void* tfirst = a->GetType() == 0x4 ? ta : tb;
        void* tsecond = a->GetType() == 0x4 ? tb : ta;
        return fn_7100d551c8(self, first, tfirst, second, tsecond, x5, x6, first != a, flag);
    }
    case 6: {
        UnkShape* first = a->GetType() == 0x2 ? a : b;
        UnkShape* second = a->GetType() == 0x2 ? b : a;
        void* tfirst = a->GetType() == 0x2 ? ta : tb;
        void* tsecond = a->GetType() == 0x2 ? tb : ta;
        return fn_7100d55b80(UnkNoContext(), first, tfirst, second, tsecond, x5, x6, first != a, flag);
    }
    case 8: {
        UnkShape* first = a->GetType() == 0x4 ? a : b;
        UnkShape* second = a->GetType() == 0x4 ? b : a;
        void* tfirst = a->GetType() == 0x4 ? ta : tb;
        void* tsecond = a->GetType() == 0x4 ? tb : ta;
        return fn_7100d556d0(UnkNoContext(), first, tfirst, second, tsecond, x5, x6, flag);
    }
    case 10: {
        UnkShape* first = a->GetType() == 0x8 ? a : b;
        UnkShape* second = a->GetType() == 0x8 ? b : a;
        void* tfirst = a->GetType() == 0x8 ? ta : tb;
        void* tsecond = a->GetType() == 0x8 ? tb : ta;
        return fn_7100d5635c(UnkNoContext(), first, tfirst, second, tsecond, x5, x6, first != a, flag);
    }
    case 12: {
        UnkShape* first = a->GetType() == 0x8 ? a : b;
        UnkShape* second = a->GetType() == 0x8 ? b : a;
        void* tfirst = a->GetType() == 0x8 ? ta : tb;
        void* tsecond = a->GetType() == 0x8 ? tb : ta;
        return fn_7100d560bc(UnkNoContext(), first, tfirst, second, tsecond, x5, x6, first != a, flag);
    }
    case 17:
        return NoContact();
    case 18: {
        UnkShape* first = a->GetType() == 0x2 ? a : b;
        UnkShape* second = a->GetType() == 0x2 ? b : a;
        void* tfirst = a->GetType() == 0x2 ? ta : tb;
        void* tsecond = a->GetType() == 0x2 ? tb : ta;
        return fn_7100d54d78(self, first, tfirst, second, tsecond, x5, x6, first != a, flag);
    }
    case 20: {
        UnkShape* first = a->GetType() == 0x4 ? a : b;
        UnkShape* second = a->GetType() == 0x4 ? b : a;
        void* tfirst = a->GetType() == 0x4 ? ta : tb;
        void* tsecond = a->GetType() == 0x4 ? tb : ta;
        return fn_7100d54bc0(self, first, tfirst, second, tsecond, x5, x6, first != a, flag);
    }
    case 24: {
        UnkShape* first = a->GetType() == 0x8 ? a : b;
        UnkShape* second = a->GetType() == 0x8 ? b : a;
        void* tfirst = a->GetType() == 0x8 ? ta : tb;
        void* tsecond = a->GetType() == 0x8 ? tb : ta;
        return fn_7100d54a08(self, first, tfirst, second, tsecond, x5, x6, first != a, flag);
    }
    case 65: {
        UnkShape* first = a->GetType() == 0x40 ? a : b;
        UnkShape* second = a->GetType() == 0x40 ? b : a;
        void* tfirst = a->GetType() == 0x40 ? ta : tb;
        void* tsecond = a->GetType() == 0x40 ? tb : ta;
        return fn_7100d55484(self, first, tfirst, second, tsecond, x5, x6, first != a, flag);
    }
    case 66: {
        UnkShape* first = a->GetType() == 0x2 ? a : b;
        UnkShape* second = a->GetType() == 0x2 ? b : a;
        void* tfirst = a->GetType() == 0x2 ? ta : tb;
        void* tsecond = a->GetType() == 0x2 ? tb : ta;
        return fn_7100d55df4(UnkNoContext(), second, tsecond, first, tfirst, x5, x6, first != a, flag);
    }
    default:
        return NoContact();
    }
}
