#pragma once

#define GET_PERMS(is_owner, in_group, permissions) \
    ((is_owner) ? ((permissions >> 6) & 0x7) : \
     (in_group) ? ((permissions >> 3) & 0x7) : \
                  ((permissions) & 0x7))
