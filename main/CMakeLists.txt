idf_component_register(
    SRCS
    "main.c"
    "arduino_main.cpp"
    REQUIRES
    ArduinoCore-avr-1.8.7
    Arduino-FOC-2.2
    INCLUDE_DIRS "."
)
