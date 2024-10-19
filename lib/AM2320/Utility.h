enum class ResponseBytes : int {
    FUNCTION_CODE = 0,
    NUMBER_OF_BYTES = 1,
    HUMIDITY_HIGH_BYTE = 2,
    HUMIDITY_LOW_BYTE = 3,
    TEMPERATURE_HIGH_BYTE = 4,
    TEMPERATURE_LOW_BYTE = 5,
    CRC_LOW_BYTE = 6,
    CRC_HIGH_BYTE = 7,
};

enum class Status : int {
    OK = 0,
    INITIALIZED = 1,
    INITIALIZE_FAILED = 2,
    SENSOR_NOT_READY = 3,
    READ_ERROR = 4,
    CRC_MISMATCH = 5
};