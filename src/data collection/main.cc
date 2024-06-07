#include <iostream>
#include <curl/curl.h>
#include <chrono>
#include <deque>
#include "main.h"

// data collection / data analysis / orders

int main()
{
  // gonna need to have worker threads and a "designator thread"
}

// i want to be able to requst and get a stocks info as soon as possible
//    -  need to block the thread and return a value

// i want to be able to update the stock and not block, (this requires us to confirm in any subsequent calls if the http request is done)
// (could simple time the last request and last update)

// update stock price whenever so that we could have and save information from stocks history
// use this like a bot gather stock price every 5 minutes
// -- need stock history to train AI on 

// I want to be able to get different timers and have that data available

///////////////////////////////////////////////////////////////////////////

// have a watchlist of stocks we're always watching and recording data on

// have a way to set it so you can priotize stocks, i.e while making trades and I need the data asap

// this could potentially involve having some dristubted sytem queued up with tasks
// 

void updateStockInfo(std::string& symbol)
{

}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer) 
{
  size_t totalSize = size * nmemb;
  buffer->append((char*)contents, totalSize);
  return totalSize;
}

int main()
{
  CURL *curl; // handle used as input to other interfaces within curl
  curl = curl_easy_init();
  std::string response;

  if (curl)
  {
    while (true)
    {
      CURLcode res;
      curl_easy_setopt(curl, CURLOPT_URL, "https://robinhood.com/us/en/stocks/AMZN/");
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

      res = curl_easy_perform(curl);
      if (res != 0)
      {
        printf("error: %s\n", curl_easy_strerror(res));
        return 1;
      }
    }
  } 

  curl_easy_cleanup(curl);
  return 0;
} 

// should seperate threads that parse data from text and ones that make http requests
// make the network and cpu not hold each other up

// probably gonna want to print out data from this program into a buffer and can that buffer can be read from most recent with time
// by the other program. 
