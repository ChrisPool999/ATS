#include <iostream>
#include <chrono>
#include <deque>
#include <thread>
#include <vector>

class Stock
{
private:
  std::chrono::steady_clock::time_point dataAge = std::chrono::steady_clock::now();
  std::string symbol = "";
  int marketCap = -1;
  float volumeInMils = -1;
  float lowToday = -1;
  float highToday = -1;
  float openPrice = -1;
  int price = -1;

  Stock(); // private, can only be called in StockWatcher (factory class)
  friend class StockWatcher;
};

class StockWatcber   // need to make this with as FEW race conditions as possible
{
private:
  std::deque<Stock> stockList;
  std::vector<std::thread> threadList;
public:
  void watchStock(std::string& symbol);
  void removeStock(std::string& symbol);
  void updateStockInfo(std::string& symbol, std::string& response); // gives me ability to immiedately request new info instead of raising priority to top
  void changeStockPrio(std::string& symbol, int prio);
  void makeTopPrio(std::string& symbol);
  void seeStockPrio() const;
};

// who is making the threads
// stockerWatcher should be all the same data, either shared or only 1 instance

void StockWatcber::updateStockInfo(std::string& symbol, std::string& response)
{
      size_t index = response.find("bid_price");
      while (response[index] - '0' < 0 || response[index] - '0' > 9)
      {
        index++;
      }
      std::string bid_price = "";
      while (response[index] - '0' >= 0 && response[index] - '0' <= 9)
      {
        bid_price += response[index];
        index++;
        if (response[index] == '.')
        {
          bid_price += '.';
          index++;
        }
      }
      std::cout << "The bid price is " << bid_price << "\n";
}