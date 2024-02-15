#include "SysIO.h"
#include "SysArr.h"

void Resize_Impl(size_t size, array_int * this) {
    static array_int * _this; 
    if(this != NULL) {
        _this = this; 
    }
    if(_this == NULL) {
        return; 
    }
    _this->array = (int*)realloc(_this->array, size); 
}

void Resize(size_t size) {
    Resize_Impl(size, NULL); 
}


int main(void) {
    _print_f("Hello World!\n"); 
    array_int arr; 
    init_array_int(&arr, 10); 
    arr.SetValue(0, 10);

    _write(arr.GetValue(0)); 
    return 0; 
}