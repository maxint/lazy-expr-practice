#ifndef __MAT_HPP__
#define __MAT_HPP__

#include <cstdio>

// expression
template<typename SubType>
struct Exp {
    inline const SubType& self() const {
        return *static_cast<const SubType*>(this);
    }
};

template<typename TLhs, typename TRhs>
struct ReturnType {
    typedef typename TLhs::value_type value_type;
};

// general binary operation
template<typename OP, typename TLhs, typename TRhs>
struct BinaryMapExp : Exp<BinaryMapExp<OP, TLhs, TRhs> > {
    const TLhs &lhs;
    const TRhs &rhs;
    typedef typename ReturnType<TLhs, TRhs>::value_type value_type;

    BinaryMapExp(const TLhs &lhs, const TRhs &rhs)
        : lhs(lhs), rhs(rhs) {}

    inline value_type Eval(int i) const {
        return OP::Map(lhs.Eval(i), rhs.Eval(i));
    }
};

// vector
template<typename T>
struct Vec : public Exp<Vec<T> > {
    typedef T value_type;

    int len;
    T* dptr;

    Vec(T* dptr, int len) : len(len), dptr(dptr) {
    }

    template<typename EType>
    inline Vec &operator=(const Exp<EType> &src_) {
        const EType &src = src_.self();
        for (int i = 0; i < len; ++i) {
            dptr[i] = src.Eval(i);
        }
        return *this;
    }

    inline T Eval(int i) const {
        return dptr[i];
    }
};

// template works for any expressions
template<typename OP, typename TLhs, typename TRhs>
inline BinaryMapExp<OP, TLhs, TRhs>
F(const Exp<TLhs> &lhs, const Exp<TRhs> &rhs) {
    return BinaryMapExp<OP, TLhs, TRhs>(lhs.self(), rhs.self());
}

namespace detail {

template<typename T> struct add {
    inline static T Map(const T& a, const T& b) {
        return a + b;
    }
};

template<typename T> struct sub {
    inline static T Map(const T& a, const T& b) {
        return a - b;
    }
};

template<typename T> struct mul {
    inline static T Map(const T& a, const T& b) {
        return a * b;
    }
};

template<typename T> struct div {
    inline static T Map(const T& a, const T& b) {
        return a / b;
    }
};

template<typename T> struct maximum {
    inline static T Map(const T& a, const T& b) {
        return a > b ? a : b;
    }
};

template<typename T> struct minimum {
    inline static T Map(const T& a, const T& b) {
        return a < b ? a : b;
    }
};

} // namespace detail

template<typename TLhs, typename TRhs>
inline BinaryMapExp<detail::add<typename ReturnType<TLhs, TRhs>::value_type>, TLhs, TRhs>
operator+(const Exp<TLhs> &lhs, const Exp<TRhs> &rhs) {
    return F<detail::add<typename ReturnType<TLhs, TRhs>::value_type> >(lhs, rhs);
}

template<typename TLhs, typename TRhs>
inline BinaryMapExp<detail::sub<typename ReturnType<TLhs, TRhs>::value_type>, TLhs, TRhs>
operator-(const Exp<TLhs> &lhs, const Exp<TRhs> &rhs) {
    return F<detail::sub<typename ReturnType<TLhs, TRhs>::value_type> >(lhs, rhs);
}

template<typename TLhs, typename TRhs>
inline BinaryMapExp<detail::mul<typename ReturnType<TLhs, TRhs>::value_type>, TLhs, TRhs>
operator*(const Exp<TLhs> &lhs, const Exp<TRhs> &rhs) {
    return F<detail::mul<typename ReturnType<TLhs, TRhs>::value_type> >(lhs, rhs);
}

template<typename TLhs, typename TRhs>
inline BinaryMapExp<detail::div<typename ReturnType<TLhs, TRhs>::value_type>, TLhs, TRhs>
operator/(const Exp<TLhs> &lhs, const Exp<TRhs> &rhs) {
    return F<detail::div<typename ReturnType<TLhs, TRhs>::value_type> >(lhs, rhs);
}

int main() {
    const int n = 3;
    float sa[n] = {1, 2, 3};
    float sb[n] = {2, 3, 4};
    float sc[n] = {3, 4, 5};
    Vec<float> A(sa, n), B(sb, n), C(sc, n);
    // run expression, this expression is longer:)
    A = B + C + C;
    for (int i = 0; i < n; ++i) {
        printf("%d:%f == %f + %f + %f\n", i,
               A.dptr[i], B.dptr[i], C.dptr[i], C.dptr[i]);
    }
    A = B * C + F<detail::maximum<float> >(B, C);
    for (int i = 0; i < n; ++i) {
        printf("%d:%f == %f * %f + max(%f, %f)\n", i,
               A.dptr[i], B.dptr[i], C.dptr[i], B.dptr[i], C.dptr[i]);
    }
    return 0;
}

#endif /* end of include guard */
