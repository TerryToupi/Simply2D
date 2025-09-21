#include <app.h>
#include <sampleApp.h>

int main()
{
    core::ApplicationSpecifications specs;
    specs.renderer.name = "Simply2D Giorgos";
    specs.renderer.vSync = false;
    specs.renderer.borderless = false;
    specs.assets.assetsPath = ASSETS_PATH;
    specs.name = "GIORGOS";
    specs.version = "1.0";
    specs.identifier = "com.terrytoupi.simply2D.giorgos";

    core::app().create(specs);
    core::app().pushLayer<SampleApp>();
    core::app().run();
    return 0;
}