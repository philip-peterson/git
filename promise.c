/*
 * Generic implementation of callbacks with await checking.
 */
#include "promise.h"

void promise_assert_finished(struct promise_t *p) {
    if (p->state == PROMISE_UNRESOLVED) {
        BUG("expected promise to have been resolved/rejected");
    }
}

void promise_resolve(struct promise_t *p, int status) {
    if (p->state != PROMISE_UNRESOLVED) {
        BUG("promise was already resolved/rejected");
        return;
    }
    p->result.success_result = status;
    p->state = PROMISE_SUCCESS;
}

void promise_reject(struct promise_t *p, int status, const char* fmt, ...) {
    if (p->state != PROMISE_UNRESOLVED) {
        BUG("promise was already resolved/rejected");
        return;
    }
    p->result.failure_result.status = status;

    strbuf_init(failure_result.message, 0);

    va_list args;
    va_start(args, fmt);
    strbuf_addf(p->result.failure_result.message, fmt, args);
    va_end(args);

    p->state = PROMISE_FAILURE;
}

struct promise_t *promise_init() {
	struct promise_t *new_promise = xmalloc(sizeof(struct promise_t));
    new_promise->state = PROMISE_UNRESOLVED;

    struct failure_result_t failure_result;
    failure_result.status = 0;

    new_promise->result.failure_result = failure_result;

    return new_promise;
}

/**
 * Performs a partial release of the promise, but if this function returns
 * a nonnegative value, an additional free() must be called on the error_message
 * buffer.
 */
int promise_consume(struct promise_t *promise, char **error_message) {
    int size = -1;
    if (promise->state == PROMISE_FAILURE) {
        *error_message = strbuf_detach(promise->result.failure_result.message, &size);
    }
    free(promise);
    return size;
}

/**
 * Fully deallocates the promise as well as the error message, if any.
 */
void promise_release(struct promise_t *promise) {
    char *error_message;
    if (promise_consume(promise, &error_message) >= 0) {
        free(error_message);
    }
}
