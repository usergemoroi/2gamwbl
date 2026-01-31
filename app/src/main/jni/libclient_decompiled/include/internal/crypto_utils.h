#ifndef LIBCLIENT_CRYPTO_UTILS_H
#define LIBCLIENT_CRYPTO_UTILS_H

#include <string>
#include <vector>
#include <cstdint>

namespace client {
namespace crypto {

class AES {
public:
    static bool encrypt(const uint8_t* input, size_t input_len,
                       const uint8_t* key, size_t key_len,
                       const uint8_t* iv, size_t iv_len,
                       std::vector<uint8_t>& output);
    
    static bool decrypt(const uint8_t* input, size_t input_len,
                       const uint8_t* key, size_t key_len,
                       const uint8_t* iv, size_t iv_len,
                       std::vector<uint8_t>& output);
    
    static bool encryptCBC(const std::vector<uint8_t>& plaintext,
                          const std::vector<uint8_t>& key,
                          const std::vector<uint8_t>& iv,
                          std::vector<uint8_t>& ciphertext);
    
    static bool decryptCBC(const std::vector<uint8_t>& ciphertext,
                          const std::vector<uint8_t>& key,
                          const std::vector<uint8_t>& iv,
                          std::vector<uint8_t>& plaintext);
};

class Hash {
public:
    static std::string md5(const std::string& input);
    static std::string md5(const uint8_t* data, size_t length);
    
    static std::string sha1(const std::string& input);
    static std::string sha1(const uint8_t* data, size_t length);
    
    static std::string sha256(const std::string& input);
    static std::string sha256(const uint8_t* data, size_t length);
    
    static std::string sha512(const std::string& input);
    static std::string sha512(const uint8_t* data, size_t length);
};

class Base64 {
public:
    static std::string encode(const uint8_t* data, size_t length);
    static std::string encode(const std::string& input);
    static std::vector<uint8_t> decode(const std::string& input);
    static bool decode(const std::string& input, std::vector<uint8_t>& output);
};

class Hex {
public:
    static std::string encode(const uint8_t* data, size_t length);
    static std::string encode(const std::vector<uint8_t>& data);
    static std::vector<uint8_t> decode(const std::string& hex);
    static bool decode(const std::string& hex, std::vector<uint8_t>& output);
};

class CRC {
public:
    static uint32_t crc32(const uint8_t* data, size_t length);
    static uint32_t crc32(const std::string& input);
    static uint32_t crc32(const std::vector<uint8_t>& data);
};

class Random {
public:
    static void initialize();
    static void bytes(uint8_t* output, size_t length);
    static std::vector<uint8_t> bytes(size_t length);
    static uint32_t uint32();
    static uint64_t uint64();
    static std::string string(size_t length);
};

// Burri encryption scheme
class BurriCrypto {
public:
    static bool encrypt(const std::vector<uint8_t>& plaintext,
                       std::vector<uint8_t>& ciphertext);
    
    static bool decrypt(const std::vector<uint8_t>& ciphertext,
                       std::vector<uint8_t>& plaintext);
    
    static bool encryptFile(const std::string& input_path,
                           const std::string& output_path);
    
    static bool decryptFile(const std::string& input_path,
                           const std::string& output_path);
    
private:
    static const uint8_t kBurriKey[];
    static const uint8_t kBurriIV[];
    static constexpr size_t kKeySize = 32;
    static constexpr size_t kIVSize = 16;
};

} // namespace crypto
} // namespace client

#endif // LIBCLIENT_CRYPTO_UTILS_H
