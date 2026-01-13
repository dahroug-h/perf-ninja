#include "solution.hpp"

#include <fstream>

#include "MappedFile.hpp"
#include <stdexcept

uint32_t solution(const char* file_name) {
  MappedFile file{file_name};

  // Initial value has all bits set to 1
  uint32_t crc = 0xff'ff'ff'ff;

  // Update the CRC32 value character by character
  char c;

  auto content = file.getContents();

  for (char c : content) {
    update_crc32(crc, static_cast<uint8_t>(c));
  }

  crc ^= 0xff'ff'ff'ff;

  return crc;
}