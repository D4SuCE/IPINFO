#define CURL_STATICLIB

#include <string>
#include <fstream>
#include <iostream>
#include "nlohmann/json.hpp"
#include "curl/curl.h"

using namespace nlohmann;

size_t SizeData(char* buffer, size_t size, size_t nitems, void* userp)
{
	((std::string*)userp)->append((char*)buffer, size * nitems);
	return size * nitems;
}

int main()
{
	std::string ip;
	std::cout << "Enter an IP: ";
	std::cin >> ip;
	std::string link = "http://ip-api.com/json/";

	CURL* curl = curl_easy_init();
	std::string response;
	curl_easy_setopt(curl, CURLOPT_URL, (link + ip).c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, SizeData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	json Data = json::parse(response);

	if (Data["status"] == "fail")
	{
		std::cout << "Failed!" << std::endl;
	}
	else
	{
		std::fstream file;
		file.open(ip + ".txt", std::fstream::out);
		if (file.is_open())
		{
			file << "IP : " << Data["query"].get<std::string>() << std::endl;
			file << "Country : " << Data["country"].get<std::string>() << std::endl;
			file << "Region : " << Data["regionName"].get<std::string>() << std::endl;
			file << "City : " << Data["city"].get<std::string>() << std::endl;
			file << "ZIP : " << Data["zip"].get<std::string>() << std::endl;
			file << "Lat : " << Data["lat"].get<double>() << std::endl;
			file << "Lon : " << Data["lon"].get<double>() << std::endl;
			file << "Provider : " << Data["isp"].get<std::string>() << std::endl;
			file << "Organization : " << Data["org"].get<std::string>() << std::endl;
			file.close();
			std::cout << "Done!" << std::endl;
		}
		else
		{
			std::cout << "What is going wrang!" << std::endl;
		}
	}
	system("pause");
	return 0;
}