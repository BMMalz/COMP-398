#ifndef __SHA256_H
#define __SHA256_H

void calc_sha_256(__uint8_t hash[32], const void *input, size_t len);

void sha_256_string(char string[65], const void *input, size_t len);

#endif
