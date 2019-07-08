#include "Aes.h"
#include <random>
#include <iostream>
#include <fstream>
#include <thread>
#include <assert.h>
#include <chrono>

Aes* aes = nullptr;
size_t outputSize = 0;

std::string FindParam(int argc, char* argv[], char param[])
{
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], param) == 0)
		{
			if (i + 1 <= argc - 1)
			{
				return std::string(argv[i + 1]);
			} else {
				std::cout << "Error while parsing command." << std::endl;
				exit(1);
			}
		}
	}
	return std::string("");
}

char* LoadFile(std::string path, size_t & size)
{
	std::fstream file;
	file.open(path.c_str(), std::ios::in | std::ios::binary);
	if (file.good() == true)
	{

		file.seekg(0, std::ios_base::end);
		size = file.tellg();
		file.seekg(0, std::ios_base::beg);

		char* bufor = new char[size];
		file.read(bufor, size);

		file.close();
		return bufor;
	}
	std::cout << "Cannot find file: "<<path << std::endl;
	exit(2);
}

void CreateFileFromBuffer(std::string path, char* data, size_t size)
{
	std::fstream file(path.c_str(), std::ios::out | std::ios::binary);
	file.write(data, size);
	file.close();
}

void PrintProgress(std::string mode)
{
	std::chrono::milliseconds timespan(33);
	int barWidth = 32;

	while (aes == nullptr || outputSize == 0)
	{
		std::this_thread::sleep_for(timespan);
	}

	while (true) {
		float progress = (float)aes->counter / (float)outputSize + 0.01f;
		std::cout << mode << "[";
		int pos = barWidth * progress;
		for (int i = 0; i < barWidth; ++i) {
			if (i < pos) std::cout << "=";
			else if (i == pos) std::cout << ">";
			else std::cout << " ";
		}
		int real = int(progress * 100.0f);
		if (real > 100) real = 100;
		std::cout << "] " << real << " %\r";
		std::cout.flush();
		if (progress >= 1.0f || progress < 0.0f) break;
		std::this_thread::sleep_for(timespan);
	}
	std::cout << std::endl;
}

void ProcessingThread(std::string inputStr, std::string outputStr, std::string modeStr, char* key, char* initv)
{
	size_t size;
	char* data = LoadFile(inputStr, size);

	if (modeStr == "crypt") {
		uint8_t* crypted = aes->CryptData(data, size, key, (uint8_t*)initv, outputSize);
		CreateFileFromBuffer(outputStr, (char*)crypted, outputSize);
		delete[] crypted;

	} else if (modeStr == "decrypt") {
		char* decrypted = aes->DecryptData((uint8_t*)data, size, key, (uint8_t*)initv, outputSize);
		CreateFileFromBuffer(outputStr, (char*)decrypted, outputSize);
		delete[] decrypted;
	}
	delete[] data;
	std::cout << "Done." << std::endl;
}

int main(int argc, char* argv[])
{
	assert(sizeof(char) == sizeof(uint8_t));

	//----------------------------------------------------------------------
	//Print help
	//----------------------------------------------------------------------
	if (argc == 1 || (argc == 2 && strcmp(argv[1], "-h") == 0)) {
		std::cout << "Usage: aes.exe [-m <mode>] [-i <path to file>] [-o <path to file>] [-p <password>] [-v <init vector>] " << std::endl << std::endl;
		std::cout << "Parameters description:" << std::endl;
		std::cout << "  -h\t Prints this guide." << std::endl;
		std::cout << "  -m\t Specifies the operation mode. Valid values: crypt, decrypt." << std::endl;
		std::cout << "  -i\t Specifies the input file." << std::endl;
		std::cout << "  -o\t Specifies the output file." << std::endl;
		std::cout << "  -p\t Specifies the password to use. Max 16 bytes." << std::endl;
		std::cout << "  -v\t Specifies the init vector to use. Max 16 bytes." << std::endl;
		std::cout << std::endl;
		exit(0);
	}

	//----------------------------------------------------------------------
	//Check params
	//----------------------------------------------------------------------
	std::string passwdStr = FindParam(argc, argv, "-p");
	bool ifPasswd = !passwdStr.empty();

	std::string initvStr = FindParam(argc, argv, "-v");
	bool ifInitv = !initvStr.empty();

	std::string inputStr = FindParam(argc, argv, "-i");
	bool inputCheck = !inputStr.empty();

	std::string outputStr = FindParam(argc, argv, "-o");
	bool outputCheck = !outputStr.empty();

	std::string modeStr = FindParam(argc, argv, "-m");
	bool mode = !modeStr.empty();

	//----------------------------------------------------------------------
	//Handle password & init vector & paths
	//----------------------------------------------------------------------

	if (mode == false)
	{
		std::cout << "Please specify the operation mode." << std::endl;
		exit(0);
	} else {
		if (modeStr != "crypt" && modeStr != "decrypt"){
			std::cout << "Valid values for -m parameter are: crypt, decrypt." << std::endl;
			exit(0);
		}
	}

	if (outputCheck == false || inputCheck == false)
	{
		std::cout << "Please specify the input and output file." << std::endl;
		exit(0);
	}

	char key[16];
	memset(key, 0, 16);
	if (ifPasswd)
	{
		if (passwdStr.size() <= 16) {
			memcpy(key, passwdStr.c_str(), passwdStr.size());
		} else {
			std::cout << "Password is too long." << std::endl;
			exit(0);
		}
	} else {
		if (modeStr == "decrypt")
		{
			std::cout << "You have to enter the password for decryption." << std::endl;
			exit(0);
		}

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> rng(33, 126);
		for (int i = 0; i < 16; i++)
		{
			key[i] = rng(gen);
		}
	}

	char initv[16];
	memset(initv, 0, 16);
	if (false)
	{
		if (initvStr.size() <= 16) {
			memcpy(initv, initvStr.c_str(), initvStr.size());
		}
		else {
			std::cout << "Init vector is too long." << std::endl;
			exit(0);
		}
	} else {
		for (int i = 0; i < 16; i++)
		{
			initv[i] = 0;
		}
	}


	//----------------------------------------------------------------------
	//Crypting/Decrypting
	//----------------------------------------------------------------------	
	aes = new Aes;
	std::thread thread(ProcessingThread, inputStr, outputStr, modeStr, key, initv);
	PrintProgress("Working... ");
	thread.join();
	delete aes;

}