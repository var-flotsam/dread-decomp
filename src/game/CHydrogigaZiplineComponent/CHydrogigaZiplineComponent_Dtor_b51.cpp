// Destructors of CHydrogigaZiplineComponent (unit 1410) and CHydrogigaZiplineRailComponent
// (unit 1411): each frees an array of 0x40-byte entries (at +0x4f8 and +0x40), then runs its base
// destructor (fn_710091e510, and the component's fn_71010c4194).
#include "base/UnkVector.h"
#include "game/UnkComponent.h"

struct UnkZiplineEntry {
    virtual ~UnkZiplineEntry();
    u8 pad_0x8[0x38];
};

void fn_710091e510(char*);

extern void* g_unk_7101a825f8[];
extern void* g_unk_7101a827e8[];

template <u32 Offset>
struct UnkZiplineComponentT {
    void* vtable;
    u8 pad_0x8[Offset - 0x8];
    base::UnkVector<UnkZiplineEntry> entries;
};

typedef UnkZiplineComponentT<0x4f8> UnkZiplineComponent;
typedef UnkZiplineComponentT<0x40> UnkZiplineRailComponent;

static inline void BaseDtor(UnkZiplineComponent* self) {
    fn_710091e510(reinterpret_cast<char*>(self));
}

static inline void BaseDtor(UnkZiplineRailComponent* self) {
    fn_71010c4194(reinterpret_cast<UnkBaseComponent*>(self));
}

template <void** Vtable, class T>
static inline void ZiplineDtor(T* self) {
    self->vtable = Vtable + 2;
    self->entries.Free();
    BaseDtor(self);
}

void fn_710091c344(UnkZiplineComponent* self) {
    ZiplineDtor<g_unk_7101a825f8>(self);
}

void fn_710091c3f4(UnkZiplineComponent* self) {
    ZiplineDtor<g_unk_7101a825f8>(self);
    fn_7100083c00(self);
}

void fn_710091d194(UnkZiplineRailComponent* self) {
    ZiplineDtor<g_unk_7101a827e8>(self);
}

void fn_710091d244(UnkZiplineRailComponent* self) {
    ZiplineDtor<g_unk_7101a827e8>(self);
    fn_7100083c00(self);
}
