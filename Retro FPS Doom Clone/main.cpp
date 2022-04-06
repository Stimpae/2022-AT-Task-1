#include "Framework.h"
#include "Graphics.h"
#include "Renderer.h"
#include <memory>

int main() {

	// Instantiate the framework class
	std::shared_ptr<Framework> framework = std::shared_ptr<Framework>(new Framework());

	// Initialise the window and create it
	if (framework->Initialise())
	{
		// Instantiate the graphics class
		std::shared_ptr<Graphics> graphics = std::shared_ptr<Graphics>(new Graphics());
		graphics->CreateWindowDependentResources(framework->GetHWND());

		// Create the appropriate device resources
	
		// Instantiate the renderer. // constructor should handle things like creating the appropriate resources
		std::shared_ptr<Renderer> renderer = std::shared_ptr<Renderer>(new Renderer(graphics));
		
		// need to create the window dependent resources here

		// if the framework has initialised then run it and pass through references
		framework->Run(graphics,renderer);
	}

	return 0;
}