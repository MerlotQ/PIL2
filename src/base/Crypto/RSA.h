#ifndef __RSA_H__
#define __RSA_H__

#include <string>

namespace pi {
namespace crypto {


///
/// \brief The RSA Key Pair
///
struct RSA_KeyPair {
public:
    std::string     N;          ///< N = P * Q
    std::string     ED;         ///< E for public key, D for private key

public:
    RSA_KeyPair() : N(""), ED("") {}
    RSA_KeyPair(const std::string& keyN, const std::string& keyED) : N(keyN), ED(keyED) {}

    int isKeySet(void) {
        return N.size()>0 && ED.size()>0;
    }

    int load(const std::string& fname);
    int save(const std::string& fname);
};


///
/// \brief Generate keys for RSA
///
/// \param primeBitsNum         - bits for prime number
/// \param nTrial               - Trial number
/// \param fnKey                - file name of key
/// \param fnPublicKey          - file name of public key
/// \param fnPrivateKey         - file name of private key
///
void RSA_genKey(int primeBitsNum, int nTrial,
                const std::string& fnKey, const std::string& fnPublicKey, const std::string& fnPrivateKey);


///
/// \brief Generate keys for RSA
///         {N, E} is public key
///         {N, D} is private key
///
/// \param primeBitsNum         - bits for prime number
/// \param nTrial               - trial number
/// \param sP                   - first prime number
/// \param sQ                   - second prime number
/// \param sN                   - P * Q
/// \param sE                   -
/// \param sD                   -
///
void RSA_genKey(int primeBitsNum, int nTrial,
                std::string& sP, std::string& sQ,
                std::string& sN, std::string& sE, std::string& sD);

///
/// \brief Generate keys for RSA
///
/// \param primeBitsNum         - bits for prime number
/// \param nTrial               - Trial number
/// \param pubKey               - public key
/// \param priKey               - private key
///
void RSA_genKey(int primeBitsNum, int nTrial,
                RSA_KeyPair &pubKey, RSA_KeyPair &priKey);


///
/// \brief encode given message by RSA signature
///
/// \param msg                  - input message
/// \param msgOut               - encoded message
/// \param sendPriKey           - sender's private key
/// \param recvPubKey           - recevier's public key
///
/// \return
///
int RSA_encodeMsg(const std::string& msg, std::string& msgOut,
                  const RSA_KeyPair& sendPriKey, const RSA_KeyPair& recvPubKey);

///
/// \brief decode message from RSA signature
///
/// \param msg                  - input signatured message
/// \param msgOut               - output decoded message
/// \param sendPubKey           - sender's public key
/// \param recvPriKey           - receiver's private key
///
/// \return
///
int RSA_decodeMsg(const std::string& msg, std::string& msgOut,
                  const RSA_KeyPair& sendPubKey, const RSA_KeyPair& recvPriKey);


}} // end of namespace pi::crypto


#endif // end of __RSA_H__
