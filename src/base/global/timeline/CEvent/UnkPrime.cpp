// Two prime helpers in unit 41 of tools/tu_map.py (base::global::timeline::CEvent's unit; the hash
// tables size their bucket arrays with them): a primality test by trial division over odd divisors,
// and the next prime at or above a number.
#include "types.h"

// Whether `n` is prime (2 and 3 are; 1 and the even numbers past 2 are not).
bool fn_71000669a0(u32 n) {
    if ((n | 1) == 3)
        return true;
    if (n == 1 || !(n & 1))
        return false;
    if (n < 9)
        return true;
    for (u32 d = 3; d * d <= n; d += 2)
        if (n % d == 0)
            return false;
    return true;
}

// The smallest prime at or above `n` (an even `n` starts from the odd number after it).
u32 fn_7100066930(u32 n) {
    n += ~n & 1;
    while (!fn_71000669a0(n))
        n += 2;
    return n;
}
