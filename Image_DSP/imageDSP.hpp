#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <array>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>

static const int BMP_HEADER_SIZE = 54;
static const int BMP_COLOR_TABLE_SIZE = 1024;
static const int _512by512_IMG_SIZE = 262144;
static const int GREYSCALE = 256;

class imageDSP 
{
protected:
	std::string imgName;
	std::string outImgName;

	int m_width;
	int m_height;
	int m_bitDepth;
	int edge_elements[2]; //array para salvar o pixel mais claro e o pixel mais escuro da imagem

	unsigned char* ptr_header;
	unsigned char* ptr_colorTable;
	unsigned char* inBuff;
	unsigned char* outBuff;

	std::array<char, 54> header{};
	std::array<char, 1024> colorTable{};
	std::vector<unsigned char> buffer{};



public: 

	imageDSP(std::string _imgName);
	~imageDSP();
	void readImage();
	void writeImage(std::string _newName, std::vector<unsigned char>& _writeBuf);
	void copyImageData(std::vector<unsigned char>& _destBuff);
	int getWidth();
	int getHeight();
	int getbitDepth();
	long int get_imgSize();
	void brigthnessControl(int _brightness_threshold, std::vector<unsigned char> &_outBuf);
	void computeHistogram(std::vector<unsigned char>& _inputData, std::vector<float>& _histogram, int _max_val);
	void cumulativeFrequency(std::string _fileName, std::vector<unsigned char> frequency);
	bool writeHistogram(std::string _fileName, std::vector<float> _data);
	void equalizeHistogram(std::vector<unsigned char>& _inputData, int max_val);
};

