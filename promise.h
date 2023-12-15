#ifndef PROMISE_H
#define PROMISE_H

enum promise_state {
	PROMISE_UNRESOLVED = 0,
	PROMISE_SUCCESS = 1,
	PROMISE_FAILURE = 2,
};

// struct PromiseTag {
//     // int promise_id;
//     void (*user_success_cbk)(int status, void* data);
//     void (*user_error_cbk)(int status, char* msg, void* data); 
// }

typedef int success_result_t;

#define PROMISE_MESSAGE_LEN 4096

struct failure_result_t {
    int status;
    char message[PROMISE_MESSAGE_LEN];
};

struct promise_t {
    enum promise_state state;
    union {
        success_result_t success_result;
        struct failure_result_t failure_result;
    } result;
    // void (*success_cbk)(int status, void* data);
    // void (*error_cbk)(int status, char* msg, void* data);
};

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

#endif
