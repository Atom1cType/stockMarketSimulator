#include "../Subject/Subject.h"
#include "../Utils/Utils.h"
#include "InputController.h"
#include "MainController.h"
#include <algorithm>
#include <iostream>

void InputController::Tick()
{
    BaseController::Tick();

    if (std::cin.get() == EOF)
    {
        std::cin.clear();

        std::cout << "Enter command: " << std::endl;

        Subject& subject = MainController::Instance().GetSubject();
        subject.Notify(NotifyEventType::EnterCommand);

        HandleInput();
    }
}

void InputController::HandleInput()
{
    std::string inputString;
    std::getline(std::cin, inputString);
    std::cin.clear();

    inputString = Utils::StringToLower(inputString);
    
    if (!TryToNotify(inputString))
    {
        HandleInput();
    }
}

bool InputController::TryToNotify(std::string& inputString)
{
    NotifyEventType notifyEventType = Utils::GetNotifyEventTypeByString(inputString);

    if (notifyEventType == NotifyEventType::Undefined)
    {
        std::cout << "Undefined command" << std::endl;
        return false;
    }

    Subject& subject = MainController::Instance().GetSubject();
    subject.Notify(notifyEventType);

    return true;
}
