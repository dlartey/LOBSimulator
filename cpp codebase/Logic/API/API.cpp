//
// Created by Daniel on 29/01/2024.
//
#include "API.hpp"
#include <nlohmann/json.hpp>

/*
 * Endpoints
 * GET: /pos -> returns the users current positions -> returns a list of positions
 *
 * POST: /submit -> attempts to add the user's order to the orderbook
 * returns a orderID
 *
 * DELETE: /cancel -> this removes a user's order if it hasn't been fulfilled
 * returns corresponding status code if successful or not
 *
 */

// Variables
httplib::Client API::cli("localhost:8080");
httplib::Server API::s;
std::random_device rd;
std::default_random_engine e(rd());
std::map<int, std::map<std::string, double> > API::orderHistory;
int API::price = 200;
float API::balance = 100000.00;
float API::quantity = 0.00;
float API::pnl = 0.00;

void API::setPrice(int p) {
  price = p;
}

int API::getPrice() {
  return price;
}

void API::updatePnL() { pnl = balance + (float) (quantity * price); }

// Handles IOC bid orders
// Since it's a bid, we subtract from balance
void API::IOC_bid(float targetPrice, float targetQuantity, OrderBook &o) {
  std::map<double, OrderList> asks = o.getAsks();

  for (const auto &ob : asks) {
    std::list<Order> ordersForPL = ob.second;
    for (Order ob2 : ordersForPL) {
      if (targetPrice >= ob2.price) {
        float q = std::min((float) ob2.quantity, targetQuantity);

        // Check that we have enough balance to buy for the user
        if (q * ob2.price > balance) {
          // then only buy enough for the quantity
          quantity -= (float) (balance / ob2.price);
          balance = 0;
          return;
        } else {
          quantity += q;
          balance -= (float) (q * ob2.price);
        }
        targetQuantity -= q;
      } else {
        return;
      }
    }
  }
}

// Handles FOK bid orders
// Since it's a bid, we subtract from balance
void API::FOK_bid(float targetPrice, float targetQuantity, OrderBook &o) {

  for (Order ob : o.getAsks().cbegin()->second) {
    if (targetPrice >= ob.price) {

      // Check that we have enough balance to buy
      // check this again
      if (ob.quantity * ob.price > balance or ob.quantity > targetQuantity) {
        return;
      }
      quantity += (float) ob.quantity;
      balance -= (float) (ob.quantity * ob.price);
      targetQuantity -= (float) ob.quantity;
    } else {
      return;
    }
  }
}

// Handles IOC bid orders
// Since it's an ask, we add to balance
void API::IOC_ask(float targetPrice, float targetQuantity, OrderBook &o) {

  std::map<double, OrderList> bids = o.getBids();
  for (auto iter = bids.rbegin(); iter != bids.rend(); ++iter) {
    for (auto const &i : iter->second) {

      if (i.price >= targetPrice) {
        float q = std::min((float) i.quantity, targetQuantity);

        // Check that we have enough balance to buy for the user
        if (q > quantity) {
          // then only sell enough for the quantity
          balance += (float) (quantity * i.price);
          quantity = 0;
          return;
        } else {
          balance += (float) (q * i.price);
          quantity -= q;
        }
        targetQuantity -= q;
      } else {
        return;
      }
    }

  }
}

// Since it's an ask, we add to balance
void API::FOK_ask(float targetPrice, float targetQuantity, OrderBook &o) {

  // check that we have the quantity to sell first
  for (Order ob : o.getBids().cend()->second) {
    if (ob.price >= targetPrice) {

      // Check that we have enough balance to buy for the user
      if (ob.quantity > quantity or ob.quantity > targetQuantity) {
        return;
      }

      balance += (float) (ob.quantity * ob.price);
      quantity -= (float) ob.quantity;
      targetQuantity -= (float) ob.quantity;
    } else {
      return;
    }
  }
}

// Gets the orders that the user has submitted
void API::getOrders(OrderBook &o, DBHandler &handler) {
  s.Get("/orders", [&o](const httplib::Request &req, httplib::Response &res) {
    try {
      nlohmann::json response_json = nlohmann::json::array();
      for (const auto &entry : orderHistory) {
        nlohmann::json item;

        item["id"] = entry.first;
        item["price"] = entry.second.at("price");
        item["bidAsk"] = entry.second.at("bidAsk");
        item["quantity"] = entry.second.at("quantity");
        response_json.push_back(item);
      }
      res.set_content(response_json.dump(), "application/json");
      return;

    } catch (const std::exception &e) {
      std::cerr << "Unable to find one of the keys: " << e.what() << std::endl;
    }
    res.status = 400;
    res.set_content("Unable to find one of the keys", "text/plain");
  });
}

