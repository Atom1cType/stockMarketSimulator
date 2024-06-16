# StockMarketSimulator

## Setup

 First, solution provided in repository and project can be easily launched after download. In case if project need to be regenerated, it can be done with help of CMake: 

`cmake -S "path to folder with CMakeLists.txt" -B "path to folder cmake_bin"`

Also it should help with build:

`cmake --build "path to folder cmake_bin"`

## Usage

After launch program will try to load data from json files and start process market.
> [!NOTE]
> In some cases if `data` folder is far enough and program can't reach it, few important ojects won't be initialized properly and there is no joy in such market, so try to move files from `data` folder closer to executable file.

After initial setup user will see repeatedly printing message with prices and stocks. To start enter input command we must pass EOF toconsole. <Ctrl+Z> -> Enter for Window, and hope <Ctrl+D> -> Enter for UNIX/Linux. In case of success `Enter command:` message will appear.
From this moment commands can be passed to program. If there will be a typo in command name you will have another chance to enter command, but if command ended or for some command after start invalid data will be passed, user input also will be interrupted and we need again pass EOF to console.

Available commands:
* `Exit` - exit from market.
* `Esc` - exit from command input mode.
* `Switch` - switch market strategy. (can be `random`+ or `trend`)
* `New` - add new portfolio.
* `Add` - add stock to current portfolio.
* `Remove` - remove stock from current portfolio.
* `Show` - print status for current portfolio.
* `Report` - print report about income for current portfolio.
* `SetActive` - change active portfolio.
* `Buy` - set request to buy stocks on market.
* `Sell` - set request to sell stocks on market.
