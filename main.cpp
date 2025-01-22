#include <iostream>



// 实现一个lsm树
// 支持增删改查
// 根据时间做索引
// 考虑实现wal
#include "memtable.h"
#include "index.h"
#include "kv_data.h"
#include "db.h"
#include "kv_search.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/signal_set.hpp>
#include <iostream>
#include <string>


//db* DB = new db("./storage");

void index_test(){
    std::vector<item> a;
    a.push_back(item("a","b"));
    lsm::index i = lsm::index(a,"/Users/xieqingxing/CLionProjects/lsm/storage/test.index");
    std::cout << i.bigKey << std::endl;
    i.serialize();
}

void index_test2(){
    lsm::index i = lsm::index("./storage/test.index");
    i.deserialize();
    std::cout << i.bigKey << std::endl;
    std::cout << i.keyNum << std::endl;
}

void kv_data_test(){
    std::vector<item> a;
    a.push_back(item("a","b"));
    kv_data i = kv_data(a,"/Users/xieqingxing/CLionProjects/lsm/storage/test.bin");
    std::cout << i.keyNum << std::endl;
    i.serialize();
}

void kv_data_test2(){
    kv_data i = kv_data("/Users/xieqingxing/CLionProjects/lsm/storage/test.bin");
    i.deserialize();
    std::cout << i.keyNum << std::endl;
    std::cout << i.pairs[0].key << std::endl;
}

void kv_search_test(){
    kv_search s = kv_search("./storage/1737512197");
//    s.load_key_by_index(0);
//    s.load_item_by_index(0);
    search_result res = s.search("a");
    res.i.print();
    search_result res2 = s.search("C");
    res2.i.print();
}

void db_test1(){
    db* DB = new db("./storage");
    DB->write("a","b");
    DB->write("C","d");
    DB->write("d","d");
}

void db_test2(){
    db* DB = new db("./storage/");
//    search_result res = DB->search("a");
//    res.i.print();
    DB->write("123","456");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    search_result res2 = DB->search("123");
    res2.print();
    std::this_thread::sleep_for(std::chrono::seconds(5));
}

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

class HttpServer {
public:
    HttpServer(net::io_context& ioc, tcp::endpoint endpoint)
            : acceptor_(ioc), socket_(ioc) {
        boost::system::error_code ec;

        // Open the acceptor
        acceptor_.open(endpoint.protocol(), ec);
        if (ec) {
            std::cerr << "Error opening acceptor: " << ec.message() << std::endl;
            return;
        }

        // Bind to the server address
        acceptor_.bind(endpoint, ec);
        if (ec) {
            std::cerr << "Error binding acceptor: " << ec.message() << std::endl;
            return;
        }

        // Start listening for connections
        acceptor_.listen(net::socket_base::max_listen_connections, ec);
        if (ec) {
            std::cerr << "Error listening: " << ec.message() << std::endl;
            return;
        }
    }

    void run() {
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept(socket_, [this](boost::system::error_code ec) {
            if (!ec) {
                std::make_shared<HttpSession>(std::move(socket_))->start();
            }
            do_accept();
        });
    }

    class HttpSession : public std::enable_shared_from_this<HttpSession> {
    public:
        explicit HttpSession(tcp::socket socket) : socket_(std::move(socket)) {}

        void start() {
            do_read();
        }

    private:
        void do_read() {
            auto self = shared_from_this();
            http::async_read(socket_, buffer_, request_, [self](beast::error_code ec, std::size_t) {
                if (!ec) {
                    self->handle_request();
                }
            });
        }

        void handle_request() {
            http::response<http::string_body> response;

            if (request_.method() == http::verb::get) {
//                std::string url = request_.target().to_string();
//                std::string query_string = url.substr(url.find('?') + 1);
//                std::cout << "Query string: " << query_string << std::endl;
//                std::map<std::string, std::string> query_params;
//                std::istringstream query_stream(query_string);
//                std::string param;
//                while (std::getline(query_stream, param, '&')) {
//                    size_t pos = param.find('=');
//                    if (pos != std::string::npos) {
//                        std::string key = param.substr(0, pos);
//                        std::string value = param.substr(pos + 1);
//                        query_params[key] = value;
//                    }
//                }
                response.result(http::status::ok);
                response.set(http::field::content_type, "text/plain");
//                search_result res = DB->search(query_params["key"]);
//                response.body() = res.to_string();
                response.body() = "123";
            } else if (request_.method() == http::verb::post && request_.target() == "/post") {
                response.result(http::status::ok);
                response.set(http::field::content_type, "application/json");
                response.body() = "{\"message\": \"POST received\", \"data\": \"" + request_.body() + "\"}";
            } else {
                response.result(http::status::not_found);
                response.set(http::field::content_type, "text/plain");
                response.body() = "Resource not found";
            }

            response.prepare_payload();
            auto self = shared_from_this();
            http::async_write(socket_, response, [self](beast::error_code ec, std::size_t) {
//                self->socket_.shutdown(tcp::socket::shutdown_send, ec);
            });
        }

        tcp::socket socket_;
        beast::flat_buffer buffer_;
        http::request<http::string_body> request_;
    };

    tcp::acceptor acceptor_;
    tcp::socket socket_;
};


int main() {
//    std::cout << "Hello, World!" << std::endl;
//    memtable* m = new memtable(2,1);
//    m->write("a","b");
//    m->write("a","b");
//    m->show_mem();
//    memtable_search_result res =  m->search("b");
//    res.print();
//    return 0;
//    db_test1();

//    index_test();
//    index_test2();
//    kv_data_test();
//    kv_data_test2();


// kv_search test
//    kv_search_test();
//    db_test2();
    try {
        auto const address = net::ip::make_address("0.0.0.0");
        unsigned short port = 8080;

        net::io_context ioc;

        // Create and run the server
        HttpServer server(ioc, {address, port});
        server.run();

        // Wait for signals to stop the server
        net::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&](boost::system::error_code const&, int) {
            ioc.stop();
        });

        std::cout << "HTTP server running on port " << port << std::endl;

        ioc.run();
    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
