// Dispatches on two checks of the same pair of objects, then always follows up with
// fn_7100310164 (distance 800) and fn_71003104e8 (given a zeroed out value). The dispatch picks
// fn_710030dff4 when the first check passes and the second fails, fn_710030ed9c otherwise.
#include "types.h"

struct UnkSelf;

bool fn_710030b8b4(UnkSelf* self, void* a2, void* zero);
bool fn_710030dda8(UnkSelf* self, void* a2);
void fn_710030ed9c(UnkSelf* self, void* a1, void* a2, void* a3, u32 a4, void* a5, void* a6, f32 f);
void fn_710030dff4(UnkSelf* self, void* a1, void* a2, void* a3, u32 a4, void* a5, void* a6, f32 f);
void fn_7100310164(UnkSelf* self, void* a6, f32 distance);
void fn_71003104e8(UnkSelf* self, void* a1, u32 a4, void* a3, void* a5, void* a6, u32* out);

void fn_710030def0(UnkSelf* self, void* a1, void* a2, void* a3, u32 a4, void* a5, void* a6, f32 f) {
    if (fn_710030b8b4(self, a2, nullptr) && !fn_710030dda8(self, a2))
        fn_710030dff4(self, a1, a2, a3, a4, a5, a6, f);
    else
        fn_710030ed9c(self, a1, a2, a3, a4, a5, a6, f);
    fn_7100310164(self, a6, 800.0f);
    u32 out = 0;
    fn_71003104e8(self, a1, a4, a3, a5, a6, &out);
}
