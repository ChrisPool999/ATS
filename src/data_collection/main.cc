#include <iostream>
#include <fstream>
#include <chrono>
#include <deque>
#include <thread>
#include <vector>
#include <filesystem>
#include <curl/curl.h>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer) 
{
  size_t totalSize = size * nmemb;
  buffer->append((char*)contents, totalSize);
  return totalSize;

  // std::filesystem::create_directories("./data/stock/year/month/day")
  // write stock price, market cap, volume, lowtoday, hightoday, openprice
}

void parseHtml(std::string& response)
{
  if (response.size() == 0)
  {
    return;
  }
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
  std::cout << "The bid price is " << bid_price << std::endl;
}

void add_handles(std::vector<CURL*>& handles, CURLM* multi, std::vector<std::string>& response)
{
  std::ifstream is("/home/breezy/Code/data/stock_list.txt");
  if (!is)
  {
    return;
  }

  std::string symbol;
  int index = 0;
  while (std::getline(is, symbol)) // add error handling, what if one stock doesnt work? probably just try
  {
    if (symbol.size() == 0)
    {
      continue;
    }
    CURL* curl = curl_easy_init();
    if (curl)
    {
      std::string URL = "https://robinhood.com/us/en/stocks/" + symbol + "/";
      curl_easy_setopt(curl, CURLOPT_URL, URL.c_str());
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response[index++]);      
      curl_multi_add_handle(multi, curl);
      handles.push_back(curl);
    }
  }
  is.close();
}

int main()
{ 
  CURLM* multi = curl_multi_init();
  std::vector<CURL*> handles; 
  std::vector<std::string> response(6);
  add_handles(handles, multi, response);
  
  int handles_running = -1;
  while (handles_running)
  {
    CURLMcode MCode = curl_multi_perform(multi, &handles_running);
    int numfds;

    if (MCode == CURLM_OK && handles_running)
    {
      MCode = curl_multi_wait(multi, NULL, 0, 1000, &numfds);
    }

    if (MCode != CURLM_OK)
    {
      std::cout << "error MCode" << std::endl;
    } 
  }

  for (auto& r : response)
  {
    if (r.size())
    {
      parseHtml(r);
    }
  }
  return 0;
}

// If you want to reuse an easy handle that was added to the multi handle for transfer, you must 
// first remove it from the multi stack and then re-add it again (possibly after having altered 
// some options at your own choice). 

// 2 tasks:
//  handle IO requests - get stock info, change stocks monitoring, change prio
//  monitor stocks

//  while true 
//    handleUserRequest(requestBuffer)
//    if (!requestBuffer)
//      multi interface (will write to file descriptor)
//      sendToDatabase (future implementation)  

// void watchStock(std::string& symbol);
// void removeStock(std::string& symbol);