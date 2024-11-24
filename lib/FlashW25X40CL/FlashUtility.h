enum class Instructions {
    WRITE_ENABLE = 0x06,
    WRITE_DISABLE = 0x04,
    READ_DATA = 0x03,
    SECTOR_ERASE = 0x20,
    BLOCK_ERASE_32 = 0x52,
    BLOCK_ERASE_64 = 0xD8,
    CHIP_ERASE = 0x60,
    PAGE_PROGRAM = 0x02
};