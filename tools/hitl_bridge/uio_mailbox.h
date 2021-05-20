#include <stdbool.h>
#include <stdint.h>

void *UIOMB_InitDevice(const char *path, unsigned int size, unsigned int blocking_delay);
bool UIOMB_UIO_WaitForInterrupt(int timeout_ms);
bool UIOMB_MB_IsFull(void *instance);
bool UIOMB_MB_IsFullError(void *instance);
bool UIOMB_MB_IsEmpty(void *instance);
bool UIOMB_MB_IsEmptyError(void *instance);
bool UIOMB_MB_Write(void *instance, const unsigned int *buffer, unsigned int size, unsigned int blocking_delay);
uint32_t UIOMB_MB_Read(void *instance, unsigned int *buffer, unsigned int size);


void UIOMB_UIO_Write(void *instance, const unsigned int offset, unsigned int value);
unsigned int UIOMB_UIO_Read(void *instance, unsigned int offset);

