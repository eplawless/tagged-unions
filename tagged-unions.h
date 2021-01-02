/*
 * Copyright (C) 2012 William Swanson
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the names of the authors or
 * their institutions shall not be used in advertising or otherwise to
 * promote the sale, use or other dealings in this Software without
 * prior written authorization from the authors.
 */

#pragma once

#define EVAL0(...) __VA_ARGS__
#define EVAL1(...) EVAL0(EVAL0(EVAL0(__VA_ARGS__)))
#define EVAL2(...) EVAL1(EVAL1(EVAL1(__VA_ARGS__)))
#define EVAL3(...) EVAL2(EVAL2(EVAL2(__VA_ARGS__)))
#define EVAL4(...) EVAL3(EVAL3(EVAL3(__VA_ARGS__)))
#define EVAL(...)  EVAL4(EVAL4(EVAL4(__VA_ARGS__)))

#define MAP_END(...)
#define MAP_OUT
#define MAP_COMMA ,

#define MAP_GET_END2() 0, MAP_END
#define MAP_GET_END1(...) MAP_GET_END2
#define MAP_GET_END(...) MAP_GET_END1
#define MAP_NEXT0(test, next, ...) next MAP_OUT
#define MAP_NEXT1(test, next) MAP_NEXT0(test, next, 0)
#define MAP_NEXT(test, next)  MAP_NEXT1(MAP_GET_END test, next)

#define MAP0(f, x, peek, ...) f(x) MAP_NEXT(peek, MAP1)(f, peek, __VA_ARGS__)
#define MAP1(f, x, peek, ...) f(x) MAP_NEXT(peek, MAP0)(f, peek, __VA_ARGS__)

#define MAP_LIST_NEXT1(test, next) MAP_NEXT0(test, MAP_COMMA next, 0)
#define MAP_LIST_NEXT(test, next)  MAP_LIST_NEXT1(MAP_GET_END test, next)

#define MAP_LIST0(f, x, peek, ...) f(x) MAP_LIST_NEXT(peek, MAP_LIST1)(f, peek, __VA_ARGS__)
#define MAP_LIST1(f, x, peek, ...) f(x) MAP_LIST_NEXT(peek, MAP_LIST0)(f, peek, __VA_ARGS__)

/**
 * Applies the function macro `f` to each of the remaining parameters.
 */
#define MAP(f, ...) EVAL(MAP1(f, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

/**
 * Applies the function macro `f` to each of the remaining parameters and
 * inserts commas between the results.
 */
#define MAP_LIST(f, ...) EVAL(MAP_LIST1(f, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

/*
 * Copyright (C) 2021 Eric Lawless
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the names of the authors or
 * their institutions shall not be used in advertising or otherwise to
 * promote the sale, use or other dealings in this Software without
 * prior written authorization from the authors.
 */

#define MAP_WITH0(f, arg1, x, peek, ...) f(arg1, x) MAP_NEXT(peek, MAP_WITH1)(f, arg1, peek, __VA_ARGS__)
#define MAP_WITH1(f, arg1, x, peek, ...) f(arg1, x) MAP_NEXT(peek, MAP_WITH0)(f, arg1, peek, __VA_ARGS__)
#define MAP_WITH(f, arg1, ...)  EVAL(MAP_WITH1(f, arg1, __VA_ARGS__, ()()(), ()()(), ()()(), 0))

#define EXPAND(x) x
#define STATE_NAME1(x, ...) x
#define STATE_NAME(...) EXPAND(STATE_NAME1 __VA_ARGS__)
#define STATE_BODY1(x, y, ...) y
#define STATE_BODY(...) EXPAND(STATE_BODY1 __VA_ARGS__)

///////////////////////////////////////////////////////////////////////////////
// State Enum
///////////////////////////////////////////////////////////////////////////////

#define TAGGED_UNION_STATE_ENUM(UnionName, ...) \
    enum class E##UnionName##State { \
        MAP_LIST(STATE_NAME, __VA_ARGS__) \
    };

///////////////////////////////////////////////////////////////////////////////
// Individual State Structs
///////////////////////////////////////////////////////////////////////////////

#define TAGGED_UNION_STATE_STRUCT2(UnionName, StateName, StateBody) \
    struct UnionName##_##StateName StateBody;

#define TAGGED_UNION_STATE_STRUCT1(...) \
    EXPAND(TAGGED_UNION_STATE_STRUCT2(__VA_ARGS__))

#define TAGGED_UNION_STATE_STRUCT(UnionName, State) \
    TAGGED_UNION_STATE_STRUCT1(UnionName, STATE_NAME(State), STATE_BODY(State))

#define TAGGED_UNION_STATE_STRUCTS(UnionName, ...) \
    MAP_WITH(TAGGED_UNION_STATE_STRUCT, UnionName, __VA_ARGS__)

///////////////////////////////////////////////////////////////////////////////
// Match Function
///////////////////////////////////////////////////////////////////////////////

#define TAGGED_UNION_MATCH_FUNCTION_TYPENAME2(StateName) \
    typename Fn##StateName

#define TAGGED_UNION_MATCH_FUNCTION_TYPENAME1(...) \
    TAGGED_UNION_MATCH_FUNCTION_TYPENAME2(__VA_ARGS__)

#define TAGGED_UNION_MATCH_FUNCTION_TYPENAME(State) \
    TAGGED_UNION_MATCH_FUNCTION_TYPENAME1(STATE_NAME(State))

#define TAGGED_UNION_MATCH_FUNCTION_ARG2(StateName) \
    Fn##StateName&& Handle##StateName

