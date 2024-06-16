cmake_minimum_required(VERSION 3.13)
project(StockMarketSimulator LANGUAGES CXX)

add_executable(StockMarketSimulator
    source/Controllers/BaseAsyncController.cpp
    source/Controllers/BaseAsyncController.h
    source/Controllers/BaseController.cpp
    source/Controllers/BaseController.h
    source/Controllers/InputController.cpp
    source/Controllers/InputController.h
    source/Controllers/MainController.cpp
    source/Controllers/MainController.h
    source/Controllers/PortfolioController.cpp
    source/Controllers/PortfolioController.h
    source/Controllers/StockSimulatorController.cpp
    source/Controllers/StockSimulatorController.h
    source/Data/MarketRequestData.h
    source/Data/NotifyEventsData.h
    source/Data/StockSimulatorData.h
    source/DataProviders/StockSimulatorDataProvider.cpp
    source/DataProviders/StockSimulatorDataProvider.h
    source/Facades/StrategyFacade.cpp
    source/Facades/StrategyFacade.h
    source/MarketRequests/BaseMarketRequest.cpp
    source/MarketRequests/BaseMarketRequest.h
    source/MarketRequests/BuyMarketRequest.cpp
    source/MarketRequests/BuyMarketRequest.h
    source/MarketRequests/SellMarketRequest.cpp
    source/MarketRequests/SellMarketRequest.h
    source/Observer/Observer.cpp
    source/Observer/Observer.h
    source/Strategies/BaseStrategy.h
    source/Strategies/RandomStrategy.cpp
    source/Strategies/RandomStrategy.h
    source/Strategies/TrendStrategy.cpp
    source/Strategies/TrendStrategy.h
    source/Subject/Subject.cpp
    source/Subject/Subject.h
    source/Utils/Utils.cpp
    source/Utils/Utils.h
    source/StockMarketSimulator.cpp
    nlohman/json.hpp
)

set_target_properties(StockMarketSimulator PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)