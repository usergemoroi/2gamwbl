/*
 * Decompiled from libe6bmfqax5v.so (arm64-v8a)
 * GameBlaster Pro v3.1 - Auxiliary Library
 * 
 * This library provides auxiliary functions for encryption, decryption,
 * and runtime code manipulation.
 * 
 * Binary size: 846KB
 * Architecture: ARM64-v8a (AArch64)
 */

#include <jni.h>
#include <android/log.h>
#include <string>
#include <vector>
#include <cstring>
#include <openssl/aes.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <zlib.h>

#define LOG_TAG "AuxiliaryLib"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace auxiliary {

// Encryption/Decryption utilities
class CryptoUtils {
public:
    static const unsigned char BURRI_KEY[32];
    static const unsigned char BURRI_IV[16];
    
    static std::vector<uint8_t> aesDecrypt(const uint8_t* data, size_t length) {
        std::vector<uint8_t> output;
        
        if (length == 0 || data == nullptr) {
            LOGE("Invalid input for AES decryption");
            return output;
        }
        
        output.resize(length);
        
        AES_KEY aes_key;
        AES_set_decrypt_key(BURRI_KEY, 256, &aes_key);
        
        // Decrypt with AES-256-CBC
        unsigned char iv[16];
        memcpy(iv, BURRI_IV, 16);
        
        AES_cbc_encrypt(data, output.data(), length, &aes_key, iv, AES_DECRYPT);
        
        // Remove PKCS7 padding
        if (!output.empty()) {
            uint8_t padding = output[output.size() - 1];
            if (padding > 0 && padding <= 16) {
                output.resize(output.size() - padding);
            }
        }
        
        return output;
    }
    
    static std::vector<uint8_t> aesEncrypt(const uint8_t* data, size_t length) {
        std::vector<uint8_t> output;
        
        if (length == 0 || data == nullptr) {
            LOGE("Invalid input for AES encryption");
            return output;
        }
        
        // Add PKCS7 padding
        size_t padding = 16 - (length % 16);
        size_t padded_length = length + padding;
        
        std::vector<uint8_t> padded_data(padded_length);
        memcpy(padded_data.data(), data, length);
        memset(padded_data.data() + length, padding, padding);
        
        output.resize(padded_length);
        
        AES_KEY aes_key;
        AES_set_encrypt_key(BURRI_KEY, 256, &aes_key);
        
        // Encrypt with AES-256-CBC
        unsigned char iv[16];
        memcpy(iv, BURRI_IV, 16);
        
        AES_cbc_encrypt(padded_data.data(), output.data(), padded_length, 
                       &aes_key, iv, AES_ENCRYPT);
        
        return output;
    }
    
    static std::string md5Hash(const uint8_t* data, size_t length) {
        unsigned char hash[MD5_DIGEST_LENGTH];
        MD5(data, length, hash);
        
        char hex[MD5_DIGEST_LENGTH * 2 + 1];
        for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
            sprintf(hex + i * 2, "%02x", hash[i]);
        }
        hex[MD5_DIGEST_LENGTH * 2] = '\0';
        
        return std::string(hex);
    }
    
    static std::string sha256Hash(const uint8_t* data, size_t length) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256(data, length, hash);
        
        char hex[SHA256_DIGEST_LENGTH * 2 + 1];
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            sprintf(hex + i * 2, "%02x", hash[i]);
        }
        hex[SHA256_DIGEST_LENGTH * 2] = '\0';
        
        return std::string(hex);
    }
};

// Burri encryption keys (would be obfuscated in actual binary)
const unsigned char CryptoUtils::BURRI_KEY[32] = {
    0x42, 0x75, 0x72, 0x72, 0x69, 0x20, 0x42, 0x75,
    0x72, 0x72, 0x69, 0x20, 0x45, 0x6e, 0x63, 0x72,
    0x79, 0x70, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x4b,
    0x65, 0x79, 0x20, 0x76, 0x33, 0x2e, 0x31, 0x00
};

const unsigned char CryptoUtils::BURRI_IV[16] = {
    0x47, 0x61, 0x6d, 0x65, 0x42, 0x6c, 0x61, 0x73,
    0x74, 0x65, 0x72, 0x50, 0x72, 0x6f, 0x00, 0x00
};

