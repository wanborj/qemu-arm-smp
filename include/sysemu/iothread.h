/*
 * Event loop thread
 *
 * Copyright Red Hat Inc., 2013
 *
 * Authors:
 *  Stefan Hajnoczi   <stefanha@redhat.com>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 *
 */

#ifndef IOTHREAD_H
#define IOTHREAD_H

#include "block/aio.h"
#include "qemu/thread.h"

#define TYPE_IOTHREAD "iothread"

typedef struct {
    Object parent_obj;

    QemuThread thread;
    AioContext *ctx;
    GMainContext *worker_context;
    GMainLoop *main_loop;
    GOnce once;
    QemuMutex init_done_lock;
    QemuCond init_done_cond;    /* is thread initialization done? */
    bool stopping;              /* has iothread_stop() been called? */
    bool running;               /* should iothread_run() continue? */
    int thread_id;

    /* AioContext poll parameters */
    int64_t poll_max_ns;
    int64_t poll_grow;
    int64_t poll_shrink;
} IOThread;

#define IOTHREAD(obj) \
   OBJECT_CHECK(IOThread, obj, TYPE_IOTHREAD)

char *iothread_get_id(IOThread *iothread);
IOThread *iothread_by_id(const char *id);
AioContext *iothread_get_aio_context(IOThread *iothread);
void iothread_stop_all(void);
GMainContext *iothread_get_g_main_context(IOThread *iothread);

/*
 * Helpers used to allocate iothreads for internal use.  These
 * iothreads will not be seen by monitor clients when query using
 * "query-iothreads".
 */
IOThread *iothread_create(const char *id, Error **errp);
void iothread_stop(IOThread *iothread);
void iothread_destroy(IOThread *iothread);

#endif /* IOTHREAD_H */