void API::getPnL(OrderBook &o, DBHandler &handler) {
  s.Get("/pnl", [&o](const httplib::Request &req, httplib::Response &res) {
    try {
      API::updatePnL();
      nlohmann::json item;
      item["PnL"] = pnl;
      item["balance"] = balance;
      item["quantity"] = quantity;
      item["price"] = price;
      nlohmann::json response_json = nlohmann::json::array();
      response_json.push_back(item);
      res.set_content(response_json.dump(), "application/json");
      return;

    } catch (const std::exception &e) {
      std::cerr << "Unable to get PnL: " << e.what() << std::endl;
    }
    res.status = 400;
    res.set_content("Unable to get PnL", "text/plain");
  });
}

// the thread to read from the db updates the map each time. Account for this when removing from the DB
// if it's no longer in the order book, then assume it's been fulfilled
void API::deleteOrder(OrderBook &o, DBHandler &handler) {
  s.Delete("/delete", [&o](const httplib::Request &req, httplib::Response &res) {
    if (req.has_header("Content-Type") && req.get_header_value("Content-Type") == "application/json") {
      try {
        auto json_data = nlohmann::json::parse(req.body);
        if (json_data.contains("id") && json_data.contains("price") && json_data.contains("bidAsk")) {
          int id = json_data["id"].get<int>();
          bool bidAsk = json_data["bidAsk"].get<bool>();
          double price = json_data["price"].get<double>();

          // Check whether the ID is in the orderHistory
          if (orderHistory.find(id) == orderHistory.end()) {
            res.status = 400;
            res.set_content("Order ID doesn't exist!", "text/plain");
          }

          // If order removed, then give SUCCESS message
          orderHistory.erase(id);
          if (o.remove_order(id, price, bidAsk)) {
            res.set_content("Order removed from the OrderBook", "text/plain");
          } else {
            // Order was fulfilled, so reflect this within the OrderbookHistory
            // will change this once we start working on integrating with trading algorithm
            // essentially, get the price the user submitted, and based on the quantity, bidAsk and price
            // either add/remove from their balance

            // will also need to check the users current positions every second & check whether the limit
            // order can be executed. future feature.
            res.set_content("Order has already been fulfilled!", "text/plain");
          }
          return;
        }
      } catch (const std::exception &e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
      }
    }
    res.status = 400;
    res.set_content("Invalid JSON structure", "text/plain");
  });
}

// Check for exisitng quantity first before adding
void API::submitOrder(OrderBook &o, DBHandler &handler) {

  s.Post("/submit", [&o, &handler](const httplib::Request &req, httplib::Response &res) {
    if (!req.has_header("Content-Type") || req.get_header_value("Content-Type") != "application/json") {
      res.status = 400;
      res.set_content("Invalid request structure", "text/plain");
    }

    try {
      auto json_data = nlohmann::json::parse(req.body);
      if (!json_data.contains("price") || !json_data.contains("bidAsk") || !json_data.contains("quantity")
          || !json_data.contains("orderType")) {
        res.status = 400;
        res.set_content("Invalid parameters in request body", "text/plain");
      }

      int random = e();
      bool bidAsk = json_data["bidAsk"].get<bool>();
      float price = json_data["price"].get<float>();
      float quantity = json_data["quantity"].get<float>();
      std::string type = json_data["orderType"].get<std::string>();

      if (type == "IOC") {
        if (bidAsk) API::IOC_bid(price, quantity, o);
        else API::IOC_ask(price, quantity, o);
      } else if (type == "FOK") {
        if (bidAsk) API::FOK_bid(price, quantity, o);
        else API::FOK_ask(price, quantity, o);
      } else {
        res.set_content("Invalid Order Type!", "text/plain");
        return;
      }

      std::map<std::string, double> new_entry;
      new_entry["price"] = price;
      new_entry["quantity"] = quantity;
      new_entry["bidAsk"] = bidAsk;
      orderHistory[random] = new_entry;
      handler.emitSuccessfulUpdate();
      res.set_content("Attempted Order", "text/plain");

    } catch (const std::exception &e) {
      res.set_content("Error parsing JSON", "text/plain");
    }
    res.status = 400;
    res.set_content("Invalid JSON structure", "text/plain");
  });
  o.OB_mutex.unlock();
}

void API::startServer(OrderBook &o, DBHandler &handler) {
  submitOrder(o, handler);
  deleteOrder(o, handler);
  getOrders(o, handler);
  getPnL(o, handler);
  s.listen("localhost", 8080);
}


