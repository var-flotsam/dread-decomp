#pragma once

#include "base/UnkDestructors.h"
#include "types.h"

// The message, as UnkEmmyDefeatedMessage_b42.cpp lays it out.
struct UnkMlogMessage_b42a1 {
    u32 kind;
    base::global::CStrId text;
    base::global::CRntVector<base::UnkStrIdMember> args;

    __attribute__((always_inline)) UnkMlogMessage_b42a1(const base::global::CStrId& t) : kind(0), text(t), args(0xc) {}
    __attribute__((always_inline)) ~UnkMlogMessage_b42a1() {
        args.Free();
    }
};
