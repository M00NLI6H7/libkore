#pragma once

#define KORE_VERIFY_THROW(EXPR, EXCEPTION, ARGS...) \
    if(!(EXPR))                                     \
    {                                               \
        throw EXCEPTION(ARGS);                      \
    }
