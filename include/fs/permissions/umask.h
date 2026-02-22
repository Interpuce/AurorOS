#pragma once

#define DEFAULT_UMASK 0022
#define MK_DEFAULT_PERMISSIONS(base) \
    (base & ~DEFAULT_UMASK)
