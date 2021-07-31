#include <Core/Inc/Core.h>

//Preconedition: a non-negative value is passed int 'n'
//Postcondition: n! is returned as a result

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    LOG("Hello, I am %s.", "Wallace");
    Sleep(1000);
    LOG("%d second have passed and I am still fine.", 1);
    
    int n = 42;
    ASSERT(n == 7, "42 is not 7");
    
    return 0;
}

