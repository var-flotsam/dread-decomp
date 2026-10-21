// Runs fn_71010fa290 on the object, then, when both flags at +0x4fc/+0x4fd of the object it
// holds at +0x146b8 are set, passes a global instance (0x108 bytes, created on first use with
// allocation flags 0x3f) to fn_71011a710c.
#include "types.h"

void* fn_71000839a0(u64 size, u32 flags);
void fn_71010fa290(char* self);
void fn_71011a664c(char* instance);  // constructs the instance
void fn_71011a710c(char* instance);

// The instance, reached through the GOT (named after its GOT slot).
extern char* g_unk_7101cd1b48;

void fn_7100369650(char* self) {
    fn_71010fa290(self);
    char* state = *reinterpret_cast<char**>(self + 0x146b8);
    if (state[0x4fc] && state[0x4fd]) {
        char* instance = g_unk_7101cd1b48;
        if (!instance) {
            instance = static_cast<char*>(fn_71000839a0(0x108, 0x3f));
            fn_71011a664c(instance);
            g_unk_7101cd1b48 = instance;
        }
        fn_71011a710c(instance);
    }
}
