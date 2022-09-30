#include "ll_stack.h"

#include <cstdint>

typedef long long stack_content_t;
stack_content_t STACK_CONTENT_POISON = 0xDEADBABEC0FEBEEF;
#include "stackworks.h"

typedef uintptr_t secure_key_t;

static const long long RNG_ATTEMPT_LIMIT = 10000;

#ifdef __linux__

    #include <sys/random.h>
    static secure_key_t generate_key(int* err_code = NULL) {
        secure_key_t key = 0;

        int attempt_cnt = 0;
        for (attempt_cnt = 0;
            getrandom(&key, sizeof(key), GRND_NONBLOCK|GRND_RANDOM) == -1 && attempt_cnt < RNG_ATTEMPT_LIMIT;
            ++attempt_cnt) {};

        _LOG_FAIL_CHECK_(attempt_cnt < RNG_ATTEMPT_LIMIT, "error", ERROR_REPORTS, return key, err_code, errno);

        printf("Stack security key: %ld.\n", key);

        return key;
    }

#elif defined(WIN32) || defined(WIN64)

    // TODO: Make sure this sh*t even compiles on Windows OS.
    #include <windowsrng.h>
    static secure_key_t generate_key(int* err_code = NULL) {

        secure_key_t key = 0;

        bool gen_failed = true;
        int attempt_cnt = 0;
        for (attempt_cnt = 0; 
            gen_failed && attempt_cnt < RNG_ATTEMPT_LIMIT; 
            ++attempt_cnt) {
            RandomSequence generator = RandomGenerator();  // Generator may fail to initialize, so we need to generate it every time...
            gen_failed = !generator.generate(&key, sizeof(key));
        }

        _LOG_FAIL_CHECK_(attempt_cnt < RNG_ATTEMPT_LIMIT, "error", ERROR_REPORTS, return key, err_code, errno);

        return key;
    }

#endif

static secure_key_t CRYPTO_KEY = generate_key(&errno);

/**
 * @brief XORed garbage goes in, pointer goes out.
 * 
 * @param ptr encrypted pointer
 * @return void* decrypted pointer
 */
static void* decrypt_ptr(void* ptr);

/**
 * @brief Pointer goes in, XORed garbage goes out.
 * 
 * @param ptr pointer to encrypt
 * @return void* encrypted pointer
 */
static void* encrypt_ptr(void* ptr);

LLStack ll_stack_ctor(size_t size, int* const err_code) {
    Stack* stack = (Stack*) calloc(1, sizeof(Stack));
    *stack = (Stack){};
    int stack_init_status = 0;
    stack_init(stack, size, &stack_init_status);
    _LOG_FAIL_CHECK_(stack_init_status == 0, "error", ERROR_REPORTS, return NULL, err_code, ENOMEM);
    return encrypt_ptr(stack);
}

void ll_stack_dtor(LLStack stack) {
    stack_destroy((Stack*)decrypt_ptr(stack));
    free(decrypt_ptr(stack));
}

void ll_stack_push(LLStack stack, const ll_stack_content_t value, int* const err_code) {
    stack_push((Stack*)decrypt_ptr(stack), value, err_code);
}

ll_stack_content_t ll_stack_pull(LLStack stack, int* const err_code) {
    return stack_get((Stack*)decrypt_ptr(stack), err_code);
}

void ll_stack_pop(LLStack stack, int* const err_code) {
    stack_pop((Stack*)decrypt_ptr(stack), err_code);
}

stack_report_t ll_stack_status(LLStack stack) {
    if (stack == NULL) return STACK_NULL;
    return stack_status((Stack*)decrypt_ptr(stack));
}

void _ll_stack_dump(LLStack stack, int importance, const char* function, const size_t line, const char* file) {
    _stack_dump((Stack*)decrypt_ptr(stack), importance, function, line, file);
}

uintptr_t ll_stack_size(LLStack stack, int* const err_code) {
    _LOG_FAIL_CHECK_(check_ptr(stack) == false, "error", ERROR_REPORTS, return (uintptr_t)NULL, err_code, EINVAL);
    uintptr_t size = ((Stack*)decrypt_ptr(stack))->size;
    return size;
}

uintptr_t ll_stack_capacity(LLStack stack, int* const err_code) {
    _LOG_FAIL_CHECK_(check_ptr(stack) == false, "error", ERROR_REPORTS, return (uintptr_t)NULL, err_code, EINVAL);
    uintptr_t size = ((Stack*)decrypt_ptr(stack))->capacity;
    return size;
}

static void* decrypt_ptr(void* ptr) {
    return (void*)((uintptr_t)ptr ^ (uintptr_t)CRYPTO_KEY);
}

static void* encrypt_ptr(void* ptr) {
    return (void*)((uintptr_t)ptr ^ (uintptr_t)CRYPTO_KEY);
}