//
//  OrderBook.cpp
//  TradingEngine
//
//  Created by Shreyas Honnalli on 06/11/2023.
//
#include "OrderBook.hpp"

bool OrderBook::is_empty() const { return bids.empty() && asks.empty(); }

void OrderBook::clear_order_book() {
  bids.clear();
  asks.clear();
}

// Add an order to the order book
void OrderBook::add_order(int id, double price, double quantity, bool is_bid) {
  Order order(id, price, quantity);
  if (is_bid) {
    bids[price].push_back(order);
  } else {
    asks[price].push_back(order);
  }
}

// return true if order removed, else false
bool OrderBook::remove_order(int id, double price, bool is_bid) {
  bool valid = false;
  auto &price_level_orders = is_bid ? bids : asks;
  auto it = price_level_orders.find(price);
  if (it != price_level_orders.end()) {
    auto &orders = it->second;
    for (auto order_it = orders.begin(); order_it != orders.end();) {
      if (order_it->id == id) {
        order_it = orders.erase(order_it); // Remove the order
        valid = true;
        break; // Assuming id is unique, we can break after finding it
      } else {
        ++order_it;
      }
    }

    // If there are no more orders at this price, remove the price level
    if (orders.empty()) {
      price_level_orders.erase(it);
    }
  }
  return valid;
}

// print order_book
void OrderBook::print_order_book() const {
  std::cout << "Current Order Book State:\n";

  // Print asks
  int index = 0; // Initialize index to keep track of the order number.
  for (const auto &ask : asks) {
    for (const auto &order : ask.second) {
      std::cout << "index = " << index++
                << "\nprice_level = " << ask.first
                << "\nnew_quantity = " << order.quantity
                << "\nid = " << order.id << "_ask\n";
    }
  }

  // Print bids
  for (const auto &bid : bids) {
    for (const auto &order : bid.second) {
      std::cout << "index = " << index++
                << "\nprice_level = " << bid.first
                << "\nnew_quantity = " << order.quantity
                << "\nid = " << order.id << "_bid\n";
    }
  }

  std::cout << "\n-----------------------------\n";
}

void OrderBook::modify_order(int id, double old_price, double new_price, double new_quantity, bool is_bid) {
  // Find the list of orders at the old price level
  auto &price_level_orders = is_bid ? bids : asks;
  auto it_price_level = price_level_orders.find(old_price);

  if (it_price_level != price_level_orders.end()) {
    // Find the specific order by id
    auto &orders = it_price_level->second;
    for (auto it_order = orders.begin(); it_order != orders.end(); ++it_order) {
      if (it_order->id == id) {
        // If the price has changed, we need to move the order to the correct price level
        if (old_price != new_price) {
          // Remove the order from the old price level
          it_order = orders.erase(it_order);

          // If there are no more orders at this price level, remove the price level from the map
          if (orders.empty()) {
            price_level_orders.erase(it_price_level);
          }

          // Add a new order at the new price level
          add_order(id, new_price, new_quantity, is_bid);
        } else {
          // If only the quantity has changed, modify the order in place
          it_order->quantity = new_quantity;
        }
        return; // We're done, so we can exit the function
      }
    }
  }
}

// Overload the << operator to print the order book
std::ostream &operator<<(std::ostream &os, const OrderBook &book) {
  os << "Asks:\n";
  for (const auto &ask : book.asks) {
    os << "Price " << ask.first << ": ";
    for (const auto &order : ask.second) {
      os << "Quantity: " << order.quantity << ", ID: " << order.id << "\t||||";
    }
  }

  os << "\nBids:\n";
  for (const auto &bid : book.bids) {
    os << "Price " << bid.first << ": ";
    for (const auto &order : bid.second) {
      os << "Quantity: " << order.quantity << ", ID: " << order.id << "\t||||";
    }
  }
  return os;
}

bool OrderBook::empty() { return bids.empty() && asks.empty(); }

std::map<double, OrderList> OrderBook::getBids() { return bids; }

std::map<double, OrderList> OrderBook::getAsks() { return asks; }

int OrderBook::getOrderCount() { return asks.size() + bids.size(); }

OrderBook::OrderBook() {}

OrderBook::~OrderBook() {}
