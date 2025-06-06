#ifndef PLATFORM_DETECT_H
#define PLATFORM_DETECT_H

#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    #define PLATFORM_LINUX
#elif defined(__ARM_ARCH) || defined(__arm__) || defined(__AVR__) || defined(__STM32__)
    #define PLATFORM_MCU
#else
    #error "Unknown platform. Please define PLATFORM_LINUX or PLATFORM_MCU manually."
#endif

#endif // PLATFORM_DETECT_H
