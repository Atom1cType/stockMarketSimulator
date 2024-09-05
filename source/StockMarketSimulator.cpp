#include "Controllers/MainController.h"
#include "iostream"

int main()
{
    MainController& MainControllerRef = MainController::Instance();

	MainControllerRef.Initialize();
	
	if (!MainControllerRef.Enable())
	{
		std::cout << "Failed to enable Main Controller.\n";

		return -1;
	}

	while (MainControllerRef)
	{
		MainControllerRef.Tick();
	}

	MainControllerRef.Reset();

	return 0;
}
