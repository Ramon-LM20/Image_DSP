#include <iostream>
#include <array>
#include <fstream>
#include <vector>
#include "timeControl.hpp"
#include "imageDSP.hpp"

const std::string imageName1{ "images/girlface.bmp" };
const std::string copyImage1{ "images/girlface_copy.bmp" };
const std::string brighterImage1{ "images/girlface_brt150.bmp" };

const std::string imageName2{ "images/lena512.bmp" };
const std::string copyImage2{ "images/lena512.bmp" };
const std::string brighterImage2{ "images/lena5122.bmp" };

int main(int argc, char* argv[]) 
{
	timeControl mainTime;

	imageDSP* myImage = new imageDSP(imageName2); //Cria o objeto na heap passando apenas o path para o construtor

	myImage->readImage();
	myImage->brigthnessControl(-15);
	myImage->writeImage("images/imageDarker.bmp");
	
	//Criando o Histograma foto original escurecida
	myImage->computeHistogram(GREYSCALE);
	myImage->writeHistogram("notes/original.txt");
	myImage->cumulativeFrequency("notes/FREQ_ORIG.txt");

	//Equalizando o Histograma 
	myImage->equalizeHistogram(255);
	myImage->computeHistogram(GREYSCALE);
	myImage->writeHistogram("notes/equalizada.txt");
	//myImage->cumulativeFrequency("notes/FREQ_EQUAL.txt", img_buffer);

	myImage->writeImage("images/equalizada.bmp");

	delete myImage;

	std::cout << "main\n";
	return 0;
}