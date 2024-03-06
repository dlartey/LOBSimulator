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

httplib::Client API::cli("localhost:8080");
httplib::Server API::s;
std::map< int, std::map<std::string, double> > API::orderHistory;
std::random_device rd;
std::default_random_engine e( rd() );

// Gets the current positions which a user has open
void API::getOrders(OrderBook &o) {
    s.Get("/orders", [&o](const httplib::Request& req, httplib::Response& res) {
        try{
            nlohmann::json response_json = nlohmann::json::array();
            for (const auto& entry: orderHistory){
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

// the thread to read from the db updates the map each time. Account for this when removing from the DB
// if it's no longer in the order book, then assume it's been fulfilled
void API::deleteOrder(OrderBook &o) {
    s.Delete("/delete", [&o](const httplib::Request& req, httplib::Response& res) {
        if (req.has_header("Content-Type") && req.get_header_value("Content-Type") == "application/json") {
            try {
                auto json_data = nlohmann::json::parse(req.body);
                if (json_data.contains("id") && json_data.contains("price") && json_data.contains("bidAsk")) {
                    int id = json_data["id"].get<int>();
                    bool bidAsk = json_data["bidAsk"].get<bool>();
                    double price = json_data["price"].get<double>();

                    // Check whether the ID is in the orderHistory
                    if (orderHistory.find(id) == orderHistory.end()){
                        res.status = 400;
                        res.set_content("Order ID doesn't exist!", "text/plain");
                    }

                    // If order removed, then give SUCCESS message
                    orderHistory.erase(id);
                    if (o.remove_order(id, price, bidAsk)){
                        res.set_content("Order removed from the OrderBook", "text/plain");
                    }else{
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

void API::submitOrder(OrderBook &o) {
    s.Post("/submit", [&o](const httplib::Request &req, httplib::Response &res) {
        if (req.has_header("Content-Type") && req.get_header_value("Content-Type") == "application/json") {
            try {
                auto json_data = nlohmann::json::parse(req.body);
                if (json_data.contains("price") && json_data.contains("bidAsk") && json_data.contains("quantity")) {
                    // Generate a random number for the orderID
                    int random = e();
                    bool bidAsk = json_data["bidAsk"].get<bool>();
                    double price = json_data["price"].get<double>();
                    double quantity = json_data["quantity"].get<double>();
                    o.add_order(random, price, quantity,bidAsk);

                    // adds a new entry to the book
                    std::map<std::string, double> new_entry;
                    new_entry["price"] = price;
                    new_entry["quantity"] = quantity;
                    new_entry["bidAsk"] = bidAsk;
                    orderHistory[random] = new_entry;

                    res.set_content("Order Added to the OrderBook", "text/plain");
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

void API::startServer(OrderBook &o) {
    submitOrder(o);
    deleteOrder(o);
    getOrders(o);
    s.listen("localhost", 8080);
}