// Compression utilities
class CompressionUtils {
public:
    static std::vector<uint8_t> decompress(const uint8_t* data, size_t length) {
        std::vector<uint8_t> output;
        
        z_stream stream;
        stream.zalloc = Z_NULL;
        stream.zfree = Z_NULL;
        stream.opaque = Z_NULL;
        stream.avail_in = length;
        stream.next_in = const_cast<uint8_t*>(data);
        
        if (inflateInit(&stream) != Z_OK) {
            LOGE("Failed to initialize decompression");
            return output;
        }
        
        const size_t CHUNK_SIZE = 16384;
        uint8_t chunk[CHUNK_SIZE];
        
        do {
            stream.avail_out = CHUNK_SIZE;
            stream.next_out = chunk;
            
            int ret = inflate(&stream, Z_NO_FLUSH);
            if (ret == Z_STREAM_ERROR || ret == Z_NEED_DICT || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
                LOGE("Decompression error: %d", ret);
                inflateEnd(&stream);
                return output;
            }
            
            size_t have = CHUNK_SIZE - stream.avail_out;
            output.insert(output.end(), chunk, chunk + have);
        } while (stream.avail_out == 0);
        
        inflateEnd(&stream);
        return output;
    }
    
    static std::vector<uint8_t> compress(const uint8_t* data, size_t length) {
        std::vector<uint8_t> output;
        
        z_stream stream;
        stream.zalloc = Z_NULL;
        stream.zfree = Z_NULL;
        stream.opaque = Z_NULL;
        
        if (deflateInit(&stream, Z_BEST_COMPRESSION) != Z_OK) {
            LOGE("Failed to initialize compression");
            return output;
        }
        
        stream.avail_in = length;
        stream.next_in = const_cast<uint8_t*>(data);
        
        const size_t CHUNK_SIZE = 16384;
        uint8_t chunk[CHUNK_SIZE];
        
        do {
            stream.avail_out = CHUNK_SIZE;
            stream.next_out = chunk;
            
            int ret = deflate(&stream, Z_FINISH);
            if (ret == Z_STREAM_ERROR) {
                LOGE("Compression error");
                deflateEnd(&stream);
                return output;
            }
            
            size_t have = CHUNK_SIZE - stream.avail_out;
            output.insert(output.end(), chunk, chunk + have);
        } while (stream.avail_out == 0);
        
        deflateEnd(&stream);
        return output;
    }
};

// Asset decryption - decrypts files from burriEnc
class AssetDecryptor {
public:
    static std::vector<uint8_t> decryptAsset(const uint8_t* encrypted_data, size_t length) {
        LOGI("Decrypting asset (%zu bytes)", length);
        
        // First, decrypt with AES
        std::vector<uint8_t> decrypted = CryptoUtils::aesDecrypt(encrypted_data, length);
        
        if (decrypted.empty()) {
            LOGE("AES decryption failed");
            return decrypted;
        }
        
        // Then, decompress
        std::vector<uint8_t> decompressed = CompressionUtils::decompress(
            decrypted.data(), decrypted.size());
        
        if (decompressed.empty()) {
            LOGW("Decompression failed or not compressed");
            return decrypted;
        }
        
        LOGI("Asset decrypted successfully (%zu -> %zu bytes)", 
             length, decompressed.size());
        
        return decompressed;
    }
    
    static bool verifyChecksum(const uint8_t* data, size_t length, const std::string& expected_md5) {
        std::string actual_md5 = CryptoUtils::md5Hash(data, length);
        bool valid = (actual_md5 == expected_md5);
        
        if (!valid) {
            LOGE("Checksum mismatch: expected %s, got %s", 
                 expected_md5.c_str(), actual_md5.c_str());
        }
        
        return valid;
    }
};

// String obfuscation utilities
class StringObfuscator {
public:
    static std::string deobfuscate(const char* obfuscated) {
        // Simple XOR deobfuscation
        std::string result;
        size_t len = strlen(obfuscated);
        const uint8_t key = 0x42;
        
        for (size_t i = 0; i < len; i++) {
            result += (char)(obfuscated[i] ^ key ^ (i & 0xFF));
        }
        
        return result;
    }
    
