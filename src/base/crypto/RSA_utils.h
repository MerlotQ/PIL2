#ifndef __RSA_UTILS_H__
#define __RSA_UTILS_H__

#include <stdint.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cstdlib>


namespace pi {
namespace crypto {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define capacity                1024                // large number stored size (with int type)
#define exponentiation_size     2049                // maximum key bit length

#define base                    0x40000000          // 2^30
#define __base                  0x20000000          // 2^29

#define base_bit                30
#define base_mod                0x3FFFFFFF
#define ascii_size              256


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

inline int sgn(int key)
{
    if (key > 0) return 1;
    if (key < 0) return -1;
    if (key == 0) return 0;
}

inline int abs(int key)
{
    return (key < 0) ? -key : key;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct output_BigInt
{
    int32_t len;
    int32_t __data[capacity];

    output_BigInt() : len(0) {}
    output_BigInt(const output_BigInt &source) : len(source.len)
    {
        memcpy(__data, source.__data, len * sizeof(int32_t));
    }

    output_BigInt(int key) : len(0)
    {
        for (; key > 0; key /= 1000000000)
            __data[len ++] = key % 1000000000;
    }

    int & operator [] (int Index) { return __data[Index]; }
    int operator [] (int Index) const { return __data[Index]; }
};


inline output_BigInt operator + (const output_BigInt &A, const output_BigInt &B)
{
    output_BigInt R;
    int i;
    int Carry = 0;
    for (i = 0; i < A.len || i < B.len || Carry > 0; ++ i)
    {
        if (i < A.len) Carry += A[i];
        if (i < B.len) Carry += B[i];
        R[i] = Carry % 1000000000;
        Carry /= 1000000000;
    }
    R.len = i;

    return R;
}

inline output_BigInt operator * (const output_BigInt &A, const int B)
{
    int i;
    if (B == 0) return 0;
    output_BigInt R;
    long long Carry = 0;
    for (i = 0; i < A.len || Carry > 0; ++ i)
    {
        if (i < A.len) Carry += (long long)(A[i]) * B;
        R[i] = Carry % 1000000000;
        Carry /= 1000000000;
    }
    R.len = i;

    return R;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct unsigned_BigInt
{
    int32_t len;
    int32_t __data[capacity];

    unsigned_BigInt() : len(0) {}
    unsigned_BigInt(const unsigned_BigInt &source) : len(source.len)
    {
        memcpy(__data, source.__data, len * sizeof(int32_t));
    }
    unsigned_BigInt(int key) : len(0)
    {
        for (; key > 0; key >>= base_bit)
            __data[len ++] = key & base_mod;
    }

    unsigned_BigInt & operator = (const unsigned_BigInt &key)
    {
        len = key.len;
        memcpy(__data, key.__data, len * sizeof(int32_t));
        return *this;
    }

    int & operator [] (int Index) { return __data[Index]; }
    int operator [] (int Index) const { return __data[Index]; }

    // encode/decode to/from string
    inline void encoder(const std::string& X);
    inline std::string decoder(void);

    // to/from string
    inline void fromString(const std::string& X);
    inline std::string toString(void);
};


inline int compare(const unsigned_BigInt &A, const unsigned_BigInt &B)
{
    if (A.len != B.len) return A.len > B.len ? 1 : -1;
    int i;
    for (i = A.len - 1; i >= 0 && A[i] == B[i]; -- i);
    if (i < 0) return 0;
    return A[i] > B[i] ? 1 : -1;
}


inline void shift_right_base(unsigned_BigInt &A)
{
    if (A.len == 0) return;
    for (int i = 0; i < A.len - 1; ++ i)
        A[i] = A[i + 1];
    -- A.len;
}

inline void shift_left_base(unsigned_BigInt &A)
{
    if (A.len == 0) return;
    for (int i = A.len; i > 0; -- i)
        A[i] = A[i - 1];
    A[0] = 0;
    ++ A.len;
}

inline void shift_right(unsigned_BigInt &A)
{
    if (A.len == 0) return;
    for (int i = 0; i < A.len - 1; ++ i)
    {
        A[i] = (A[i] >> 1);
        if ((A[i + 1] & 1) == 1)
            A[i] = A[i] | __base;
    }
    A[A.len - 1] = A[A.len - 1] >> 1;
    if (A[A.len - 1] == 0) -- A.len;
}

inline void shift_left(unsigned_BigInt &A)
{
    if (A.len == 0) return;
    int k = A.len;
    if ((A[A.len - 1] & __base) != 0)
        A[A.len ++] = 1;
    for (int i = k - 1; i > 0; -- i)
    {
        A[i] = (A[i] << 1) & base_mod;
        if ((A[i - 1] & __base) != 0)
            ++ A[i];
    }
    A[0] = (A[0] << 1) & base_mod;
}




inline unsigned_BigInt operator + (const unsigned_BigInt &A, const unsigned_BigInt &B)
{
    unsigned_BigInt R;
    int i;
    int Carry = 0;

    for (i = 0; i < A.len || i < B.len || Carry > 0; ++ i)
    {
        if (i < A.len) Carry += A[i];
        if (i < B.len) Carry += B[i];
        R[i] = Carry & base_mod;
        Carry >>= base_bit;
    }

    R.len = i;

    return R;
}

inline unsigned_BigInt operator - (const unsigned_BigInt &A, const unsigned_BigInt &B)
{
    unsigned_BigInt R;

    int Carry = 0;
    R.len = A.len;

    for (int i = 0; i < R.len; ++ i)
    {
        R[i] = A[i] - Carry;
        if (i < B.len) R[i] -= B[i];
        if (R[i] < 0) Carry = 1, R[i] += base;
        else Carry = 0;
    }

    while (R.len > 0 && R[R.len - 1] == 0) -- R.len;

    return R;
}

inline unsigned_BigInt operator * (const unsigned_BigInt &A, const int B)
{
    int i;
    unsigned_BigInt R;
    long long Carry = 0;

    if (B == 0) return 0;

    for (i = 0; i < A.len || Carry > 0; ++ i)
    {
        if (i < A.len) Carry += (long long)(A[i]) * B;
        R[i] = Carry & base_mod;
        Carry >>= base_bit;
    }

    R.len = i;

    return R;
}

inline unsigned_BigInt operator * (const unsigned_BigInt &A, const unsigned_BigInt &B)
{
    if (B.len == 0) return 0;
    unsigned_BigInt R;
    for (int i = 0; i < A.len; ++ i)
    {
        long long Carry = 0;
        for (int j = 0; j < B.len || Carry > 0; ++ j)
        {
            if (j < B.len) Carry += (long long)(A[i]) * B[j];
            if (i + j < R.len) Carry += R[i + j];
            if (i + j >= R.len) R[R.len ++] = Carry & base_mod;
            else R[i + j] = Carry & base_mod;
            Carry >>= base_bit;
        }
    }

    return R;
}



inline unsigned_BigInt operator / (const unsigned_BigInt &A, const int B)
{
    unsigned_BigInt R;
    long long C = 0;

    for (int i = A.len - 1; i >= 0; -- i)
    {
        C = (C << base_bit) + A[i];
        R[i] = C / B;
        C %= B;
    }

    R.len = A.len;
    while (R.len > 0 && R[R.len - 1] == 0) -- R.len;

    return R;

}

inline unsigned_BigInt operator % (const unsigned_BigInt &A, const int B)
{
    long long C = 0;

    for (int i = A.len - 1; i >= 0; -- i)
    {
        C = (C << base_bit) + A[i];
        C %= B;
    }

    return C;
}


inline unsigned_BigInt operator / (const unsigned_BigInt &A, const unsigned_BigInt &B)
{
    if (compare(A, B) < 0) return 0;

    unsigned_BigInt R, Carry = 0;
    int left, right, mid;
    for (int i = A.len - 1; i >= 0; -- i)
    {
        //Carry = Carry * base + A[i];
        shift_left_base(Carry);
        Carry = Carry + A[i];

        left = 0;
        right = base;
        while (left + 1 < right)
        {
            mid = (left + right) >> 1;
            if (compare(B * mid , Carry) <= 0)
                left = mid;
            else
                right = mid;
        }
        R[i] = left;
        Carry = Carry - B * left;
    }

    R.len = A.len;
    while (R.len > 0 && R[R.len - 1] == 0) -- R.len;

    return R;

}

inline unsigned_BigInt operator % (const unsigned_BigInt &A, const unsigned_BigInt &B)
{
    if (compare(A, B) < 0) return A;

    unsigned_BigInt Carry = 0;
    int left, right, mid;
    for (int i = A.len - 1; i >= 0; -- i)
    {
        //Carry = Carry * base + A[i];
        shift_left_base(Carry);
        Carry = Carry + A[i];

        left = 0;
        right = base;
        while (left + 1 < right)
        {
            mid = (left + right) >> 1;
            if (compare(B * mid, Carry) <= 0)
                left = mid;
            else
                right = mid;
        }
        Carry = Carry - B * left;
    }

    return Carry;

}

inline std::istream & operator >> (std::istream &In, unsigned_BigInt &A)
{
    char ch;
    for (A = 0; In >> ch;)
    {
        A = A * 10 + (ch - '0');
        if (In.peek() <= ' ') break;
    }

    return In;
}

inline std::ostream & operator << (std::ostream &Out, const unsigned_BigInt &A)
{
    if (A.len == 0)
    {
        Out << 0;
        return Out;
    }

    output_BigInt V = 0;
    for (int i = A.len - 1; i >= 0; -- i)
        V = V * base + A[i];


    Out << V[V.len - 1];
    for (int i = V.len - 2; i >= 0; -- i)
        for (int j = 100000000; j > 0; j /= 10)
            Out << V[i] / j % 10;
    Out << std::endl;

    if ( 0 )
    {
        int digit = 0;
        for (int k = 1; k <= V[V.len - 1]; k *= 10)
            ++ digit;

        digit += (V.len - 1) * 9;

        Out << "Total Digit = " << digit << std::endl;
    }

    return Out;
}


inline void unsigned_BigInt::encoder(const std::string& X)
{
    unsigned_BigInt& R = *this;
    R = 0;

    for (int i = 0; i < X.size(); ++ i)
        R = (R * ascii_size) + int(X[i]);
}

inline std::string unsigned_BigInt::decoder(void)
{
    unsigned_BigInt A = *this;
    unsigned_BigInt temp;
    std::string S = "";

    for (; A.len != 0; A = A / ascii_size)
    {
        temp = A % ascii_size;
        char ch = char(temp[0]);
        S = ch + S;
    }

    return S;
}

// to/from string
inline void unsigned_BigInt::fromString(const std::string& X)
{
    char ch;
    unsigned_BigInt& A = *this;

    A = 0;

    for(int i=0; i<X.size(); i++) {
        ch = X[i];
        A = A * 10 + (ch - '0');
    }
}

inline std::string unsigned_BigInt::toString(void)
{
    std::stringstream Out;
    unsigned_BigInt& A = *this;

    if (A.len == 0) {
        Out << 0;
    } else {
        output_BigInt V = 0;

        for (int i = A.len - 1; i >= 0; -- i)
            V = V * base + A[i];

        Out << V[V.len - 1];
        for (int i = V.len - 2; i >= 0; -- i)
            for (int j = 100000000; j > 0; j /= 10)
                Out << V[i] / j % 10;
    }

    return Out.str();
}

inline unsigned_BigInt encoder(const std::string& X)
{
    unsigned_BigInt R = 0;

    for (int i = 0; i < X.size(); ++ i)
        R = (R * ascii_size) + int(X[i]);

    return R;
}

inline std::string decoder(unsigned_BigInt A)
{
    unsigned_BigInt temp;
    std::string S = "";

    for (; A.len != 0; A = A / ascii_size)
    {
        temp = A % ascii_size;
        char ch = char(temp[0]);
        S = ch + S;
    }

    return S;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct signed_BigInt
{
    unsigned_BigInt     data;
    int                 sign;

    signed_BigInt() : data(0), sign(0) {}
    signed_BigInt(const signed_BigInt &source) : sign(source.sign)
    {
        data = source.data;
    }
    signed_BigInt(int key) : sign(sgn(key)), data(abs(key)) {}


    signed_BigInt & operator = (const signed_BigInt &key)
    {
        sign = key.sign;
        data = key.data;
        return *this;
    }

    int & operator [] (int Index) { return data[Index]; }
    int operator [] (int Index) const { return data[Index]; }

    std::string toString(void) {
        std::stringstream Out;

        std::string c = (sign == -1) ? "-" : "";
        Out << c << data;

        return Out.str();
    }
};


inline std::ostream & operator << (std::ostream &Out, const signed_BigInt &V)
{
    std::string c = (V.sign == -1) ? "-" : "";
    Out << c << V.data;

    return Out;
}

inline bool operator == (const signed_BigInt &A, const signed_BigInt &B)
{
    if (A.sign != B.sign) return false;
    return (compare(A.data, B.data) == 0);
}

inline signed_BigInt operator + (const signed_BigInt &A, const signed_BigInt &B)
{
    signed_BigInt R;

    if (A.sign == 0) return B;
    if (B.sign == 0) return A;

    if (A.sign * B.sign == 1)
    {
        R.data = A.data + B.data;
        R.sign = A.sign;
    }
    else if (A.sign == 1)
    {
        int k = compare(A.data, B.data);
        if (k == 1)
        {
            R.sign = 1;
            R.data = A.data - B.data;
        }
        else if (k == 0)
        {
            R.sign = 0;
            R.data = 0;
        }
        else if (k == -1)
        {
            R.sign = -1;
            R.data = B.data - A.data;
        }
    }
    else
    {
        int k = compare(B.data, A.data);
        if (k == 1)
        {
            R.sign = 1;
            R.data = B.data - A.data;
        }
        else if (k == 0)
        {
            R.sign = 0;
            R.data = 0;
        }
        else if (k == -1)
        {
            R.sign = -1;
            R.data = A.data - B.data;
        }
    }

    return R;
}

inline signed_BigInt operator - (const signed_BigInt &A)
{
    signed_BigInt R;

    R.sign = -A.sign;
    R.data = A.data;

    return R;
}

inline signed_BigInt operator - (const signed_BigInt &A, const signed_BigInt &B)
{
    signed_BigInt R;

    if (B.sign == 0) return A;
    if (A.sign == 0) return -B;

    if (A.sign * B.sign == -1)
    {
        R.sign = A.sign;
        R.data = A.data + B.data;
    }
    else
    {
        int k = compare(A.data, B.data);
        if (k == 0)
        {
            R.sign = 0;
            R.data = 0;
            return R;
        }
        if (A.sign == 1 && B.sign == 1)
        {
            if (k == -1)
            {
                R.data = B.data - A.data;
                R.sign = -1;
            }
            else
            {
                R.data = A.data - B.data;
                R.sign = 1;
            }
        }
        else
        {
            if (k == -1)
            {
                R.data = B.data - A.data;
                R.sign = 1;
            }
            else
            {
                R.data = A.data - B.data;
                R.sign = -1;
            }
        }
    }

    return R;
}

inline signed_BigInt operator * (const signed_BigInt &A, const signed_BigInt &B)
{
    signed_BigInt R;
    if (A.sign * B.sign == 0)
    {
        R.data = 0;
        R.sign = 0;
        return R;
    }
    R.data = A.data * B.data;
    R.sign = A.sign * B.sign;

    return R;
}

inline signed_BigInt operator / (const signed_BigInt &A, const signed_BigInt &B)
{
    signed_BigInt R;
    if (A.sign == 0)
    {
        R.data = 0;
        R.sign = 0;
        return R;
    }
    R.data = A.data / B.data;
    if (R.data.len == 0)
        R.sign = 0;
    else
        R.sign = A.sign * B.sign;
    return R;
}

inline signed_BigInt operator % (const signed_BigInt &A, const signed_BigInt &B)
{
    signed_BigInt R;

    if (A.sign == 0)
    {
        R.data = 0;
        R.sign = 0;
        return R;
    }

    R.data = A.data % B.data;
    if (R.data.len == 0)
    {
        R.sign = 0;
        return R;
    }

    R.sign = 1;
    if (A.sign * B.sign == -1)
        R.data = B.data - A.data % B.data;

    return R;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct BigInt_Exponentiation
{
    unsigned_BigInt         N;
    int32_t                 len, valid;
    int32_t                 data[exponentiation_size];

    BigInt_Exponentiation() : len(0) { memset(data, 0, sizeof(data)); }
    BigInt_Exponentiation(const BigInt_Exponentiation &source) : len(source.len)
    {
        memcpy(data, source.data, len * sizeof(int32_t));
    }

    int & operator [] (int Index) { return data[Index]; }
    int operator [] (int Index) const { return data[Index]; }
};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

inline unsigned_BigInt Bin_To_Int(const BigInt_Exponentiation &W)
{
    unsigned_BigInt R;
    R.len = 0;
    int i, k;

    for (i = 0; i + base_bit < W.len; i += base_bit)
    {
        k = 0;
        for (int j = base_bit - 1; j >= 0; -- j)
            k = (k << 1) + W[i + j];
        R[R.len ++] = k;
    }

    if (i < W.len)
    {
        k = 0;
        for (int j = W.len - 1; j >= i; -- j)
            k = (k << 1) + W[j];
        R[R.len ++] = k;
    }

    return R;
}


inline unsigned_BigInt Modular_Exponentiation(unsigned_BigInt A, unsigned_BigInt B, const unsigned_BigInt &N)
{
    unsigned_BigInt R = 1;

    while (B.len != 0)
    {
        if ((B[0] & 1) == 1)
            R = (R * A) % N;
        A = (A * A) % N;
        shift_right(B);
    }

    return R;
}


inline signed_BigInt Euclid_GCD(const signed_BigInt &A, const signed_BigInt &B)
{
    return (B.sign == 0) ? A : Euclid_GCD(B, A % B);
}


inline signed_BigInt Extended_Euclid_GCD(const signed_BigInt &A, const signed_BigInt &B,
                                         signed_BigInt &X, signed_BigInt &Y)
{
    if (B.sign == 0)
    {
        X = 1, Y = 0;
        return A;
    }
    else
    {
        signed_BigInt XX, YY;
        signed_BigInt D = Extended_Euclid_GCD(B, A % B, XX, YY);

        X = YY, Y = XX - (A / B) * YY;

        return D;
    }
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// usefull big numbers
const unsigned_BigInt   unsigned_Zero = 0;
const unsigned_BigInt   unsigned_One  = 1;
const signed_BigInt     Zero = 0;
const signed_BigInt     One = 1;


}} // end of namespace pi::crypto



#endif // end of __RSA_UTILS_H__


