#include <iostream>
#include <curl/curl.h>

// API KEY: G8JYPPY20JYPYTAB

int main()
{
  CURL *curl; // handle used as input to other interfaces
  curl = curl_easy_init();

  if (curl)
  {
    CURLcode res;
    curl_easy_setopt(curl, CURLOPT_URL, 
        "https://www.alphavantage.co/query?"
        "function=TIME_SERIES_INTRADAY&"
        "symbol=IBM"
        "&interval=5min&"
        "apikey=G8JYPPY20JYPYTAB");

    res = curl_easy_perform(curl);
    if (res != 0)
    {
      printf("error: %s\n", curl_easy_strerror(res));
    }
  }

  curl_easy_cleanup(curl);
  return 0;
}