#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <unistd.h>
#include <string>
#include "my_mqtt.hpp"
#include <sstream>
#include <nlohmann/json.hpp>
std::string url = "";
std::string serverip_get(){
    try{
        curlpp::Cleanup cleaner;
        curlpp::Easy request;
        std::ostringstream response;
        request.setOpt(new curlpp::options::Url(url)); 
        request.setOpt(new curlpp::options::Verbose(true)); 
        std::list<std::string> header; 
        header.push_back("Content-Type: application/octet-stream"); 
        request.setOpt(new curlpp::options::HttpHeader(header)); 
        request.setOpt(new curlpp::options::PostFields("abcd"));
        request.setOpt(new curlpp::options::PostFieldSize(0));
        request.setOpt(new curlpp::options::WriteStream(&response));
        request.perform(); 
        return nlohmann::json::parse(response.str())["IP"];
    }
    catch(...){
        std::cout << "Server is down" << std::endl;
    }
}

void mqtt_callback(std::string data){
    std::cout << data << endl;
}

int main()
{
    std::string ip_addr = serverip_get();
    my_mqtt::my_mqtt_service new_device(ip_addr, "port", "username", "password", "clientid");
    if(!new_device.connect()){
        std::cout << "connect fail" << std::endl;
    }
    my_mqtt::nodehandle nh(&new_device);
    my_mqtt::publisher *pub = nh.advertise("mqtt_subscribe_status");
    my_mqtt::subscriber *subs = nh.subscribe("mqtt_subscribe_status", mqtt_callback);
    while(true){
        pub->publish("alive");
        sleep(1);
    }
    return 0;
}