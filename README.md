# Project: Crypto_Trading_api_c++
Project Trading API Implementation in C++

API_DOC_BITVAVO: https://docs.bitvavo.com/.

Adapt to your specifications

- Program uses libCURL and openSSL.
- The program queries the server for the name and price of a cryptocurrency.
- Program has in-built RequestLimit protection so your IP does not get banned by Bitvavo.
- I will add many more features into this project, such as trading and selling with a target, Automatic Trading Bot, 
Advanced market indicator and predictor for Binance and other populair platforms. 


EXAMPLE:


```
#include <iostream>
#include "bitvavo.h"

int main() {

   bitvavo bit;
   
   bit.start();
   
   std::cin.get();
   
   bit.stop();
   
   return 0;	
}
```
