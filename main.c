#include "SysArr.h"
#include <unistd.h>
#include "SysIO.h"

int main() {
    array_int arr; 
    init_array_int(&arr, 10); 
    arr.SetValue(0, 10);
    _print_f("%d\n", arr.GetValue(0));
    return 0; 
}