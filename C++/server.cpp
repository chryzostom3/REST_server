#include <cpprest/http_listener.h>
#include <cpprest/json.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "openssl/sha.h"

using namespace std;

// function converting string to hex
string to_hex(unsigned char s)
{
    stringstream ss;
    ss << hex << (int) s;
    return ss.str();
}

// function calculating sha256 for given string
string sha256(string line)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, line.c_str(), line.length());
    SHA256_Final(hash, &sha256);

    string output = "";
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        output += to_hex(hash[i]);
    }

    return output;
}

// function displaying json object
void display_json(json::value const& jvalue)
{
    cout << jvalue.serialize() << endl;
}

// function handling post requests
void handle_post(http_request request)
{
    cout << "\nhandle POST\n";
    cout << endl;

    // getting json from request and preparing response
    request.extract_json().then([request](pplx::task<json::value> task) {
        try
        {
            vector<string> signature_vector;
            //parsing data from json Post request
            auto const jvalue = task.get();

            cout << "Received data from client" << endl;

            // getting json array from json
            auto array_data = jvalue.at(U("data")).as_array();

            // pushing signatures from json array into vector
            for(int i = 0; i < array_data.size(); i++)
            {
                auto signature = array_data[i].at(U("signature")).as_string();
                signature_vector.push_back(signature);
            }

            string signature = "";
            string number_of_items = to_string(signature_vector.size());

            // concatenating all signatures into one
            for(int i = 0; i < signature_vector.size(); i++)
            {
                signature += signature_vector[i];
            }

            // calculating sha256 for signature in hex
            signature = sha256(signature);

            cout << number_of_items << endl;
            cout << signature << endl;

            // creating response json
            json::value jsonResult;
            jsonResult[U("signature")] = json::value::string(signature);
            jsonResult[U("number-of-items")] = json::value::string(number_of_items);

            json::value jsonResponse;
            jsonResponse[U("metric")] = jsonResult;
            
            // setting response for post request
            request.reply(status_codes::OK, jsonResponse);

            display_json(jsonResponse);
            return;
        }
        catch (http_exception const& e)
        {
            cout << e.what() << endl;
            json::value jsonResponse;
            jsonResponse[U("error")] = json::value::string(U("Got some error"));
            request.reply(status_codes::OK, jsonResponse);
        }
        }).wait();
}


int main(int argc, char* argv[])
{
    // creating url on which server is listening
    string ip = argv[1];
    string url = "http://" + ip + ":9000";

    // creating listener on url
    http_listener listener(url);

    // setting function handle_post as method that is called for method POST
    listener.support(methods::POST, handle_post);

    try
    {
        // starting server
        listener.open().then([&listener]() 
        {
            cout << "\nC++ server is listening\n";
        }).wait();

        while (true);
    }
    catch (exception const& e)
    {
        cout << e.what() << endl;
    }

    return 0;
}