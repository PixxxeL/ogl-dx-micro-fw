#include <windows.h>

class Timer
{
    
public:
    Timer();
    ~Timer();
    void start();
    DWORD get();
    void wait(DWORD count);
    void reset();

private:
    DWORD start_millis;

};
