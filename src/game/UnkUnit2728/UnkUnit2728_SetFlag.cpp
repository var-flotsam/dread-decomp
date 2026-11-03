// Unit 2728: sets the flag at +0x38 and, when it changes, tells the game manager's object at +0x2278
// (its +0x10) through fn_7100b8534c (set) or fn_7100b8538c (clear). Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "types.h"
struct UnkUnit2728Target;
void fn_7100b8534c(UnkUnit2728Target* target);
void fn_7100b8538c(UnkUnit2728Target* target);
struct UnkUnit2728Holder {
    u8 pad_0x0[0x10];
    UnkUnit2728Target* target;  // 0x10
};
struct UnkUnit2728 {
    u8 pad_0x0[0x38];
    bool flag;  // 0x38
};
void fn_7101192b48(UnkUnit2728* self, bool flag) {
    if (self->flag == flag)
        return;
    UnkUnit2728Holder* holder = *reinterpret_cast<UnkUnit2728Holder**>(reinterpret_cast<char*>(g_unk_7101d47310) + 0x2278);
    if (holder && holder->target) {
        if (flag)
            fn_7100b8534c(holder->target);
        else
            fn_7100b8538c(holder->target);
    }
    self->flag = flag;
}
