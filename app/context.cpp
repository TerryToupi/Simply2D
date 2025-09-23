#include <Simly2D.h>
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

    Simply2D::app().create(specs);
    Simply2D::app().pushLayer<SampleApp>();
    Simply2D::app().run();
    return 0;
}