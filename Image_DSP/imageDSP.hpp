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

	std::array<char, 54> header;
	std::array<char, 1024> colorTable;

	std::vector<unsigned char> buffer;
	std::vector<unsigned char> backup_buffer;

	std::vector<float> m_histogram;


public: 

	imageDSP(std::string _imgName);
	~imageDSP();
	void readImage();
	void writeImage(std::string _newName);
	void copyImageData(std::vector<unsigned char>& _destBuff);
	void setBackup();
	int getWidth();
	int getHeight();
	int getbitDepth();
	long int get_imgSize();
	void brigthnessControl(int _brightness_threshold);
	void computeHistogram(int _max_val);
	void cumulativeFrequency(std::string _fileName);
	bool writeHistogram(std::string _fileName);
	void equalizeHistogram(int max_val);
	void rotateClockWise();
};