    static std::string obfuscate(const std::string& plain) {
        std::string result;
        const uint8_t key = 0x42;
        
        for (size_t i = 0; i < plain.length(); i++) {
            result += (char)(plain[i] ^ key ^ (i & 0xFF));
        }
        
        return result;
    }
};

} // namespace auxiliary

// JNI Implementation
extern "C" {

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGI("libe6bmfqax5v.so loaded - Auxiliary Library");
    return JNI_VERSION_1_6;
}

// Decrypt asset file
JNIEXPORT jbyteArray JNICALL
Java_auxiliarylib_CryptoHelper_decryptAsset(JNIEnv* env, jclass clazz, jbyteArray encrypted) {
    jsize length = env->GetArrayLength(encrypted);
    jbyte* data = env->GetByteArrayElements(encrypted, nullptr);
    
    std::vector<uint8_t> decrypted = auxiliary::AssetDecryptor::decryptAsset(
        reinterpret_cast<uint8_t*>(data), length);
    
    env->ReleaseByteArrayElements(encrypted, data, JNI_ABORT);
    
    if (decrypted.empty()) {
        return nullptr;
    }
    
    jbyteArray result = env->NewByteArray(decrypted.size());
    env->SetByteArrayRegion(result, 0, decrypted.size(), 
                           reinterpret_cast<const jbyte*>(decrypted.data()));
    
    return result;
}

// Compute MD5 hash
JNIEXPORT jstring JNICALL
Java_auxiliarylib_CryptoHelper_md5(JNIEnv* env, jclass clazz, jbyteArray data) {
    jsize length = env->GetArrayLength(data);
    jbyte* bytes = env->GetByteArrayElements(data, nullptr);
    
    std::string hash = auxiliary::CryptoUtils::md5Hash(
        reinterpret_cast<uint8_t*>(bytes), length);
    
    env->ReleaseByteArrayElements(data, bytes, JNI_ABORT);
    
    return env->NewStringUTF(hash.c_str());
}

// Compute SHA256 hash
JNIEXPORT jstring JNICALL
Java_auxiliarylib_CryptoHelper_sha256(JNIEnv* env, jclass clazz, jbyteArray data) {
    jsize length = env->GetArrayLength(data);
    jbyte* bytes = env->GetByteArrayElements(data, nullptr);
    
    std::string hash = auxiliary::CryptoUtils::sha256Hash(
        reinterpret_cast<uint8_t*>(bytes), length);
    
    env->ReleaseByteArrayElements(data, bytes, JNI_ABORT);
    
    return env->NewStringUTF(hash.c_str());
}

// Decompress data
JNIEXPORT jbyteArray JNICALL
Java_auxiliarylib_CompressionHelper_decompress(JNIEnv* env, jclass clazz, jbyteArray compressed) {
    jsize length = env->GetArrayLength(compressed);
    jbyte* data = env->GetByteArrayElements(compressed, nullptr);
    
    std::vector<uint8_t> decompressed = auxiliary::CompressionUtils::decompress(
        reinterpret_cast<uint8_t*>(data), length);
    
    env->ReleaseByteArrayElements(compressed, data, JNI_ABORT);
    
    if (decompressed.empty()) {
        return nullptr;
    }
    
    jbyteArray result = env->NewByteArray(decompressed.size());
    env->SetByteArrayRegion(result, 0, decompressed.size(),
                           reinterpret_cast<const jbyte*>(decompressed.data()));
    
    return result;
}

// Deobfuscate string
JNIEXPORT jstring JNICALL
Java_auxiliarylib_StringHelper_deobfuscate(JNIEnv* env, jclass clazz, jstring obfuscated) {
    const char* str = env->GetStringUTFChars(obfuscated, nullptr);
    
    std::string deobfuscated = auxiliary::StringObfuscator::deobfuscate(str);
    
    env->ReleaseStringUTFChars(obfuscated, str);
    
    return env->NewStringUTF(deobfuscated.c_str());
}

} // extern "C"
