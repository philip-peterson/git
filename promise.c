/*
 * Generic implementation of callbacks with await checking.
 */
#include "promise.h"

void promise_assert_finished(struct promise_t *p) {
    if (p->state == PROMISE_UNRESOLVED) {
        BUG("expected promise to have resolved");
    }
}

void promise_resolve(struct promise_t *p, int status) {
    if (p->state != PROMISE_UNRESOLVED) {
        BUG("promise was already resolved");
        return;
    }
    p->result.success_result = status;
    p->state = PROMISE_SUCCESS;
}

void promise_reject(struct promise_t *p, int status, const char* fmt, ...) {
    if (p->state != PROMISE_UNRESOLVED) {
        BUG("promise was already resolved");
        return;
    }
    p->result.failure_result.status = status;

    va_list args;
    va_start(args, fmt);
    xsnprintf(p->result.failure_result.message, PROMISE_MESSAGE_LEN, fmt, args);
    va_end(args);

    p->state = PROMISE_FAILURE;
}

struct promise_t *create_promise() {
	// static int promise_id = 0; /* GLOBAL */

	struct promise_t *new_promise = xmalloc(sizeof(struct promise_t));
    new_promise->state = PROMISE_UNRESOLVED;

    struct failure_result_t failure_result = {
        .status = 0,
        .message = { 0 }
    };

    new_promise->result.failure_result = failure_result;

    return new_promise;
}

void release_promise(struct promise_t *promise) {
    free(promise);
}
