#include <stdio.h>
#include <dlfcn.h>
#include "vgl.h"
#include "LoadShaders.h"
#include "drawcommands.hpp"
#include "instancing.hpp"

VermilionApplication *VermilionApplication::s_app;

int main(int argc, char *argv[])
{
    VermilionApplication *app = DrawCommandExample::Create();
    //VermilionApplication *app = instancing::Create();

    app->Initialize("1");
    app->MainLoop();
    app->Finalize();

    return 0;
}
