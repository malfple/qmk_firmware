OPT_DEFS += -DSTM32_DMA_REQUIRED=TRUE
OPT_DEFS += -DCORTEX_ENABLE_WFI_IDLE=TRUE

# Address of the bootloader in system memory
STM32_BOOTLOADER_ADDRESS = 0x1FFF0000
