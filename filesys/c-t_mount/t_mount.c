#include <sys/mount.h>
#include "../../lib/tlpi_hdr.h"
#include <stdio.h>

static void usage_error(const char *, const char *);

int main(int ac, char *av[]) {
    unsigned long flags = 0;
    int opt = 0;
    char *data = NULL, *fstype = NULL;

    while ((opt = getopt(ac, av, "o:t:f:")) != -1) {
        switch (opt) {
            case 'o':
                data = optarg;
                break;
            case 't':
                fstype = optarg;
                break;
            case 'f':
                for (int i = 0; i < (int)strlen(optarg); ++i) {
                    switch (optarg[i]) {
                        case 'b':
                            flags |= MS_BIND;
                            break;
                        case 'd':
                            flags |= MS_DIRSYNC;
                            break;
                        case 'l':
                            flags |= MS_MANDLOCK;
                            break;
                        case 'm':
                            flags |= MS_MOVE;
                            break;
                        case 'A':
                            flags |= MS_NOATIME;
                            break;
                        case 'V':
                            flags |= MS_NODEV;
                            break;
                        case 'D':
                            flags |= MS_NODIRATIME;
                            break;
                        case 'E':
                            flags |= MS_NOEXEC;
                            break;
                        case 'S':
                            flags |= MS_NOSUID;
                            break;
                        case 'p':
                            flags |= MS_PRIVATE;
                            break;
                        case 'r':
                            flags |= MS_RDONLY;
                            break;
                        case 'c':
                            flags |= MS_REC;
                            break;
                        case 'T':
                            flags |= MS_RELATIME;
                            break;
                        case 'R':
                            flags |= MS_REMOUNT;
                            break;
                        case 'h':
                            flags |= MS_SHARED;
                            break;
                        case 'v':
                            flags |= MS_SLAVE;
                            break;
                        case 's':
                            flags |= MS_SYNCHRONOUS;
                            break;
                        case 'u':
                            flags |= MS_UNBINDABLE;
                            break;
                        default:
                            usage_error(av[0], NULL);
                    }
                }
                break;
            default:
                usage_error(av[0], NULL);
        }
    }

    if (ac != optind + 2) {
        usage_error(av[0], "Wrong number of arguments\n");
    }

    if (mount(av[optind], av[optind + 1], fstype, flags, data) == -1) {
        errExit("mount(2)");
    }

    exit(EXIT_SUCCESS);
}

static void usage_error(const char *prog_name, const char *msg) {
    if (msg != NULL) {
        fprintf(stderr, "%s", msg);
    }

    fprintf(stderr, "Usage: %s [options] source target\n\n", prog_name);
    fprintf(stderr, "Available options:\n");
#define fpe(str) fprintf(stderr, "     %s", str)
    fpe("-t fstype        [e.g., 'ext2' or 'reiserfs']\n");
    fpe("-o data          [file system-dependent options,\n");
    fpe("                 e.g., 'bsdgroups' for ext2]\n");
    fpe("-f mountflags    can include any of:\n");
#define fpe2(str) fprintf(stderr, "            %s", str)
    fpe2("b - MS_BIND         create a bind mount\n");
    fpe2("d - MS_DIRSYNC      synchronous directory updates\n");
    fpe2("l - MS_MANDLOCK     permit mandatory locking\n");
    fpe2("L - MS_LAZYATIME    lazy atime updates\n");
    fpe2("m - MS_MOVE         atomically move subtree\n");
    fpe2("A - MS_NOATIME      don't update atime (last access time)\n");
    fpe2("V - MS_NODEV        don't permit device access\n");
    fpe2("D - MS_NODIRATIME   don't update atime on directories\n");
    fpe2("E - MS_NOEXEC       don't allow executables\n");
    fpe2("S - MS_NOSUID       disable set-user/group-ID programs\n");
    fpe2("p - MS_PRIVATE      mark as private\n");
    fpe2("r - MS_RDONLY       read-only mount\n");
    fpe2("c - MS_REC          recursive mount\n");
    fpe2("T - MS_RELATIME     relative atime updates\n");
    fpe2("R - MS_REMOUNT      remount\n");
    fpe2("h - MS_SHARED       mark as shared\n");
    fpe2("v - MS_SLAVE        mark as slave\n");
    fpe2("s - MS_SYNCHRONOUS  make writes synchronous\n");
    fpe2("u - MS_UNBINDABLE   mark as unbindable\n");
    exit(EXIT_FAILURE);
}
