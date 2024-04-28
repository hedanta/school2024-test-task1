#include "order_analytics.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

#include <date/tz.h>
#include <nlohmann/json.hpp>

using json = nlohmann::ordered_json;

const std::vector<Order> OrderAnalytics::ReadOrdersFile(const std::string& filename) {
    std::ifstream orders_file;
    json data;

    try {
        orders_file.open(filename);
        if (!orders_file.is_open()) {
            throw std::runtime_error("Error: Unable to open file '" + filename + "'");
        }
        data = json::parse(orders_file);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        throw std::ifstream::failbit;
    }

    orders_file.close();

    std::vector<Order> orders_list;
    for (auto it = data.begin(); it != data.end(); it += 1) {
        Order current_order = Order((*it)["user_id"].get<std::string>(),
                                    (*it)["ordered_at"].get<std::string>(),
                                    (*it)["status"].get<std::string>(),
                                    (*it)["total"].get<std::string>());
        orders_list.push_back(current_order);
    }

    return orders_list;
}

OrderAnalytics::~OrderAnalytics() = default;

OrderAnalytics::OrderAnalytics(const std::string &filename)
        : orders_(ReadOrdersFile(filename))
{
};

const std::vector<std::string> OrderAnalytics::GetMaxSpendingMonth() const {
    std::map<std::string, double> month_total = {
            {"january", 0}, {"february", 0}, {"march", 0}, {"april", 0},
            {"may", 0}, {"june", 0}, {"july", 0}, {"august", 0},
            {"september", 0}, {"october", 0}, {"november", 0}, {"december", 0}
    };

    double max_amount = 0;

    for (Order order : orders_) {
        std::string current_month = order.GetMonth();

        if (order.GetStatus() == "COMPLETED") {
            month_total[current_month] += std::stod(order.GetTotal());
            if (month_total[current_month] > max_amount) {
                max_amount = month_total[current_month];
            }
        }
    }

    std::vector<std::string> result;
    for (auto it : month_total) {
        const std::string& month = it.first;
        const double& count = it.second;

        if (count == max_amount) {
            result.push_back(month);
        }
    }

    return result;
}

const void OrderAnalytics::CreateMonthReport() const {
    std::vector<std::string> max_months = GetMaxSpendingMonth();

    json j_report = {
            {"months", max_months}
    };

    std::cout << j_report;
}