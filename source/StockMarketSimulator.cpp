// StockMarketSimulator.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "Controllers/MainController.h"
#include "iostream"

int main()
{
    MainController& MainControllerRef = MainController::Instance();

	MainControllerRef.Initialize();

	//{
	//	MainController test;
	//	test.Initialize();
	//}
	
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
