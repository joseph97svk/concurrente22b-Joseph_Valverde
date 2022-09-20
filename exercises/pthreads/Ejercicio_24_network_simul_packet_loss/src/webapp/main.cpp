// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0
// Serial web server's initial code for parallelization

#ifdef WEBSERVER

#include "HttpServer.hpp"
#include "FactWebApp.hpp"

/// Start the web server
int main(int argc, char* argv[]) {
  // Create the web server
  HttpServer httpServer;
  // Create a factorization web application, and other apps if you want
  FactWebApp factWebApp;
  // Register the web application(s) with the web server
  httpServer.chainWebApp(&factWebApp);
  // Start the web server
  return httpServer.start(argc, argv);
}

#endif  // WEBSERVER
