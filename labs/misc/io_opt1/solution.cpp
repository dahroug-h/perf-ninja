#include "solution.hpp"

#include <cstdio>
#include <fstream>
#include <stdexcept>

constexpr size_t BUFFER_SIZE = 1024 * 1024;

uint32_t solution(const char *file_name) {
  std::ifstream file_stream{file_name};
  if (!file_stream.is_open())
    throw std::runtime_error{"The file could not be opened"};

  // Initial value has all bits set to 1
  uint32_t crc = 0xff'ff'ff'ff;

  // Update the CRC32 value character by character
  char buffer[BUFFER_SIZE];
  while (true) {
    file_stream.read(buffer, BUFFER_SIZE);
    const size_t count = file_stream.gcount();
    for (size_t idx = 0; idx < count; idx++) {
      update_crc32(crc, static_cast<uint8_t>(buffer[idx]));
    }
    if (file_stream.eof())
      break;
  }

  // Invert the bits
  crc ^= 0xff'ff'ff'ff;

  return crc;
}
