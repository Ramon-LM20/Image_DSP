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

	std::vector<float>graph;	
	std::vector<unsigned char> img_buffer;	
	std::vector<unsigned char> freq_buffer;

	imageDSP* myImage = new imageDSP(imageName2); //Cria o objeto na heap passando apenas o path para o construtor

	myImage->readImage();
	myImage->copyImageData(img_buffer);
	myImage->brigthnessControl(-150, img_buffer);
	myImage->writeImage("images/imageDarker.bmp", img_buffer);
	
	//Criando o Histograma foto original escurecida
	myImage->computeHistogram(img_buffer, graph, GREYSCALE);
	myImage->writeHistogram("notes/original.txt", graph);
	//myImage->cumulativeFrequency("notes/FREQ_ORIG.txt", img_buffer);

	//Equalizando o Histograma 
	myImage->equalizeHistogram(img_buffer, 255);
	myImage->computeHistogram(img_buffer, graph, GREYSCALE);
	myImage->writeHistogram("notes/equalizada.txt", graph);
	//myImage->cumulativeFrequency("notes/FREQ_EQUAL.txt", img_buffer);

	myImage->writeImage("images/equalizada.bmp", img_buffer);

	delete myImage;

	std::cout << "main\n";
	return 0;
}