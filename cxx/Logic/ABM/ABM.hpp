//
// Created by Daniel on 05/04/2024.
//

#ifndef ABM_HPP
#define ABM_HPP

#include "OrderBook.hpp"
#include "API.hpp"
#include <filesystem>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class Abm {
 private:
  static int counter;
  static std::vector<std::vector<double>> csvData;

 public:
  static std::thread startGenerate;
  static std::atomic<bool> cancelRequested;
  static void readCsv(const std::string& filename);
  static void generateQuantity(OrderBook *o);
  static void startServer(OrderBook *o);
  static std::string getProjectSourceDirectory();
};

#endif