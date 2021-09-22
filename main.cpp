#include <iostream>
#include <map>
#include <sstream>
#include "cpr/cpr.h"

enum RequestType
{
    GET,
    POST
};

const std::string WEBSITE_GET = "http://httpbin.org/get";
const std::string WEBSITE_POST = "http://httpbin.org/post";

/**
 * @function Print actual headers and values to console.
 * @param [in] headers std::map with headers & value
 */
void printHeaders(std::map<std::string, std::string>* headers)
{
    for (auto pair : *headers)
    {
        std::cout << "\t - " << pair.first << " : " << pair.second << "\n";
    }
    std::cout << "--------------\n";
}

/**
 * @function Return the string of headers & values in URL form.
 * @param [in] headers std::map with headers & values
 * @return [out] std::string URL with parameters (headers & values)
 */
std::string makeURLRequestFromHeaders(std::map<std::string, std::string>* headers)
{
    if (headers->empty()) return "";

    std::string result = "?";
    {
        for (auto it : *headers)
        {
            result += it.first;
            result += "=";
            result += it.second;
            result += "&";
        }
    }
    if (result[result.length() - 1] == '&')
        result.resize(result.length()-1);

    return result;
}

/**
 * @function Send the specified request to server using actual headers & values.
 * @param [in] requestType enum with type of the request
 * @param [in] headers std::map with headers & values
 */
void sendRequest(RequestType requestType, std::map<std::string, std::string>* headers)
{
    if (requestType == POST)
    {
        cpr::Payload payLoad({});
        for (auto it : *headers)
        {
            cpr::Pair* p = new cpr::Pair(it.first.c_str(), it.second.c_str());
            payLoad.Add(*p);
        }

        cpr::Response r = cpr::Post(cpr::Url(WEBSITE_POST),
                                    cpr::Payload(payLoad));
        std::cout << "\t - POST request is sent.\n";
        std::cout << "\t - Server reply:\n";
        std::cout << r.text << "\n";
    }
    else if (requestType == GET)
    {
        std::string urlWithParameters = WEBSITE_GET + makeURLRequestFromHeaders(headers);
        cpr::Response r = cpr::Get(cpr::Url(urlWithParameters));
        std::cout << "GET request is sent: " + urlWithParameters + "\n";
        std::cout << "Server reply:\n";
        std::cout << r.text << "\n";
    }
}

/**
 * @function Adds pair of arguments to the named map.
 * @param [in/out] headers std::map to be used
 * @param [in] first std::string argument 1
 * @param [out] second std::string argument 2
 */
void addToHeaders(std::map<std::string, std::string>* headers, std::string first, std::string second)
{
    auto pair = std::make_pair(first, second);
    auto it = headers->find(first);
    if (it == headers->end()) headers->insert(pair);
    else it->second = second;
    std::cout << "Pair " << first << " : " << second << " is added to map.\n";
}

/**
 * @function Show help prompt with command list.
 */
void showHelp()
{
    std::cout << "Please enter the input.\n";
    std::cout << "\t - \'[header]\' \'[value]\' ... with space delimiter to set / reset header.\n";
    std::cout << "\t - \'get\' - to make get request with named headers.\n";
    std::cout << "\t - \'post\' - to make post request with named headers.\n";
    std::cout << "\t - \'headers\' - to print actual headers & values.\n";
    std::cout << "\t - \'clear\' - to remove actual headers & values.\n";
    std::cout << "\t - \'help\' - to see this message again.\n";
}

int main() {

    std::map<std::string, std::string> headers;
    std::string input;
    std::string headerName;
    std::string headerValue;
    bool isFirst = true;
    showHelp();

    do
    {
        std::cin >> input;
        if (input == "get")
        {
            sendRequest(GET, &headers);
        }
        else if (input == "post")
        {
            sendRequest(POST, &headers);
        }
        else if (input == "headers")
        {
            printHeaders(&headers);
        }
        else if (input == "clear")
        {
            std::cout << "Clearing headers...\n";
            headers.clear();
            std::cout << "Headers are cleared.\n";
        }
        else if (input == "exit")
        {
            break;
        }
        else if (input == "help")
        {
            showHelp();
        }
        else
        {
            if (isFirst)
            {
                headerName = input;
                isFirst = false;
            }
            else
            {
                headerValue = input;
                auto pair = std::make_pair(headerName, headerValue);
                addToHeaders(&headers, headerName, headerValue);
                isFirst = true;
            }
        }
    } while (input != "exit");

    return 0;
}