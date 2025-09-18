#include <app.h>

int main()
{
    core::ApplicationSpecifications specs;
    specs.name = "GIORGOS";
    specs.window.name = "GIORGOS WINDOW";

    core::Application app(specs);
    app.run();
    return 0;
}