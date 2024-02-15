#ifndef SysArr_H
#define SysArr_H

#if !__cplusplus

#include <stdlib.h>

#define CONCAT_IMPL( x, y ) x##y
#define MACRO_CONCAT( x, y ) CONCAT_IMPL( x, y )
#define CREATE_FUNCTION_NAME(type,name) MACRO_CONCAT(type, MACRO_CONCAT(MACRO_CONCAT(_, MACRO_CONCAT(name, _)), MACRO_CONCAT(function_, __COUNTER__)))

#define Define_Array_Struct(type, type_name) \
        struct array_##type_name { \
            /* Member Functions */\
            type * array; \
            size_t size; \
            size_t capacity; \
            /* Function pointers;  */\
            type (*GetValue)(size_t); \
            void (*SetValue)(size_t, type); \
            void (*Resize)(size_t);\
        }; \
        typedef struct array_##type_name array_##type_name;


#define Define_Array_Default_Get_Function(type, name, type_name) \
        type  name(size_t index, array_##type_name * this) { \
            static array_##type_name * _this = NULL;  \
            if(this != NULL) { \
                _this = this;  \
            } \
            if(_this == NULL) { \
                return -1; \
            } \
            if(index < _this->size) { \
                return _this->array[index]; \
            } \
            return -1; \
        } 

#define Define_Array_Get_Function(type, name, type_name) \
        Define_Array_Default_Get_Function(type, MACRO_CONCAT(name, _def), type_name)\
        type  name(size_t index) {\
            return MACRO_CONCAT(name, _def)(index, NULL);\
        }

#define Define_Array_Default_Set_Function(type, name, type_name) \
        void name(size_t index, type value, array_##type_name * this) { \
            static array_##type_name * _this = NULL; \
            if(this != NULL) { \
                _this = this; \
            } \
            if(_this == NULL) { \
                return; \
            } \
            if(index < _this->size) { \
                _this->array[index] = value; \
            } \
        }

#define Define_Array_Set_Function(type, name, type_name) \
        Define_Array_Default_Set_Function(type, MACRO_CONCAT(name, _def), type_name) \
        void name(size_t index, type value) {\
            MACRO_CONCAT(name, _def)(index, value, NULL);\
        }

#define Define_Array_Resize_Function(type, name, type_name) \

#define INITIALIZER(type, initializer_name, set_name, get_name, type_name)\
        void initializer_name(array_##type_name * item, size_t size) {\
            item->array = (type*)malloc(sizeof(type) * size);\
            MACRO_CONCAT(get_name, _def)(0, item);\
            MACRO_CONCAT(set_name, _def)(0, 0, item);\
            item->GetValue = get_name;\
            item->SetValue = set_name;\
            item->size = size;\
            item->capacity = size; \
        }

#define CREATE_INITIALIZER(type, initializer_name, set_name, get_name, type_name) \
        Define_Array_Set_Function(type, set_name, type_name) \
        Define_Array_Get_Function(type, get_name, type_name) \
        INITIALIZER(type, initializer_name, set_name, get_name, type_name)\
        void (*init_array_##type_name)(array_##type_name * item, size_t size) = initializer_name; 

#define RECREATE_INITIALIZER(type, type_name) \
        CREATE_INITIALIZER(type, CREATE_FUNCTION_NAME(type_name, init), CREATE_FUNCTION_NAME(type, set), CREATE_FUNCTION_NAME(type, get), type_name)

#define CREATE_ARRAY_TYPE(type, type_name) \
        Define_Array_Struct(type, type_name) \
        CREATE_INITIALIZER(type, CREATE_FUNCTION_NAME(type_name, init), CREATE_FUNCTION_NAME(type, set), CREATE_FUNCTION_NAME(type, get), type_name)


#define CREATE_ARRAY_TYPES \
    CREATE_ARRAY_TYPE(int, int) \
    CREATE_ARRAY_TYPE(float, float) \
    CREATE_ARRAY_TYPE(double, double) \
    CREATE_ARRAY_TYPE(char, char) \
    CREATE_ARRAY_TYPE(short, short) \
    CREATE_ARRAY_TYPE(long, long) \
    CREATE_ARRAY_TYPE(long long, long_long) \
    CREATE_ARRAY_TYPE(unsigned int, uint) \
    CREATE_ARRAY_TYPE(unsigned char, uchar) \
    CREATE_ARRAY_TYPE(unsigned short, ushort) \
    CREATE_ARRAY_TYPE(unsigned long, ulong) \
    CREATE_ARRAY_TYPE(unsigned long long, ulong_long) \
    CREATE_ARRAY_TYPE(size_t, size_t) \


//CREATE_ARRAY_TYPES

CREATE_ARRAY_TYPE(int, int);



#endif // !__cplusplus
#endif // !SysArr_H

