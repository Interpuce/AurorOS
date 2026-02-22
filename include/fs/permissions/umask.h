#pragma once

#define DEFAULT_UMASK 0077
#define MK_DEFAULT_PERMISSIONS(base) \
    (base & ~DEFAULT_UMASK)
