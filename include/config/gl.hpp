#pragma once

constexpr bool ENABLE_MSAA = true;

#define logGlIsEnabled(cap) printf("%s=%d\n", #cap, glIsEnabled(cap))

#define logGlEnable(cap)                              \
    do {                                              \
        printf("%s=%d->1\n", #cap, glIsEnabled(cap)); \
        glEnable(cap);                                \
    } while (0)

#define logGlDisable(cap)                             \
    do {                                              \
        printf("%s=%d->0\n", #cap, glIsEnabled(cap)); \
        glDisable(cap);                               \
    } while (0)