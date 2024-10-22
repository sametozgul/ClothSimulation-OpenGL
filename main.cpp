#include <memory>
#include "Window.h"

using namespace std;

int main()
{
    std::unique_ptr<Window>  win = make_unique<Window>();
    if(!win->init(3600,2000,"Hello World Example")){
        return -1;
    }
    win->mainLoop();
    win->cleanUp();
    return 0;
}
