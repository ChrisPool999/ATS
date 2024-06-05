#include <iostream>
#include <curl/curl.h>

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
    CURLcode res;
    curl_easy_setopt(curl, CURLOPT_URL, "https://robinhood.com/us/en/stocks/AMZN/");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    res = curl_easy_perform(curl);
    if (res != 0)
    {
      printf("error: %s\n", curl_easy_strerror(res));
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
    std::cout << "The bid price is " << bid_price << "\n";
  }

  curl_easy_cleanup(curl);
  return 0;
}

// should seperate threads that parse data from text and ones that make http requests
// make the network and cpu not hold each other up

