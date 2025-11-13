#ifndef LOADFROMFILEBLOCK_H
#define LOADFROMFILEBLOCK_H

#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "QuakeSharedBuffers.h"

std::vector<uint8_t> loadBlockFromFileNoCache(int32_t blockNum, std::string filePath);

#endif