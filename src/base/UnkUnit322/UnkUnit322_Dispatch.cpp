// tools/tu_map.py unit 322 (beside src/base/spatial/UnkModelInstanceSet.cpp): in pass 0 only,
// hands the item to the handler for its kind (+0x2c): 3, 5, 6 or 11; other kinds are ignored.
// Names are placeholders.
#include "types.h"

struct UnkItem322 {
    u8 pad_0x0[0x2c];
    s32 kind;  // 0x2c
};

char* fn_710022471c(char* p0);  // as src/lifted declares it
void fn_7100224728(char* self, UnkItem322* item);
void fn_71002247c4(char* self, UnkItem322* item);
void fn_7100224868(char* self, UnkItem322* item);

void fn_710021d394(char* self, UnkItem322* item, u32 pass) {
    if (pass != 0) {
        return;
    }
    switch (item->kind) {
    case 3:
        fn_710022471c(self);
        return;
    case 5:
        fn_7100224728(self, item);
        return;
    case 6:
        fn_71002247c4(self, item);
        return;
    case 11:
        fn_7100224868(self, item);
        return;
    default:
        return;
    }
}
