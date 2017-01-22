/**
 * cc fs-onchange.c -luv -o fs-onchange
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <uv.h>

uv_loop_t *loop;

void fs_event_cb(uv_fs_event_t *handle, const char *filename, int events, int status)
{
    assert(status == 0);
    const char *type;

    switch (events) {
        case UV_RENAME:
            type = "RENAMED";
            break;
        case UV_CHANGE:
            type = "CHANGED";
            break;
        default:
            type = "UNKNOWN";
            break;
    }

    fprintf(stderr, "Change detected in %s: ", handle->path);
    fprintf(stderr, "%s", type);
    fprintf(stderr, " %s\n", filename ? filename : "");
}

int main(int argc, const char *argv[])
{
    const char *path;
    uv_fs_event_t *fs_event;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file1> [file2 ...]\n", argv[0]);
        return 1;
    }

    loop = uv_default_loop();

    while (argc-- > 1) {
        path = argv[argc];
        fprintf(stderr, "Adding watch on %s\n", path);
        fs_event = (uv_fs_event_t*)malloc(sizeof(uv_fs_event_t));
        uv_fs_event_init(loop, fs_event);
        uv_fs_event_start(fs_event, fs_event_cb, path, 0);
    }

    return uv_run(loop, UV_RUN_DEFAULT);
}