#define TAGGED_UNION_MATCH_FUNCTION_ARG1(...) \
    TAGGED_UNION_MATCH_FUNCTION_ARG2(__VA_ARGS__)

#define TAGGED_UNION_MATCH_FUNCTION_ARG(State) \
    TAGGED_UNION_MATCH_FUNCTION_ARG1(STATE_NAME(State))

#define TAGGED_UNION_MATCH_FUNCTION_SWITCH_CASE2(UnionName, StateName) \
    case E##UnionName##State::StateName: \
        return std::forward<Fn##StateName>(Handle##StateName)(UnionName.Value.StateName);

#define TAGGED_UNION_MATCH_FUNCTION_SWITCH_CASE1(...) \
    TAGGED_UNION_MATCH_FUNCTION_SWITCH_CASE2(__VA_ARGS__)

#define TAGGED_UNION_MATCH_FUNCTION_SWITCH_CASE(UnionName, State) \
    TAGGED_UNION_MATCH_FUNCTION_SWITCH_CASE1(UnionName, STATE_NAME(State))

#define TAGGED_UNION_MATCH_FUNCTION(UnionName, ...) \
    template <typename T, MAP_LIST(TAGGED_UNION_MATCH_FUNCTION_TYPENAME, __VA_ARGS__)> \
    T Match( \
        const UnionName &UnionName, \
        MAP_LIST(TAGGED_UNION_MATCH_FUNCTION_ARG, __VA_ARGS__) \
    ) { \
        switch (UnionName.State) \
        { \
            MAP_WITH(TAGGED_UNION_MATCH_FUNCTION_SWITCH_CASE, UnionName, __VA_ARGS__) \
        } \
    }

///////////////////////////////////////////////////////////////////////////////
// Actual Union
///////////////////////////////////////////////////////////////////////////////

#define TAGGED_UNION_VALUE_STRUCT_MEMBER2(UnionName, StateName, StateBody) \
    UnionName##_##StateName StateName;

#define TAGGED_UNION_VALUE_STRUCT_MEMBER1(...) \
    EXPAND(TAGGED_UNION_VALUE_STRUCT_MEMBER2(__VA_ARGS__))

#define TAGGED_UNION_VALUE_STRUCT_MEMBER(UnionName, State) \
    TAGGED_UNION_VALUE_STRUCT_MEMBER1(UnionName, STATE_NAME(State), STATE_BODY(State))

#define TAGGED_UNION_VALUE_STRUCT(ConstructType, UnionName, ...) \
    struct UnionName; \
    ConstructType UnionName##Value { \
    public: \
        UnionName##Value() {} \
        ~UnionName##Value() = default; \
        UnionName##Value(const UnionName##Value&) = default; \
    private: \
        MAP_WITH(TAGGED_UNION_VALUE_STRUCT_MEMBER, UnionName, __VA_ARGS__) \
        friend struct UnionName; \
        template <typename T, MAP_LIST(TAGGED_UNION_MATCH_FUNCTION_TYPENAME, __VA_ARGS__)> \
        friend T Match( \
            const UnionName &UnionName, \
            MAP_LIST(TAGGED_UNION_MATCH_FUNCTION_ARG, __VA_ARGS__) \
        ); \
    };

///////////////////////////////////////////////////////////////////////////////
// Container
///////////////////////////////////////////////////////////////////////////////

#define TAGGED_UNION_CONTAINER_TYPE_CONSTRUCTOR2(UnionName, StateName) \
    UnionName(const UnionName##_##StateName& StateName) { \
        State = E##UnionName##State::StateName; \
        Value.StateName = StateName; \
    }

#define TAGGED_UNION_CONTAINER_TYPE_CONSTRUCTOR1(...) \
    EXPAND(TAGGED_UNION_CONTAINER_TYPE_CONSTRUCTOR2(__VA_ARGS__))

#define TAGGED_UNION_CONTAINER_TYPE_CONSTRUCTOR(UnionName, State) \
    TAGGED_UNION_CONTAINER_TYPE_CONSTRUCTOR1(UnionName, STATE_NAME(State))


#define TAGGED_UNION_CONTAINER_TYPE(UnionName, ...) \
    struct UnionName { \
        E##UnionName##State State; \
        UnionName##Value Value; \
        MAP_WITH(TAGGED_UNION_CONTAINER_TYPE_CONSTRUCTOR, UnionName, __VA_ARGS__) \
    };

///////////////////////////////////////////////////////////////////////////////
// Top-Level Definition
///////////////////////////////////////////////////////////////////////////////

#define TAGGED_UNION_IMPL(ConstructType, UnionName, ...) \
    TAGGED_UNION_STATE_ENUM(UnionName, __VA_ARGS__) \
    TAGGED_UNION_STATE_STRUCTS(UnionName, __VA_ARGS__) \
    TAGGED_UNION_VALUE_STRUCT(ConstructType, UnionName, __VA_ARGS__) \
    TAGGED_UNION_CONTAINER_TYPE(UnionName, __VA_ARGS__) \
    TAGGED_UNION_MATCH_FUNCTION(UnionName, __VA_ARGS__)

#define TAGGED_UNION(UnionName, ...) \
    TAGGED_UNION_IMPL(union, UnionName, __VA_ARGS__)

#define TAGGED_STRUCT(UnionName, ...) \
    TAGGED_UNION_IMPL(struct, UnionName, __VA_ARGS__)

// TODO: SFINAE to choose between union and struct?
