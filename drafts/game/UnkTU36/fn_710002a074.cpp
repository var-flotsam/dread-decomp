// Unit 36 of tools/tu_map.py (the NFP worker): the per-frame update of the NFP client. It steps the
// worker's state machine, turns the state it is in (and, on the step it entered it, the client's
// own phase) into a status code, and handles the start and cancel requests. Names and layouts are
// placeholders.
#include "types.h"

void* fn_710002c1b0(void* arg);
bool fn_710002c180();
s32 fn_710002c170();
void fn_710002bfd0(s32 request);
u32 fn_710002bff0();
u32 fn_710002c210();

struct UnkNfpClient {
    s32 status;  // 0x0
    s32 lastStatus;  // 0x4
    bool statusChanged;  // 0x8
    bool enabled;  // 0x9
    bool startRequested;  // 0xa
    bool cancelRequested;  // 0xb
    s32 phase;  // 0xc
    u32 firstTagId;  // 0x10
    u32 tagId;  // 0x14
};

static const u32 kNoTag = 0xffff;

static inline u32 ReadTagId(UnkNfpClient* self) {
    return self->enabled ? fn_710002c210() : kNoTag;
}

static inline void CancelIfRequested(UnkNfpClient* self) {
    if (self->cancelRequested) {
        self->phase = 6;
        fn_710002bfd0(2);
    }
}

static inline void Fail(UnkNfpClient* self, s32 status) {
    self->status = status;
    self->phase = 5;
    fn_710002bfd0(2);
}

static inline s32 TagErrorStatus() {
    switch (fn_710002bff0()) {
    case 8:
        return 3;
    case 10:
        return 0x13;
    case 14:
        return 0x14;
    default:
        return 0x15;
    }
}

static inline s32 MountErrorStatus() {
    switch (fn_710002bff0()) {
    case 1:
        return 0x10;
    case 2:
        return 0x11;
    case 4:
        return 0x15;
    case 14:
        return 0x14;
    default:
        return 0x16;
    }
}

// Reads the tag again and checks it is the one first seen.
static inline void CheckSameTag(UnkNfpClient* self, s32 status) {
    u32 id = ReadTagId(self);
    self->tagId = id;
    if (self->firstTagId == id)
        self->status = status;
    else
        Fail(self, 0x15);
}

void fn_710002a074(UnkNfpClient* self) {
    if (!self->enabled)
        return;
    fn_710002c1b0(self);
    bool entered = fn_710002c180();
    s32 state = fn_710002c170();
    if (self->phase == 6 && state != 0)
        fn_710002bfd0(2);

    switch (state) {
    case 0:
        self->firstTagId = kNoTag;
        self->tagId = kNoTag;
        if (entered)
            self->status = 0;
        self->phase = 0;
        if (self->startRequested) {
            self->phase = 1;
            fn_710002bfd0(1);
        }
        break;
    case 1:
        if (entered)
            self->status = 1;
        break;
    case 6:
        if (entered) {
            switch (self->phase) {
            case 1:
                self->status = 2;
                break;
            case 2:
                self->status = 5;
                break;
            case 3:
                self->status = 8;
                break;
            case 4:
                self->status = 0xb;
                break;
            }
        }
        CancelIfRequested(self);
        break;
    case 0x16:
        if (entered)
            self->status = 0xe;
        // fallthrough
    case 7:
        if (self->phase >= 2 && self->phase <= 4) {
            self->phase = 6;
            fn_710002bfd0(2);
        } else {
            CancelIfRequested(self);
        }
        break;
    case 8:
        if (entered) {
            switch (self->phase) {
            case 1: {
                u32 id = ReadTagId(self);
                self->firstTagId = id;
                self->tagId = id;
                if (id != kNoTag)
                    self->status = 3;
                else
                    Fail(self, TagErrorStatus());
                break;
            }
            case 2:
                CheckSameTag(self, 7);
                break;
            case 3:
                CheckSameTag(self, 0xa);
                break;
            case 4:
                CheckSameTag(self, 0xd);
                break;
            }
        }
        CancelIfRequested(self);
        break;
    case 9:
        CancelIfRequested(self);
        break;
    case 0xe:
        if (entered) {
            switch (self->phase) {
            case 1:
                self->status = 4;
                break;
            case 2:
                self->status = 6;
                break;
            case 3:
                self->status = 9;
                break;
            case 4:
                self->status = 0xc;
                break;
            }
        }
        if (self->phase >= 2 && self->phase <= 4)
            fn_710002bfd0(1);
        break;
    case 0xf:
        self->phase = 5;
        if (entered)
            self->status = 0x19;
        if (self->startRequested) {
            self->phase = 3;
            fn_710002bfd0(1);
        } else {
            CancelIfRequested(self);
        }
        break;
    case 0x10:
        self->phase = 5;
        if (entered)
            self->status = 0x1a;
        if (self->startRequested) {
            self->phase = 4;
            fn_710002bfd0(1);
        } else {
            CancelIfRequested(self);
        }
        break;
    case 0x12:
        self->phase = 5;
        if (entered)
            self->status = 0x17;
        CancelIfRequested(self);
        break;
    case 0x13:
        self->phase = 5;
        if (entered)
            self->status = 0x18;
        CancelIfRequested(self);
        break;
    case 0x14:
        self->phase = 5;
        if (entered)
            self->status = 0x13;
        CancelIfRequested(self);
        break;
    case 0x17:
        self->phase = 5;
        if (entered)
            self->status = MountErrorStatus();
        CancelIfRequested(self);
        break;
    }

    self->startRequested = false;
    self->cancelRequested = false;
    self->statusChanged = self->status != self->lastStatus;
    self->lastStatus = self->status;
}
