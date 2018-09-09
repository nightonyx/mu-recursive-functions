#ifndef POINT_WATCHER_H
#define POINT_WATCHER_H

typedef enum point_watcher_mode {
    POINT_WATCHER_UNDEFINED_MODE, POINT_WATCHER_PASSIVE_MODE, POINT_WATCHER_ACTIVE_MODE
} point_watcher_mode;

typedef struct point_watcher {
    unsigned pointer;
    unsigned start;
    unsigned end;
    enum point_watcher_mode mode;
} point_watcher;

struct point_watcher *new_point_watcher();

struct point_watcher *new_point_watcher_with_pointer(const unsigned pointer_value);

void reset_point_watcher(struct point_watcher *watcher);

void free_point_watcher(struct point_watcher *watcher);

#endif //POINT_WATCHER_H
