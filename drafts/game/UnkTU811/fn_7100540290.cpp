#include "types.h"
// ---- 7100540290
struct UnkMat4_b0193 {
    u8 unk_0x0[0x78];
    void* field_0x78;
};
struct UnkMat3_b0193 {
    u8 unk_0x0[0x138];
    UnkMat4_b0193* field_0x138;
};
struct UnkMat2_b0193 {
    u8 unk_0x0[0x50];
    UnkMat3_b0193* field_0x50;
};
struct UnkMat1_b0193 {
    u8 unk_0x0[0x228];
    UnkMat2_b0193* field_0x228;
};
struct UnkMatOwner_b0193 {
    u8 unk_0x0[0x30];
    UnkMat1_b0193* field_0x30;
    u8 unk_0x38[0x2a010 - 0x38];
    u32 field_0x2a010;
    u8 unk_0x2a014[0x2a030 - 0x2a014];
    u32 field_0x2a030;
};

void fn_7100369508(UnkMatOwner_b0193* self);
bool fn_7100199d88(void* table, const u64* key, u32* id);

void fn_7100540290(UnkMatOwner_b0193* self) {
    fn_7100369508(self);
    u64 key1 = 0x63f8212a4a3b5aaaull;
    fn_7100199d88(self->field_0x30->field_0x228->field_0x50->field_0x138->field_0x78, &key1, &self->field_0x2a010);
    u64 key2 = 0x0d68092d2c695e41ull;
    fn_7100199d88(self->field_0x30->field_0x228->field_0x50->field_0x138->field_0x78, &key2, &self->field_0x2a030);
    if (self->field_0x2a030 == 0xffffffffu) {
        u64 key3 = 0xedba724da199466full;
        fn_7100199d88(self->field_0x30->field_0x228->field_0x50->field_0x138->field_0x78, &key3, &self->field_0x2a030);
    }
}
