#ifndef PROMISE_H
#define PROMISE_H

#include "git-compat-util.h"

enum promise_state {
	PROMISE_UNRESOLVED = 0,
	PROMISE_SUCCESS = 1,
	PROMISE_FAILURE = 2,
};

typedef int success_result_t;

#define PROMISE_MESSAGE_LEN 4096

struct failure_result_t {
    int status;
    strbuf *message;
};

struct promise_t {
    enum promise_state state;
    union {
        success_result_t success_result;
        struct failure_result_t failure_result;
    } result;
};

// Function to assert that a promise has been resolved
void promise_assert_finished(struct promise_t *p);

// Function to resolve a promise with a success result
void promise_resolve(struct promise_t *p, int status);

// Function to reject a promise with a failure result and an optional formatted error message
void promise_reject(struct promise_t *p, int status, const char* fmt, ...);

// Function to create a new promise
struct promise_t *promise_init();

// Partially deallocates the promise
int promise_consume(struct promise_t *promise, char **error_message);

// Fully deallocates the promise
void promise_release(struct promise_t *promise);

#endif
