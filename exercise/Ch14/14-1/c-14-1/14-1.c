#include <bits/time.h>
#include <fcntl.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>

typedef int bool;
#define TRUE 1
#define FALSE 0

typedef struct {
    bool ordered;
    long n;
} Arg;

static void elapsed(const struct timespec *, const struct timespec *,
                    struct timespec *);

static void shuffle_array(int32_t *, size_t);

static Arg arg_paring(int, char **);

static void file_manipulation(const Arg *, struct timespec *);

int main(int ac, char *av[]) {
    struct timespec duration;
    const Arg a = arg_paring(ac, av);
    printf("File system: ");
    fflush(stdout);
    system("stat . -f -c \"%T\"");

    if (a.ordered) {
        printf("Benchmarking (Creating and deleting %ld file in an **ordered** way...)\n",
               a.n);
    } else {
        printf("Benchmarking (Creating and deleting %ld file in a **random** way...)\n",
               a.n);
    }
    file_manipulation(&a, &duration);
    double time = (double)duration.tv_sec;
    time += (double)(duration.tv_nsec)/1000000000.0;
    printf("Consumed %f seconds", time);
    return 0;
}

static void file_manipulation(const Arg *arg, struct timespec *duration) {
    assert(arg != NULL);

    char filename[1024];

    // init array
    int arr[arg->n];
    for (int i = 0; i < arg->n; i += 1) {
        arr[i] = i;
    }

    if (arg->ordered) {
        shuffle_array(arr, arg->n);
    }

    struct timespec start;
    clock_gettime(CLOCK_REALTIME, &start);

    // create file
    for (int i = 0; i < arg->n; i += 1) {
        sprintf(filename, "%d", arr[i]);
        int fd = open(filename, O_CREAT | O_WRONLY, 0666);
        if (fd == -1) {
            fprintf(stderr, "failed to create %s\n", filename);
            continue;
        }
        assert(1 == write(fd, "C", 1));
        close(fd);
    }

    // delete file
    for (int i = 0; i < arg->n; i += 1) {
        sprintf(filename, "%d", arr[i]);

        if (-1 == unlink(filename)) {
            fprintf(stderr, "Unable to delete %s\n", filename);
            continue;
        }
    }

    struct timespec end;
    clock_gettime(CLOCK_REALTIME, &end);

    elapsed(&end, &start, duration);
}

static Arg arg_paring(int ac, char *av[]) {
    Arg a;
    a.ordered = FALSE;
    a.n = 0;
    int c;
    char *res = NULL;
    while ((c = getopt(ac, av, "on:")) != -1) {
        switch (c) {
            case 'o':
                a.ordered = TRUE;
                break;
            case 'n':
                errno = 0;
                a.n = strtol(optarg, &res, 10);
                if (*res != '\0' || errno != 0) {
                    fprintf(stderr, "Invalid value for n: %s\n",
                            optarg);
                    exit(EXIT_FAILURE);
                }
                break;
            default:
                break;
        }
    }

    return a;
}

static void shuffle_array(int32_t *array, size_t len) {
    if (len > 1) {
        size_t i;
        for (i = 0; i < len - 1; i++) {
            size_t j = i + rand() / (RAND_MAX / (len - i) + 1);
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}

static void elapsed(const struct timespec *now, const struct timespec *since,
                    struct timespec *res) {
    res->tv_sec = now->tv_sec - since->tv_sec;
    res->tv_nsec = now->tv_nsec - since->tv_nsec;

    if (res->tv_nsec < 0) {
        res->tv_sec -= 1;
        res->tv_nsec += 1000000000;
    }
}
