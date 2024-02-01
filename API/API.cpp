//
// Created by Daniel on 29/01/2024.
//
#include "API.hpp"
#include <nlohmann/json.hpp>
/*
 * Endpoints
 * GET: /pos -> returns the users current position (all of them)
 *
 * GET: /pos/{id} -> allows the user to see a specific position
 *
 * POST: /submit -> attempts to add the user's order to the orderbook
 * returns a txid
 *
 * UPDATE: /edit -> updates the user's submitted order if it hasn't been fulfilled yet
 * returns corresponding status code if it can be changed along with message
 *
 * DELETE: /cancel -> this removes a user's order if it hasn't been fulfilled
 * returns corresponding status code if successful or not
 *
 * Also need to check that we can access the server when the main program is run
 * And if the port is free
 */

// Makes an HTTP GET request to add orders to the orderbook

httplib::Client API::cli("localhost:8080");
httplib::Server API::s;

void API::getOrders(OrderBook &o){
    if (auto res = cli.Get("/pos")){
        if (res->status == 200){
            int i  = 0;
        }
    }
}

 void API::submitOrder(OrderBook &o){
    s.Post("/submit", [&o](const httplib::Request& req, httplib::Response& res) {
        if (req.has_header("Content-Type") && req.get_header_value("Content-Type") == "application/json") {
            try {
                auto json_data = nlohmann::json::parse(req.body);
                if (json_data.contains("price") && json_data.contains("bidAsk") && json_data.contains("quantity")) {
                    double p = json_data["price"].get<double>();
                    double q = json_data["quantity"].get<double>();
                    bool bA =json_data["bidAsk"].get<bool>();
                    o.add_order(1000, p, q, bA);
                    res.set_content("Order Added to the OrderBook", "text/plain");
                    return;
                }
            } catch (const std::exception& e) {
                std::cerr << "JSON parsing error: " << e.what() << std::endl;
            }
        }

    res.status = 400;
    res.set_content("Invalid JSON structure", "text/plain");
});
}

void API::startServer(OrderBook &o){
    submitOrder(o);
    s.listen("localhost", 8080);
}

