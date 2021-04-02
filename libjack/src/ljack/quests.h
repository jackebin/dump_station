#ifndef ljack_Quests_h
#define ljack_Quests_h

// current quiz data.

const int MSGN = 4;
const char * MSG[] = {

    "Aviable quizzes:\n",
    "1. Data Types",
    "2. Type Sizes (from stdint.h)",
    "3. Type Modes\n",
};

const int DATA_TYPE_VALS[] = {6, 100};
const char * DATA_TYPE_TITL = "QUIZ - DATA TYPES";
const char * DATA_TYPE_ANSW[] = {"char", "double", "enum", "int", "void", "float"};
const char * DATA_TYPE_QUIZ[] = {

    "Holds a singe 1-byte character.",
    "Holds a large floating-point number.",
    "Enumerated types. Converts to integers by symbolic names for sets.",
    "Stores a regular integer, defaulting to 32 bits in size.",
    "Indicates \"no type\". Data type empty.",
    "Holds a smaller floating-point number."
};

const int TYPE_SIZE_VALS[] = {5, 80};
const char * TYPE_SIZE_TITL = {"QUIZ - TYPE SIZES"};
const char * TYPE_SIZE_ANSW[] = {"int32_t", "uint8_t", "uint64_t", "int64_t", "int8_t"};
const char * TYPE_SIZE_QUIZ[] = {

    "32-bit signed integer.",
    "8-bit unsigned integer.",
    "64-bit unsigned integer.",
    "64-bit signed integer.",
    "8-bit signed integer."
};

const int TYPE_MODE_VALS[] = {4, 100};
const char * TYPE_MODE_TITL = {"QUIZ - TYPE MODES"};
const char * TYPE_MODE_ANSW[] = {"long", "unsigned", "signed", "short"};
const char * TYPE_MODE_QUIZ[] = {

    "Uses a larger storage for the type allowing it to hold bigger numbers.",
    "Modifies the type by giving it a larger bound but nothing under 0.",
    "Allowing negative and positive numbers by halving the upper bound.",
    "Uses smaller storage for the type but takes half the space."
};

#endif
