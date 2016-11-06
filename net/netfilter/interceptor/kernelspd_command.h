/**
   @copyright
<<<<<<< HEAD
   Copyright (c) 2013, INSIDE Secure Oy. All rights reserved.
=======
   Copyright (c) 2013 - 2014, INSIDE Secure Oy. All rights reserved.
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
*/

#ifndef KERNEL_SPD_COMMAND_H
#define KERNEL_SPD_COMMAND_H

#include "public_defs.h"

#define LINUX_SPD_PROC_FILENAME "spd"

<<<<<<< HEAD
=======
#define KERNEL_SPD_VERSION 1

>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
#define KERNEL_SPD_ID_VALID(__spd_id) \
  ((__spd_id) == KERNEL_SPD_IN || (__spd_id) == KERNEL_SPD_OUT)

enum
{
  KERNEL_SPD_IN,
  KERNEL_SPD_OUT,
  KERNEL_SPD_COUNT
};


enum
{
  /*
<<<<<<< HEAD
     Activate command is followed by a comma separated string of
     unprotected interfaces.
=======
     Activate command is followed by an IPsec boundary string.
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
  */
  KERNEL_SPD_ACTIVATE,

  /*
    Insert command is followed by an encoded IP Selector Group.
   */
  KERNEL_SPD_INSERT_ENTRY,

  /*
    Remove command has no additional data.
   */
  KERNEL_SPD_REMOVE_ENTRY,

  /*
<<<<<<< HEAD
    Deactivate command has no additional data.
   */
  KERNEL_SPD_DEACTIVATE
=======
    Update IPsec boundary (i.e. interface name list)
   */
  KERNEL_SPD_UPDATE_IPSEC_BOUNDARY,

  /*
    Deactivate command has no additional data.
   */
  KERNEL_SPD_DEACTIVATE,

  /*
    Synchronize on version. Command is followed by an integer version
    number.
   */
  KERNEL_SPD_VERSION_SYNC
>>>>>>> 0b824330b77d5a6e25bd7e249c633c1aa5e3ea68
};


enum
{
  KERNEL_SPD_DISCARD,
  KERNEL_SPD_BYPASS
};

struct KernelSpdCommand
{
  uint32_t command_id;
  uint32_t action_id;
  uint32_t priority;
  uint32_t precedence;
  uint32_t spd_id;
  uint32_t entry_id;
  uint32_t bytecount;
};

#endif /* KERNEL_SPD_COMMAND_H */

