#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <openssl/evp.h>

char *md5(const char *str, int length) {
    EVP_MD_CTX *mdctx;
    unsigned int md5_digest_len = EVP_MD_size(EVP_md5());
    uint8_t md5_digest[md5_digest_len];
    char *hexdigest = (char*) malloc(md5_digest_len * 2 + 1);

    // MD5_Init
    mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, EVP_md5(), NULL);

    // MD5_Update
    while (length > 0) {
        if (length > 512) {
            EVP_DigestUpdate(mdctx, str, 512);
        } else {
            EVP_DigestUpdate(mdctx, str, length);
        }
        length -= 512;
        str += 512;
    }

    // MD5_Final
    EVP_DigestFinal_ex(mdctx, md5_digest, &md5_digest_len);
    EVP_MD_CTX_free(mdctx);

    for (int n = 0; n < md5_digest_len; ++n) {
        snprintf(hexdigest + n*2, 3, "%02x", md5_digest[n]);
    }

    hexdigest[md5_digest_len * 2] = '\0';

    return hexdigest;
}

char *md5file(const char *filename)
{
    FILE *in = fopen(filename, "rb");
    if (!in)
    {
        perror("md5file(), can't open file");
        return NULL;
    }

    EVP_MD_CTX *mdctx;
    unsigned int md5_digest_len = EVP_MD_size(EVP_md5());
    uint8_t md5_digest[md5_digest_len];
    char *hexdigest = (char*) malloc(md5_digest_len * 2 + 1);

    // MD5_Init
    mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, EVP_md5(), NULL);

    // MD5_Update
    int length;
    char buf[512];
    while ((length = fread(buf, 1, 512, in)) > 0) {
        EVP_DigestUpdate(mdctx, buf, length);
    }

    fclose(in);

    // MD5_Final
    EVP_DigestFinal_ex(mdctx, md5_digest, &md5_digest_len);
    EVP_MD_CTX_free(mdctx);

    for (int n = 0; n < md5_digest_len; ++n) {
        snprintf(hexdigest + n*2, 3, "%02x", md5_digest[n]);
    }

    hexdigest[md5_digest_len * 2] = '\0';

    return hexdigest;
}