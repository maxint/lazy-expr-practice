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

// addition operation
template<typename TLhs, typename TRhs>
struct BinaryAddExp : Exp<BinaryAddExp<TLhs, TRhs> > {
    const TLhs &lhs;
    const TRhs &rhs;
    typedef typename TLhs::value_type value_type;

    BinaryAddExp(const TLhs &lhs, const TRhs &rhs)
        : lhs(lhs), rhs(rhs) {}

    inline value_type Eval(int i) const {
        return lhs.Eval(i) + rhs.Eval(i);
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

// do addition
template<typename TLhs, typename TRhs>
BinaryAddExp<TLhs, TRhs> operator+(const Exp<TLhs> &lhs, const Exp<TRhs> &rhs) {
    return BinaryAddExp<TLhs, TRhs>(lhs.self(), rhs.self());
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
    return 0;
}

#endif /* end of include guard */
