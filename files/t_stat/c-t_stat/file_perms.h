#ifndef FILES_FILE_PERMS_H
#define FILES_FILE_PERMS_H

/* file_perms.h

   Header file for file_perms.c.
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#define FP_SPECIAL 1            /* Include set-user-ID, set-group-ID, and sticky
                                   bit information in returned string */

char *filePermStr(mode_t perm, int flags);

#endif //FILES_FILE_PERMS_H
