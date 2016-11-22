#include "soundinvade3.h"
#include <stdint.h>

uint16_t soundinvade3_sample_rate = 11025;
uint16_t soundinvade3_num_samples = 1054;
uint8_t soundinvade3[] = {139, 139, 139, 144, 139, 144, 139, 144, 144, 139, 139, 144, 139, 144, 144, 139, 144, 139, 139, 144, 144, 139, 144, 144, 139, 144, 139, 139, 139, 139, 139, 144, 139, 144, 144, 139, 144, 139, 144, 139, 144, 139, 144, 139, 139, 144, 144, 139, 144, 139, 139, 144, 144, 139, 144, 144, 139, 144, 144, 139, 139, 144, 139, 139, 139, 133, 139, 144, 139, 133, 139, 144, 139, 133, 139, 133, 139, 139, 133, 128, 133, 133, 128, 128, 133, 133, 128, 133, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 122, 128, 128, 128, 122, 128, 122, 116, 122, 116, 122, 116, 122, 122, 116, 116, 116, 111, 116, 111, 116, 122, 116, 122, 116, 116, 122, 122, 116, 122, 116, 116, 122, 116, 116, 116, 116, 111, 116, 111, 111, 116, 111, 116, 116, 111, 116, 111, 116, 116, 111, 111, 111, 105, 111, 105, 111, 111, 105, 105, 111, 105, 111, 105, 99, 105, 105, 99, 105, 105, 105, 99, 105, 105, 105, 111, 111, 105, 111, 105, 99, 105, 99, 105, 99, 105, 99, 105, 105, 105, 105, 111, 105, 111, 105, 111, 111, 105, 111, 116, 111, 111, 116, 111, 111, 116, 116, 111, 116, 122, 116, 122, 116, 116, 122, 128, 122, 128, 122, 128, 122, 122, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 133, 128, 133, 133, 139, 133, 133, 139, 133, 133, 139, 144, 139, 144, 144, 150, 144, 144, 150, 156, 150, 156, 150, 156, 156, 156, 156, 161, 156, 161, 167, 161, 167, 161, 161, 167, 173, 167, 173, 173, 167, 173, 167, 167, 173, 178, 173, 173, 178, 173, 178, 178, 184, 184, 184, 178, 184, 178, 178, 184, 190, 184, 190, 184, 184, 190, 190, 184, 190, 184, 190, 184, 184, 184, 184, 184, 190, 184, 184, 184, 178, 184, 178, 184, 178, 173, 178, 173, 178, 178, 178, 173, 173, 167, 173, 167, 167, 167, 167, 161, 161, 156, 156, 150, 156, 150, 150, 144, 150, 144, 144, 139, 144, 144, 139, 133, 128, 133, 128, 128, 128, 128, 116, 122, 122, 116, 111, 111, 105, 105, 99, 94, 88, 88, 94, 88, 82, 77, 77, 71, 71, 65, 65, 60, 60, 54, 48, 54, 54, 48, 43, 37, 43, 37, 31, 26, 31, 26, 20, 20, 15, 20, 15, 9, 15, 9, 3, 9, 3, 0, 3, 0, 3, 0, 0, 0, 3, 0, 3, 0, 0, 3, 3, 3, 9, 9, 9, 9, 9, 20, 15, 20, 20, 26, 26, 31, 31, 37, 43, 43, 48, 54, 54, 60, 65, 71, 77, 82, 82, 88, 99, 105, 105, 111, 116, 128, 128, 133, 144, 144, 150, 156, 161, 173, 178, 184, 195, 201, 207, 212, 218, 229, 235, 240, 246, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 246, 240, 229, 218, 201, 190, 178, 167, 150, 139, 128, 122, 111, 94, 82, 77, 60, 48, 37, 26, 20, 9, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 15, 26, 31, 43, 54, 65, 77, 88, 94, 99, 99, 94, 94, 77, 65, 54, 43, 43, 43, 48, 48, 60, 65, 71, 77, 82, 88, 94, 99, 105, 111, 116, 122, 128, 128, 133, 139, 144, 144, 150, 150, 156, 150, 150, 144, 139, 133, 128, 128, 116, 105, 94, 77, 60, 37, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 3, 9, 9, 15, 20, 26, 26, 31, 31, 43, 43, 48, 54, 60, 60, 65, 71, 77, 82, 88, 94, 99, 111, 116, 122, 128, 128, 139, 144, 156, 161, 167, 178, 190, 190, 201, 207, 212, 218, 229, 235, 246, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 252, 240, 229, 224, 212, 195, 190, 178, 161, 156, 144, 128, 122, 111, 99, 88, 77, 65, 54, 43, 37, 31, 20, 20, 15, 3, 9, 3, 3, 3, 9, 9, 15, 20, 31, 31, 37, 48, 60, 71, 82, 94, 105, 111, 116, 122, 128, 122, 122, 111, 99, 88, 77, 71, 71, 77, 88, 94, 94, 105, 111, 122, 122, 128, 128, 133, 144, 156, 156, 167, 167, 173, 173, 184, 184, 184, 190, 184, 190, 190, 184, 173, 173, 161, 156, 144, 133, 128, 111, 94, 77, 54, 37, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 9, 9, 9, 3, 3, 9, 9, 9, 9, 15, 9, 9, 15, 15, 15, 20, 15, 20, 26, 20, 26, 26, 31, 31, 43, 48, 60, 60, 65, 65, 77, 77, 88, 88, 94, 99, 105, 105, 116, 122, 122, 128, 128, 133, 144, 150, 156, 161, 173, 173, 184, 184, 195, 201, 207, 212, 224, 235, 235, 240, 246, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 246, 240, 229, 218, 212, 195, 190, 178, 161, 156, 144, 133, 122, 116, 105, 94, 77, 60, 54, 43, 31, 20, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 20, 37, 48, 60, 71, 88, 88, 94, 94, 94, 82, 77, 65, 60, 54, 65, 71, 77, 82, 94, 105, 111, 122, 128, 139, 144, 156, 161, 173, 184, 195, 201, 212, 218, 229, 235, 246, 246, 252, 255, 255, 255, 255, 255, 255, 252, 240, 240, 224, 218, 201, 184, 161, 150, 128, 111, 88, 71, 54, 31, 20, 15, 20, 31, 54, 71, 94, 116, 128, 133, 128, 133, 133, 128, 128};
