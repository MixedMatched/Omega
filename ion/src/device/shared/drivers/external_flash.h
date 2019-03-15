#ifndef ION_DEVICE_SHARED_EXTERNAL_FLASH_H
#define ION_DEVICE_SHARED_EXTERNAL_FLASH_H

#include <stddef.h>
#include <stdint.h>

namespace Ion {
namespace Device {
namespace ExternalFlash {

void init();
void shutdown();

int NumberOfSectors();
int SectorAtAddress(uint32_t address);
void MassErase();
void EraseSector(int i);
void WriteMemory(uint8_t * source, uint8_t * destination, size_t length);

}
}
}

#endif