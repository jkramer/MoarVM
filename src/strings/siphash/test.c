#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <sys/time.h>
#define MVM_HASH_FORCE_LITTLE_ENDIAN 1
#include "csiphash.h"
uint64_t gettime_ns() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (uint64_t)tv.tv_sec * 1000000000ULL + tv.tv_usec * 1000ULL;
}

#define REPEATS 20000

uint64_t vectors[64] = {
	0x726fdb47dd0e0e31LLU, 0x74f839c593dc67fdLLU, 0x0d6c8009d9a94f5aLLU, 0x85676696d7fb7e2dLLU,
	0xcf2794e0277187b7LLU, 0x18765564cd99a68dLLU, 0xcbc9466e58fee3ceLLU, 0xab0200f58b01d137LLU,
	0x93f5f5799a932462LLU, 0x9e0082df0ba9e4b0LLU, 0x7a5dbbc594ddb9f3LLU, 0xf4b32f46226bada7LLU,
	0x751e8fbc860ee5fbLLU, 0x14ea5627c0843d90LLU, 0xf723ca908e7af2eeLLU, 0xa129ca6149be45e5LLU,
	0x3f2acc7f57c29bdbLLU, 0x699ae9f52cbe4794LLU, 0x4bc1b3f0968dd39cLLU, 0xbb6dc91da77961bdLLU,
	0xbed65cf21aa2ee98LLU, 0xd0f2cbb02e3b67c7LLU, 0x93536795e3a33e88LLU, 0xa80c038ccd5ccec8LLU,
	0xb8ad50c6f649af94LLU, 0xbce192de8a85b8eaLLU, 0x17d835b85bbb15f3LLU, 0x2f2e6163076bcfadLLU,
	0xde4daaaca71dc9a5LLU, 0xa6a2506687956571LLU, 0xad87a3535c49ef28LLU, 0x32d892fad841c342LLU,
	0x7127512f72f27cceLLU, 0xa7f32346f95978e3LLU, 0x12e0b01abb051238LLU, 0x15e034d40fa197aeLLU,
	0x314dffbe0815a3b4LLU, 0x027990f029623981LLU, 0xcadcd4e59ef40c4dLLU, 0x9abfd8766a33735cLLU,
	0x0e3ea96b5304a7d0LLU, 0xad0c42d6fc585992LLU, 0x187306c89bc215a9LLU, 0xd4a60abcf3792b95LLU,
	0xf935451de4f21df2LLU, 0xa9538f0419755787LLU, 0xdb9acddff56ca510LLU, 0xd06c98cd5c0975ebLLU,
	0xe612a3cb9ecba951LLU, 0xc766e62cfcadaf96LLU, 0xee64435a9752fe72LLU, 0xa192d576b245165aLLU,
	0x0a8787bf8ecb74b2LLU, 0x81b3e73d20b49b6fLLU, 0x7fa8220ba3b2eceaLLU, 0x245731c13ca42499LLU,
	0xb78dbfaf3a8d83bdLLU, 0xea1ad565322a1a0bLLU, 0x60e61c23a3795013LLU, 0x6606d7e446282b93LLU,
	0x6ca4ecb15c5f91e1LLU, 0x9f626da15c9625f3LLU, 0xe51b38608ef25f57LLU, 0x958a324ceb064572LLU,
};
#define MVMGrapheme32 int32_t
#define MVMint32 int32_t
#define MVMuint64 uint64_t
#define MVMuint8 uint8_t
typedef union {
	MVMint32 graphs[2];
	MVMuint8 bytes[4];
	uint64_t u64;
} MVMJenHashGraphemeView;
int testmvm (void) {
	size_t i;
	int rep_count = 0;
	char key[16] = {0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf};
	size_t s_len = 9;
	int32_t Grapheme32[9] = { 171, -72, 69, 76, 76, 79, 9829, 9826, 187  };
	int32_t Grapheme32_LE[9] = { 171, -72, 69, 76, 76, 79, 9829, 9826, 187  };
	for (i = 0; i < 9; i++) {
		Grapheme32[i] = MVM_TO_LITTLE_ENDIAN_32(Grapheme32[i]);
	}
	for (rep_count = 0; rep_count < REPEATS; rep_count++) {
		/* Using siphashfinish */
		{
			siphash sh;
			MVMuint64 hash;
			MVMJenHashGraphemeView gv;
			siphashinit(&sh, s_len * sizeof(MVMGrapheme32), (uint64_t*)key);
			for (i = 0; i + 1 < s_len;) {
				gv.graphs[0] = MVM_TO_LITTLE_ENDIAN_32(Grapheme32[i++]);
				gv.graphs[1] = MVM_TO_LITTLE_ENDIAN_32(Grapheme32[i++]);
				siphashadd64bits(&sh, gv.u64);
			}
			if (i < s_len) {
				//printf("some left");
				gv.graphs[0] = MVM_TO_LITTLE_ENDIAN_32(Grapheme32[i]);
				hash = siphashfinish(&sh, gv.bytes, sizeof(MVMGrapheme32));
			}
			else {
				hash = siphashfinish(&sh, NULL, 0);
			}
			assert(hash == 4563223716124497198LLU);
		}
		/* Using siphashfinish_32bits */
		{
			siphash sh;
			MVMuint64 hash;
			MVMJenHashGraphemeView gv;
			siphashinit(&sh, s_len * sizeof(MVMGrapheme32), (uint64_t*)key);
			for (i = 0; i + 1 < s_len;) {
				gv.graphs[0] = MVM_TO_LITTLE_ENDIAN_32(Grapheme32[i++]);
				gv.graphs[1] = MVM_TO_LITTLE_ENDIAN_32(Grapheme32[i++]);
				siphashadd64bits(&sh, gv.u64);
			}
			hash = siphashfinish_32bits(&sh, i < s_len ? MVM_TO_LITTLE_ENDIAN_32(Grapheme32[i]) : 0);
			assert(hash == 4563223716124497198LLU);
		}
		{
			assert(siphash24((uint8_t*)Grapheme32_LE, 9 * sizeof(int32_t), (uint64_t*)key) == 4563223716124497198LLU);
		}
	}
	return 0;
}
int main() {
	int i;
	char key[16] = {0,1,2,3,4,5,6,7,8,9,0xa,0xb,0xc,0xd,0xe,0xf};
	uint8_t plaintext[64];
	for (i=0; i<64; i++) plaintext[i] = i;
	int j;
	uint64_t t0, t1, t2, t3;
	t0 = gettime_ns();
	for (j=0; j<REPEATS; j++){
		for (i=0; i<64; i++) {
			assert(siphash24(plaintext, i, (uint64_t*)key) == vectors[i]);
		}
	}
	t1 = gettime_ns();
	printf("%i standard tests passed in %.3fms, %.0fns per test\n", REPEATS*64, (t1-t0)/1000000., (t1-t0)/(REPEATS*64.));
	t2 = gettime_ns();
	testmvm();
	t3 = gettime_ns();

	printf("%i MVM tests passed in %.3fms, %.0fns per test\n", REPEATS*3, (t3-t2)/1000000., (t3-t2)/(REPEATS*3.));
	return 0;
}
