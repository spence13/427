#include "soundshoot.h"
#include <stdint.h>

uint16_t soundshoot_sample_rate = 11025;
uint16_t soundshoot_num_samples = 4080;
uint8_t soundshoot[] = {125, 125, 129, 125, 129, 128, 128, 126, 125, 125, 125, 129, 125, 125, 125, 125, 129, 129, 125, 129, 125, 129, 125, 125, 129, 129, 125, 129, 125, 130, 125, 125, 125, 125, 130, 125, 125, 125, 125, 129, 125, 125, 130, 126, 126, 126, 125, 125, 125, 125, 129, 129, 125, 129, 125, 125, 130, 125, 125, 125, 125, 129, 130, 126, 126, 126, 126, 126, 125, 125, 125, 125, 129, 128, 128, 128, 126, 126, 126, 126, 125, 125, 125, 130, 126, 126, 126, 125, 125, 125, 125, 129, 125, 129, 129, 128, 128, 128, 128, 128, 123, 128, 123, 128, 128, 128, 123, 128, 126, 122, 126, 122, 123, 123, 123, 128, 129, 122, 126, 126, 126, 125, 125, 125, 123, 130, 129, 129, 125, 125, 125, 129, 125, 129, 125, 123, 130, 129, 125, 129, 125, 125, 125, 129, 125, 125, 135, 129, 125, 129, 126, 125, 125, 125, 125, 126, 130, 129, 130, 126, 126, 126, 125, 125, 125, 125, 129, 129, 125, 125, 125, 125, 129, 125, 125, 129, 123, 130, 130, 126, 126, 126, 126, 125, 125, 125, 125, 132, 126, 126, 126, 126, 125, 125, 125, 125, 129, 130, 126, 126, 126, 126, 125, 125, 125, 125, 130, 130, 126, 126, 126, 125, 125, 125, 125, 125, 135, 130, 126, 126, 126, 125, 125, 125, 125, 123, 135, 128, 128, 128, 128, 128, 123, 125, 125, 123, 133, 128, 123, 128, 123, 125, 125, 125, 126, 133, 128, 128, 128, 123, 128, 123, 125, 125, 123, 136, 128, 128, 128, 123, 125, 126, 120, 126, 136, 126, 126, 126, 126, 126, 125, 125, 125, 125, 132, 125, 125, 128, 128, 125, 128, 123, 120, 137, 128, 128, 128, 123, 125, 125, 125, 125, 135, 129, 125, 125, 125, 125, 125, 120, 130, 129, 135, 130, 123, 128, 128, 119, 125, 125, 123, 136, 129, 123, 123, 123, 123, 128, 123, 123, 136, 128, 128, 128, 126, 119, 128, 123, 123, 136, 128, 128, 128, 123, 125, 125, 125, 120, 137, 128, 128, 129, 125, 123, 123, 123, 120, 137, 128, 128, 128, 123, 123, 125, 125, 126, 136, 126, 126, 126, 125, 125, 125, 123, 130, 132, 128, 128, 128, 123, 125, 125, 125, 137, 129, 128, 128, 123, 123, 125, 123, 119, 142, 129, 128, 125, 128, 123, 125, 126, 122, 142, 129, 126, 125, 125, 125, 122, 126, 137, 129, 128, 128, 128, 123, 125, 125, 119, 142, 129, 128, 128, 123, 125, 122, 126, 136, 132, 128, 128, 128, 123, 125, 125, 118, 143, 129, 125, 128, 123, 125, 122, 126, 136, 132, 128, 128, 128, 123, 125, 125, 120, 142, 125, 125, 128, 123, 125, 122, 123, 143, 129, 128, 128, 123, 122, 125, 125, 132, 136, 129, 123, 128, 119, 125, 125, 120, 140, 129, 122, 128, 116, 128, 122, 118, 144, 130, 120, 126, 118, 125, 115, 118, 144, 129, 123, 122, 116, 119, 115, 128, 144, 132, 125, 125, 125, 125, 120, 128, 140, 136, 125, 125, 125, 125, 120, 126, 137, 136, 128, 128, 123, 120, 120, 128, 137, 135, 129, 126, 125, 125, 122, 123, 137, 137, 126, 126, 126, 120, 120, 128, 137, 140, 129, 125, 125, 119, 120, 122, 139, 132, 125, 129, 120, 125, 122, 122, 150, 129, 128, 129, 119, 125, 120, 120, 153, 129, 122, 129, 119, 125, 123, 119, 149, 132, 118, 129, 119, 125, 122, 126, 142, 129, 123, 123, 118, 120, 128, 144, 136, 123, 128, 119, 125, 119, 122, 153, 129, 122, 129, 116, 125, 125, 120, 149, 132, 120, 122, 119, 120, 122, 137, 139, 126, 126, 116, 128, 116, 116, 153, 132, 120, 123, 116, 125, 123, 129, 142, 128, 122, 118, 123, 116, 116, 153, 132, 120, 126, 115, 122, 123, 126, 146, 128, 122, 118, 122, 113, 122, 154, 132, 120, 123, 116, 123, 125, 132, 140, 126, 126, 116, 125, 118, 118, 154, 132, 118, 125, 119, 119, 123, 142, 139, 126, 123, 119, 125, 119, 122, 153, 129, 122, 123, 126, 115, 122, 154, 132, 120, 125, 119, 119, 126, 143, 137, 125, 122, 115, 122, 125, 122, 146, 132, 126, 115, 123, 113, 119, 154, 132, 119, 118, 123, 115, 122, 154, 132, 120, 122, 119, 119, 125, 147, 135, 120, 123, 118, 118, 122, 137, 144, 126, 122, 113, 120, 120, 130, 144, 126, 123, 116, 125, 125, 120, 152, 128, 122, 118, 123, 120, 120, 153, 129, 123, 122, 123, 118, 118, 154, 132, 120, 119, 125, 118, 116, 159, 132, 120, 119, 126, 116, 116, 159, 129, 123, 119, 126, 116, 116, 159, 129, 123, 119, 125, 119, 122, 157, 128, 122, 118, 123, 120, 120, 153, 132, 116, 116, 125, 120, 128, 152, 128, 123, 118, 115, 122, 132, 147, 128, 118, 119, 116, 128, 142, 139, 126, 118, 123, 111, 123, 156, 137, 125, 115, 125, 109, 120, 159, 132, 122, 118, 123, 118, 119, 157, 130, 116, 116, 119, 125, 132, 150, 122, 118, 122, 112, 126, 152, 136, 125, 113, 123, 108, 119, 163, 129, 122, 112, 120, 116, 122, 154, 123, 118, 115, 109, 125, 139, 136, 120, 112, 126, 109, 122, 166, 132, 120, 118, 120, 120, 126, 153, 128, 116, 126, 109, 128, 154, 133, 126, 116, 125, 116, 116, 160, 130, 118, 119, 116, 129, 143, 144, 125, 112, 129, 106, 118, 166, 130, 116, 122, 113, 123, 139, 146, 126, 115, 126, 106, 120, 164, 130, 116, 118, 113, 128, 142, 144, 125, 111, 128, 112, 116, 164, 129, 115, 125, 111, 126, 150, 139, 122, 112, 125, 118, 120, 159, 128, 111, 128, 106, 123, 159, 132, 119, 112, 119, 128, 132, 150, 125, 111, 129, 109, 118, 164, 132, 111, 125, 108, 128, 159, 132, 122, 115, 118, 123, 130, 149, 125, 111, 129, 108, 119, 163, 126, 109, 128, 102, 118, 164, 133, 116, 120, 109, 126, 146, 139, 120, 111, 123, 119, 125, 154, 126, 109, 129, 111, 115, 163, 126, 109, 128, 102, 122, 160, 132, 113, 120, 106, 126, 153, 133, 119, 116, 113, 126, 146, 139, 120, 111, 120, 123, 130, 150, 126, 106, 128, 116, 120, 157, 125, 111, 129, 112, 119, 159, 128, 111, 129, 108, 116, 164, 130, 111, 129, 101, 116, 164, 130, 109, 128, 105, 119, 163, 130, 109, 129, 101, 120, 160, 130, 115, 128, 102, 120, 160, 132, 111, 129, 105, 115, 166, 136, 111, 130, 103, 118, 166, 132, 113, 130, 99, 122, 166, 130, 116, 113, 113, 102, 119, 176, 153, 132, 139, 137, 139, 140, 140, 144, 139, 133, 123, 106, 96, 77, 70, 79, 91, 108, 133, 144, 147, 150, 144, 142, 129, 128, 128, 128, 128, 128, 126, 125, 125, 125, 126, 126, 126, 130, 126, 128, 132, 128, 130, 125, 126, 128, 132, 128, 129, 133, 128, 129, 133, 136, 132, 139, 137, 139, 140, 143, 140, 139, 133, 120, 106, 89, 70, 65, 82, 99, 126, 150, 156, 152, 152, 139, 130, 129, 125, 128, 128, 128, 126, 126, 126, 128, 130, 126, 126, 132, 126, 128, 132, 128, 128, 133, 128, 132, 136, 139, 139, 142, 142, 143, 139, 137, 129, 120, 106, 89, 71, 61, 78, 102, 135, 157, 159, 154, 143, 135, 126, 125, 125, 125, 129, 125, 126, 126, 126, 123, 125, 129, 129, 123, 128, 128, 129, 132, 130, 139, 140, 143, 142, 140, 132, 123, 115, 95, 75, 63, 65, 91, 125, 163, 163, 159, 150, 135, 128, 130, 130, 125, 128, 126, 128, 132, 128, 129, 128, 132, 135, 130, 137, 136, 137, 139, 140, 140, 139, 130, 122, 109, 88, 72, 58, 74, 102, 146, 170, 163, 156, 137, 130, 125, 126, 130, 130, 126, 128, 129, 133, 130, 130, 135, 132, 140, 142, 142, 140, 140, 135, 129, 120, 102, 84, 67, 60, 79, 118, 161, 173, 163, 150, 132, 126, 130, 126, 128, 128, 128, 132, 135, 130, 137, 136, 139, 140, 140, 142, 140, 132, 126, 111, 95, 72, 57, 67, 95, 142, 174, 167, 154, 139, 130, 126, 128, 132, 132, 129, 130, 133, 130, 139, 140, 137, 140, 140, 139, 133, 119, 105, 87, 65, 55, 71, 113, 160, 176, 167, 146, 126, 130, 130, 125, 130, 133, 129, 133, 139, 137, 140, 140, 142, 136, 132, 123, 106, 92, 65, 57, 68, 106, 156, 183, 168, 147, 128, 126, 130, 130, 132, 135, 132, 140, 142, 139, 142, 142, 136, 136, 119, 108, 85, 67, 54, 70, 115, 167, 181, 167, 140, 128, 132, 129, 130, 137, 136, 135, 140, 143, 139, 140, 136, 130, 118, 101, 81, 60, 53, 75, 132, 183, 178, 161, 137, 128, 132, 135, 137, 136, 139, 140, 142, 139, 139, 135, 129, 112, 94, 77, 55, 61, 87, 149, 185, 176, 153, 133, 130, 135, 137, 136, 139, 142, 142, 143, 137, 135, 122, 111, 91, 72, 54, 58, 101, 160, 188, 177, 150, 130, 135, 137, 135, 142, 142, 139, 139, 142, 132, 123, 115, 96, 77, 53, 58, 96, 161, 192, 176, 150, 136, 135, 136, 140, 139, 144, 140, 139, 139, 129, 116, 102, 79, 64, 54, 74, 139, 185, 185, 161, 142, 139, 137, 136, 139, 144, 140, 142, 137, 128, 115, 98, 77, 55, 57, 84, 153, 197, 184, 153, 136, 136, 140, 139, 142, 142, 143, 137, 132, 115, 99, 81, 55, 55, 75, 144, 195, 188, 157, 135, 137, 139, 140, 143, 140, 139, 133, 125, 112, 89, 75, 50, 55, 98, 163, 200, 180, 144, 135, 136, 137, 140, 142, 136, 133, 125, 106, 92, 70, 51, 54, 84, 157, 205, 183, 154, 139, 140, 142, 144, 142, 140, 132, 125, 111, 92, 70, 51, 57, 111, 180, 201, 176, 147, 137, 140, 146, 142, 143, 139, 129, 116, 98, 72, 53, 58, 84, 159, 205, 184, 157, 140, 142, 143, 139, 140, 132, 128, 112, 96, 74, 54, 54, 89, 163, 208, 184, 152, 140, 143, 143, 140, 139, 132, 120, 108, 85, 63, 50, 61, 128, 198, 202, 171, 147, 143, 143, 137, 137, 136, 123, 111, 92, 70, 51, 57, 109, 184, 211, 177, 146, 147, 143, 140, 137, 128, 123, 111, 89, 71, 51, 57, 116, 194, 208, 174, 146, 142, 137, 137, 137, 130, 118, 101, 79, 60, 54, 70, 152, 212, 198, 160, 143, 143, 139, 139, 132, 120, 105, 87, 65, 50, 60, 128, 202, 209, 168, 143, 139, 139, 139, 132, 118, 112, 88, 65, 48, 60, 122, 198, 214, 171, 147, 143, 140, 136, 130, 118, 102, 84, 63, 51, 63, 144, 216, 204, 164, 142, 142, 143, 137, 125, 116, 95, 70, 54, 55, 89, 178, 224, 188, 154, 144, 144, 140, 130, 120, 102, 75, 60, 53, 64, 156, 221, 202, 160, 139, 142, 140, 129, 120, 105, 79, 63, 51, 64, 149, 221, 204, 161, 146, 140, 136, 130, 118, 106, 78, 55, 51, 68, 156, 229, 197, 160, 142, 137, 139, 129, 116, 96, 72, 54, 54, 89, 187, 229, 184, 150, 140, 140, 133, 123, 108, 79, 60, 51, 64, 146, 224, 209, 164, 143, 143, 135, 125, 112, 94, 71, 51, 57, 115, 204, 226, 174, 143, 140, 140, 129, 112, 95, 71, 53, 57, 103, 195, 232, 181, 144, 144, 139, 126, 113, 94, 71, 51, 55, 108, 204, 232, 176, 146, 143, 133, 125, 112, 91, 67, 48, 60, 132, 218, 224, 168, 143, 139, 133, 120, 106, 79, 58, 55, 72, 168, 239, 198, 156, 140, 136, 126, 115, 91, 68, 50, 60, 120, 212, 229, 173, 143, 143, 132, 118, 99, 72, 57, 53, 87, 187, 240, 190, 143, 142, 132, 116, 102, 81, 60, 57, 70, 170, 240, 197, 150, 140, 133, 118, 102, 77, 55, 50, 68, 167, 238, 197, 144, 136, 129, 113, 101, 70, 57, 53, 72, 180, 249, 197, 149, 143, 133, 119, 101, 71, 57, 60, 92, 200, 239, 188, 144, 139, 129, 112, 91, 63, 50, 63, 129, 228, 225, 168, 144, 135, 119, 109, 78, 55, 55, 72, 178, 249, 195, 152, 140, 126, 115, 94, 63, 48, 60, 132, 232, 224, 164, 137, 132, 118, 98, 74, 54, 54, 99, 202, 240, 177, 144, 133, 119, 102, 77, 63, 54, 77, 187, 250, 190, 143, 137, 118, 106, 81, 60, 53, 71, 173, 252, 197, 143, 137, 123, 109, 81, 60, 54, 67, 176, 255, 200, 144, 133, 118, 106, 81, 57, 53, 75, 187, 249, 190, 147, 132, 118, 99, 72, 54, 58, 92, 202, 248, 177, 140, 129, 112, 95, 64, 50, 58, 125, 229, 231, 163, 133, 126, 111, 79, 61, 50, 68, 170, 249, 198, 146, 130, 116, 96, 67, 53, 58, 103, 219, 243, 164, 136, 128, 105, 84, 61, 50, 67, 174, 252, 198, 147, 126, 113, 94, 67, 51, 58, 130, 236, 229, 161, 133, 119, 99, 72, 60, 58, 92, 207, 249, 177, 137, 126, 108, 79, 58, 50, 72, 185, 253, 190, 139, 123, 105, 82, 57, 50, 67, 173, 253, 197, 146, 128, 112, 85, 58, 50, 65, 168, 255, 202, 144, 126, 108, 85, 57, 50, 67, 171, 255, 198, 144, 125, 108, 79, 60, 53, 68, 181, 255, 195, 142, 122, 102, 71, 55, 54, 79, 202, 255, 180, 136, 120, 96, 67, 50, 58, 112, 226, 242, 166, 126, 116, 88, 63, 54, 61, 153, 255, 214, 150, 125, 106, 78, 60, 54, 77, 197, 255, 183, 132, 118, 95, 67, 57, 57, 130, 246, 229, 154, 126, 109, 82, 58, 53, 74, 192, 255, 187, 136, 118, 94, 67, 51, 57, 140, 245, 225, 153, 122, 108, 71, 58, 57, 94, 218, 255, 171, 126, 109, 85, 63, 51, 64, 181, 255, 194, 136, 119, 91, 63, 54, 60, 152, 255, 215, 144, 119, 98, 65, 55, 55, 128, 242, 231, 154, 122, 101, 70, 51, 55, 108, 228, 243, 156, 126, 105, 68, 55, 51, 99, 221, 248, 159, 122, 106, 65, 55, 50, 94, 224, 246, 159, 118, 101, 65, 48, 46, 96, 224, 246, 156, 119, 103, 68, 58, 54, 112, 240, 239, 154, 119, 94, 64, 54, 58, 137, 255, 225, 144, 119, 89, 58, 51, 61, 164, 255, 207, 136, 113, 81, 60, 53, 68, 195, 255, 178, 123, 103, 72, 58, 54, 99, 229, 253, 159, 115, 92, 60, 51, 55, 150, 255, 216, 136, 112, 84, 58, 51, 72, 195, 255, 176, 119, 102, 68, 55, 58, 123, 249, 233, 143, 111, 89, 57, 48, 67, 195, 255, 181, 122, 102, 64, 51, 55, 128, 253, 228, 143, 112, 79, 53, 51, 74, 208, 255, 171, 115, 98, 58, 51, 58, 156, 255, 202, 128, 105, 65, 55, 55, 108, 240, 243, 146, 109, 78, 53, 54, 71, 211, 255, 164, 115, 87, 57, 47, 61, 177, 255, 191, 120, 91, 61, 53, 54, 147, 255, 208, 129, 101, 65, 53, 53, 126, 250, 231, 139, 103, 74, 50, 50, 106, 238, 245, 147, 106, 74, 54, 51, 89, 228, 255, 152, 105, 75, 54, 51, 85, 226, 255, 154, 106, 75, 54, 53, 78, 224, 255, 159, 106, 79, 54, 51, 84, 222, 255, 156, 103, 74, 53, 50, 84, 232, 255, 146, 103, 72, 48, 48, 102, 240, 242, 144, 98, 65, 53, 51, 123, 255, 226, 135, 96, 61, 48, 50, 153, 255, 205, 122, 88, 53, 47, 57, 187, 255, 180, 113, 81, 50, 50, 71, 216, 255, 159, 101, 67, 51, 48, 108, 252, 242, 140, 95, 58, 51, 55, 160, 255, 200, 118, 84, 53, 48, 67, 214, 255, 164, 105, 65, 51, 50, 116, 255, 233, 133, 87, 54, 48, 57, 185, 255, 184, 106, 74, 50, 51, 96, 243, 250, 144, 92, 63, 48, 54, 168, 255, 191, 113, 77, 54, 47, 92, 243, 252, 146, 91, 55, 47, 55, 177, 255, 188, 111, 71, 55, 44, 106, 253, 243, 136, 84, 57, 47, 64, 201, 255, 167, 101, 63, 47, 48, 146, 255, 212, 118, 75, 50, 48, 91, 242, 255, 142, 82, 55, 46, 63, 200, 255, 170, 92, 60, 47, 47, 160, 255, 201, 106, 67, 47, 43, 112, 255, 232, 120, 72, 50, 41, 79, 233, 255, 137, 78, 51, 34, 65, 208, 255, 149, 77, 46, 39, 57, 195, 255, 173, 92, 58, 50, 50, 170, 255, 192, 98, 61, 48, 50, 154, 255, 205, 108, 63, 53, 47, 135, 255, 219, 112, 67, 47, 46, 128, 255, 229, 116, 68, 50, 43, 118, 255, 231, 119, 64, 48, 47, 120, 255, 236, 115, 63, 48, 39, 122, 255, 229, 113, 65, 51, 44, 126, 255, 224, 112, 61, 50, 44, 139, 255, 216, 101, 60, 47, 47, 156, 255, 201, 95, 57, 46, 50, 177, 255, 184, 84, 53, 43, 55, 200, 255, 164, 75, 54, 41, 67, 221, 255, 144, 70, 51, 37, 95, 243, 255, 122, 61, 48, 41, 125, 255, 222, 101, 60, 47, 46, 166, 255, 190, 85, 53, 43, 60, 207, 255, 156, 70, 54, 41, 88, 242, 255, 120, 57, 53, 40, 136, 255, 214, 94, 50, 46, 51, 191, 255, 163, 68, 53, 40, 82, 240, 255, 120, 55, 48, 40, 140, 255, 202, 88, 51, 39, 63, 207, 255, 152, 64, 54, 41, 106, 255, 235, 101, 54, 46, 50, 180, 255, 176, 71, 51, 46, 89, 245, 252, 111, 58, 48, 47, 168, 255, 177, 72, 54, 40, 81, 236, 255, 113, 54, 46, 48, 174, 255, 176, 65, 54, 41, 94, 246, 248, 102, 53, 46, 51, 187, 255, 164, 61, 54, 43, 116, 255, 222, 92, 50, 43, 68, 212, 255, 129, 58, 54, 46, 152, 255, 190, 70, 51, 44, 91, 243, 243, 101, 50, 48, 58, 201, 255, 142, 55, 53, 46, 150, 255, 191, 75, 60, 46, 98, 245, 238, 98, 51, 47, 64, 215, 255, 122, 55, 55, 53, 177, 255, 163, 63, 57, 47, 133, 255, 200, 78, 54, 44, 96, 243, 232, 96, 55, 51, 75, 214, 255, 115, 55, 53, 58, 191, 255, 143, 57, 54, 55, 164, 255, 167, 63, 57, 51, 139, 255, 183, 72, 55, 51, 118, 250, 205, 81, 58, 53, 98, 236, 221, 88, 58, 53, 82, 225, 232, 99, 58, 60, 81, 216, 245, 103, 57, 58, 77, 211, 243, 109, 60, 64, 78, 207, 242, 112, 65, 65, 78, 204, 242, 118, 67, 68, 79, 207, 239, 120, 71, 70, 79, 204, 235, 112, 70, 71, 82, 208, 219, 105, 55, 58, 82, 174, 255, 255, 200, 161, 139, 96, 53, 6, 0, 13, 46, 87, 119, 120, 113, 111, 116, 128, 126, 129, 130, 136, 146, 150, 147, 143, 142, 140, 135, 128, 128, 133, 137, 129, 132, 130, 140, 140, 142, 139, 140, 152, 153, 153, 160, 167, 177, 176, 173, 171, 170, 156, 136, 113, 105, 105, 84, 67, 58, 53, 53, 46, 50, 68, 89, 112, 118, 112, 116, 129, 136, 133, 140, 147, 160, 163, 152, 143, 137, 144, 139, 130, 128, 132, 143, 143, 139, 142, 156, 159, 159, 161, 168, 183, 183, 173, 166, 156, 146, 119, 91, 75, 60, 55, 44, 31, 26, 46, 74, 92, 106, 118, 136, 135, 136, 139, 149, 168, 163, 156, 153, 149, 143, 129, 126, 137, 150, 149, 144, 144, 166, 177, 174, 173, 178, 184, 180, 149, 126, 96, 82, 70, 40, 24, 15, 34, 44, 53, 91, 109, 128, 150, 147, 150, 153, 160, 174, 176, 159, 142, 132, 144, 147, 139, 139, 161, 174, 168, 171, 173, 191, 188, 168, 146, 106, 81, 71, 50, 23, 13, 5, 23, 68, 96, 109, 130, 156, 159, 159, 154, 170, 188, 168, 149, 133, 146, 160, 152, 147, 160, 178, 187, 178, 171, 183, 171, 128, 85, 55, 48, 29, 7, 0, 17, 60, 84, 108, 142, 168, 163, 156, 171, 191, 185, 161, 136, 146, 170, 156, 156, 164, 185, 194, 176, 173, 149, 137, 120, 58, 22, 3, 0, 16, 30, 44, 85, 128, 170, 171, 168, 167, 181, 201, 180, 154, 142, 170, 181, 168, 168, 177, 202, 185, 156, 125, 105, 78, 24, 0, 0, 0, 36, 60, 96, 132, 157, 185, 180, 174, 194, 204, 180, 152, 144, 166, 197, 183, 170, 184, 204, 166, 115, 71, 19, 6, 3, 0, 0, 34, 103, 140, 153, 160, 202, 214, 190, 184, 159, 173, 194, 177, 167, 181, 197, 198, 159, 108, 88, 64, 3, 0, 0, 0, 60, 91, 132, 149, 174, 218, 205, 185, 181, 195, 187, 170, 166, 198, 211, 180, 152, 108, 77, 55, 0, 0, 0, 2, 55, 87, 136, 168, 208, 218, 195, 177, 192, 201, 176, 157, 178, 214, 200, 152, 115, 54, 16, 0, 0, 0, 0, 68, 113, 154, 171, 212, 240, 211, 202, 171, 183, 216, 187, 168, 163, 174, 164, 89, 19, 0, 0, 0, 0, 20, 109, 204, 207, 200, 205, 229, 240, 191, 160, 174, 215, 202, 166, 133, 79, 63, 15, 0, 0, 0, 53, 105, 143, 188, 235, 249, 221, 195, 195, 218, 202, 177, 160, 167, 154, 75, 2, 0, 0, 3, 9, 48, 122, 204, 243, 225, 211, 221, 250, 214, 171, 173, 161, 177, 154, 67, 0, 0, 0, 0, 2, 54, 144, 221, 255, 233, 214, 221, 243, 224, 190, 173, 139, 128, 126, 27, 0, 0, 0, 6, 37, 112, 195, 255, 255, 233, 212, 221, 248, 200, 170, 136, 106, 103, 22, 0, 0, 0, 36, 71, 144, 209, 255, 255, 233, 207, 212, 240, 197, 154, 108, 78, 64, 0, 0, 0, 17, 81, 137, 180, 233, 255, 255, 219, 198, 216, 198, 140, 113, 101, 40, 0, 0, 0, 23, 63, 150, 225, 255, 255, 216, 232, 245, 214, 167, 115, 101, 91, 0, 0, 0, 10, 44, 112, 201, 255, 255, 255, 215, 204, 225, 178, 112, 105, 94, 0, 0, 0, 0, 75, 137, 208, 231, 255, 255, 238, 177, 200, 204, 130, 54, 22, 0, 0, 0, 0, 87, 219, 239, 228, 255, 255, 253, 181, 153, 115, 120, 88, 0, 0, 0, 10, 63, 137, 209, 255, 255, 255, 224, 163, 167, 208, 118, 24, 0, 0, 0, 0, 40, 140, 236, 255, 255, 248, 191, 192, 232, 149, 70, 0, 0, 0, 0, 0, 50, 207, 255, 255, 255, 242, 174, 192, 192, 96, 13, 0, 0, 0, 0, 39, 140, 255, 255, 255, 228, 225, 232, 170, 98, 31, 12, 0, 0, 0, 36, 226, 255, 255, 255, 243, 255, 215, 128, 58, 44, 55, 0, 0, 0, 33, 226, 255, 255, 248, 255, 255, 184, 115, 33, 31, 29, 0, 0, 0, 132, 255, 255, 255, 255, 255, 207, 123, 81, 85, 33, 0, 0, 0, 122, 211, 236, 255, 255, 255, 180, 119, 140, 112, 0, 0, 0, 20, 74, 147, 255, 255, 255, 209, 191, 205, 154, 40, 0, 0, 0, 0, 19, 198, 255, 255, 255, 194, 164, 208, 129, 17, 0, 0, 0, 50, 98, 236, 255, 255, 255, 222, 122, 116, 125, 44, 0, 0, 0, 119, 191, 255, 255, 255, 255, 177, 98, 96, 102, 0, 0, 0, 74, 136, 222, 255, 255, 255, 185, 103, 133, 112, 0, 0, 0, 54, 119, 222, 255, 255, 255, 209, 126, 47, 63, 75, 0, 0, 0, 115, 255, 255, 255, 229, 225, 214, 81, 0, 0, 0, 0, 0, 82, 255, 255, 255, 219, 159, 166, 136, 0, 0, 0, 7, 68, 146, 255, 255, 255, 209, 123, 77, 106, 9, 0, 0, 53, 139, 233, 255, 255, 255, 171, 75, 68, 77, 0, 0, 0, 144, 252, 255, 255, 255, 255, 128, 31, 16, 15, 0, 0, 0, 209, 255, 255, 236, 208, 201, 136, 10, 0, 0, 12, 51, 150, 255, 255, 255, 211, 128, 44, 60, 41, 0, 0, 60, 255, 255, 255, 255, 255, 167, 60, 0, 15, 0, 0, 37, 181, 255, 255, 255, 137, 161, 170, 20, 0, 0, 0, 190, 255, 255, 255, 255, 236, 89, 0, 0, 0, 0, 0, 118, 255, 255, 238, 120, 111, 167, 79, 0, 12, 95, 113, 191, 198, 184, 209, 150, 81, 34, 115, 163, 84, 43, 91, 236, 215, 106, 74, 133, 200, 122, 54, 105, 173, 143, 106, 64, 136, 205, 122, 68, 118, 195, 135, 79, 72, 166, 207, 125, 94, 47, 135, 209, 129, 75, 101, 184, 154, 84, 61, 152, 225, 140, 92, 55, 118, 204, 105, 31, 129, 214, 164, 103, 99, 129};
