#include <Simply2D.h>
#include <level1.h>

int main()
{
	MM::Initialize();
	{
		Simply2D::ApplicationSpecifications specs;
		specs.renderer.name = "Simply2D Giorgos";
		specs.renderer.vSync = false;
		specs.renderer.borderless = false;
		specs.assets.assetsPath = ASSETS_PATH;
		specs.name = "GIORGOS";
		specs.version = "1.0";
		specs.identifier = "com.terrytoupi.simply2D.giorgos";

		// create application
		Simply2D::Application::Create(specs);

		// push scenes and make first scene active
		Simply2D::app().pushScene<Level1>("scene://Level1");
		Simply2D::app().activateScene<Level1>();

		// run the application
		Simply2D::app().Run();

		// shutdown systems
		Simply2D::Application::Destroy();
	}
	MM::Shutdown();
    return 0;
}