
#include <time.h>

#include <iostream>
#include <fstream>

#include "RSA_utils.h"
#include "RSA.h"

using namespace std;


namespace pi {
namespace crypto {


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define some_prime  25

const int Prime[some_prime] = {
                                2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37,
                                41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97
                              };


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

unsigned_BigInt Modular_Exponentiation_ex(unsigned_BigInt A, const BigInt_Exponentiation &W)
{
    unsigned_BigInt R = 1;
    for (int i = W.valid; i < W.len; ++ i)
    {
        if (W[i] == 1)
            R = (R * A) % W.N;
        A = (A * A) % W.N;
    }

    return R;
}

// LSB --> MSB   scheme: (10000)empty(1xxxxx1)full
BigInt_Exponentiation Get_Random_Binary(const int full, const int empty)
{
    BigInt_Exponentiation X;
    X.len = full + empty;
    X.valid = empty;  // Start from empty
    X[0] = X[empty] = 1;
    for (int i = empty + 1; i < X.len - 1; ++ i)
        X[i] = (rand() & 1);
    X[X.len - 1] = 1;
    X.N = Bin_To_Int(X);

    return X;
}


bool Miller_Rabin_Witness(const unsigned_BigInt &A, const BigInt_Exponentiation &W,
                          unsigned_BigInt& Minus_One)
{
    int t = W.valid;

    unsigned_BigInt X[2];
    X[0] = Modular_Exponentiation_ex(A, W);

    for (int i = 1; i <= t; ++ i)
    {
        X[i & 1] = (X[(i - 1) & 1] * X[(i - 1) & 1]) % W.N;
        if (    compare(X[i & 1], unsigned_One) == 0 &&
                compare(X[(i - 1) & 1], unsigned_One) != 0 &&
                compare(X[(i - 1) & 1], Minus_One) != 0 )
        {
            return true;
        }
    }

    if (compare(X[t & 1], unsigned_One) != 0)
    {
        return true;
    }

    return false;
}

bool Miller_Rabin_Primality_Test(const BigInt_Exponentiation &W, const int Trial)
{
    unsigned_BigInt Minus_One;
    unsigned_BigInt A;

    for (int i = 0; i < some_prime; ++ i)
        if ((W.N % Prime[i]).len == 0) return false;

    Minus_One = W.N - 1;


    if (Miller_Rabin_Witness(2, W, Minus_One)) return false;
    for (int i = 1; i < Trial; ++ i)
    {

        A = Get_Random_Binary(W.len - 2, 1).N;

        if (Miller_Rabin_Witness(A, W, Minus_One)) return false;
    }

    return true;
}


signed_BigInt Get_Prime(const int Prime_Bits, const int Trial)
{
    int delta = rand() % (Prime_Bits / 20) + 1;
    int bits = Prime_Bits + delta;

    int noisy = rand() % 100 + 100;

    int counter = 0;
    BigInt_Exponentiation W;

    while (1)
    {
        ++ counter;

        W = Get_Random_Binary(bits - noisy, noisy);    // empty >= 1


        if (Miller_Rabin_Primality_Test(W, Trial))
        {
            //cout << "Total Trial = " << counter << endl;

            signed_BigInt R;
            R.sign = 1;
            R.data = W.N;

            return R;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int RSA_KeyPair::load(const std::string& fname)
{
    ifstream fnKey(fname.c_str());

    if( fnKey.is_open() ) {
        fnKey >> N >> ED;
        fnKey.close();

        return 0;
    }

    return -1;
}

int RSA_KeyPair::save(const std::string& fname)
{
    ofstream fnKey(fname.c_str());

    if( fnKey.is_open() ) {
        fnKey << N << std::endl << ED;
        fnKey.close();

        return 0;
    }

    return -1;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void RSA_genKey(int primeBitsNum, int nTrial,
                const std::string& fnKey, const std::string& fnPublicKey, const std::string& fnPrivateKey)
{
    ofstream fout;
    ofstream public_out;
    ofstream private_out;

    // set random seed
    srand(time(NULL));


    signed_BigInt P = Get_Prime(primeBitsNum, nTrial), Q = Get_Prime(primeBitsNum, nTrial);
    signed_BigInt Phi = (P - 1) * (Q - 1);
    signed_BigInt N = P * Q;

    signed_BigInt D, E, temp;

    for (E = 7; !(Extended_Euclid_GCD(E, Phi, D, temp) == One); E = E + 2);
    if (D.sign == -1)
        D = D + Phi;

    // open output file
    fout.open(fnKey.c_str());
    public_out.open(fnPublicKey.c_str());
    private_out.open(fnPrivateKey.c_str());

    // write keys
    fout << "P = " << endl;
    fout << P << endl;
    fout << endl;
    fout << "Q = " << endl;
    fout << Q << endl;
    fout << endl;

    fout << "N = " << endl;
    fout << N << endl;
    fout << endl;
    fout << "E = " << endl;
    fout << E << endl;
    fout << endl;
    fout << "D = " << endl;
    fout << D << endl;

    public_out << N;
    public_out << E;

    private_out << N;
    private_out << D;

    fout.close();
    public_out.close();
    private_out.close();
}

void RSA_genKey(int primeBitsNum, int nTrial,
                std::string& sP, std::string& sQ,
                std::string& sN, std::string& sE, std::string& sD)
{
    // set random seed
    srand(time(NULL));


    signed_BigInt P = Get_Prime(primeBitsNum, nTrial), Q = Get_Prime(primeBitsNum, nTrial);
    signed_BigInt Phi = (P - 1) * (Q - 1);
    signed_BigInt N = P * Q;

    signed_BigInt D, E, temp;

    for (E = 7; !(Extended_Euclid_GCD(E, Phi, D, temp) == One); E = E + 2);
    if (D.sign == -1)
        D = D + Phi;

    sP = P.toString();
    sQ = Q.toString();

    sN = N.toString();
    sE = E.toString();
    sD = D.toString();
}

void RSA_genKey(int primeBitsNum, int nTrial,
                RSA_KeyPair &pubKey, RSA_KeyPair &priKey)
{
    // set random seed
    srand(time(NULL));


    signed_BigInt P = Get_Prime(primeBitsNum, nTrial), Q = Get_Prime(primeBitsNum, nTrial);
    signed_BigInt Phi = (P - 1) * (Q - 1);
    signed_BigInt N = P * Q;

    signed_BigInt D, E, temp;

    for (E = 7; !(Extended_Euclid_GCD(E, Phi, D, temp) == One); E = E + 2);
    if (D.sign == -1)
        D = D + Phi;

    pubKey.N  = N.toString();
    pubKey.ED = E.toString();

    priKey.N  = pubKey.N;
    priKey.ED = D.toString();
}


int RSA_encodeMsg(const std::string& msg, std::string& msgOut,
                  const RSA_KeyPair& sendPriKey, const RSA_KeyPair& recvPubKey)
{
    unsigned_BigInt N_A, N_B, D, E;
    unsigned_BigInt R;

    R.encoder(msg);

    N_A.fromString(sendPriKey.N);
    D.fromString(sendPriKey.ED);

    N_B.fromString(recvPubKey.N);
    E.fromString(recvPubKey.ED);

    R = Modular_Exponentiation(R, D, N_A);
    R = Modular_Exponentiation(R, E, N_B);

    msgOut = R.toString();

    return 0;
}


int RSA_decodeMsg(const std::string& msg, std::string& msgOut,
                  const RSA_KeyPair& sendPubKey, const RSA_KeyPair& recvPriKey)
{
    unsigned_BigInt N_A, N_B, D, E;
    unsigned_BigInt R;

    R.fromString(msg);

    N_A.fromString(recvPriKey.N);
    D.fromString(recvPriKey.ED);

    N_B.fromString(sendPubKey.N);
    E.fromString(sendPubKey.ED);

    R = Modular_Exponentiation(R, D, N_A);
    R = Modular_Exponentiation(R, E, N_B);

    msgOut = R.decoder();

    return 0;
}

}} // end of namespace pi::crypto
