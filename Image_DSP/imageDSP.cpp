#include "imageDSP.hpp"

imageDSP::imageDSP(std::string _imgName)
{ 
	imgName = _imgName;
	this->readImage();
	this->setBackup();
}
imageDSP::~imageDSP()
{ 

}

void imageDSP::setBackup()
{
	copyImageData(backup_buffer);
}
void imageDSP::brigthnessControl(int _brightness_threshold)
{
	long int sizeBuffer = 0;

	sizeBuffer = this->buffer.size();
	buffer.resize(sizeBuffer);

	if (_brightness_threshold > 0)
	{
		for (int i = 0; i < sizeBuffer; i++)
		{
			int temp = (buffer[i] + _brightness_threshold);

			buffer[i] = (temp > 255) ? 255 : temp;
		}
	}
	if (_brightness_threshold < 0)
	{
		for (int i = 0; i < sizeBuffer; i++)
		{
			int temp = (buffer[i] + _brightness_threshold);

			buffer[i] = (temp < 0) ? 0 : temp;
		}
	}
}
void imageDSP::readImage()
{
	std::ifstream streamIn(imgName, std::ios::in | std::ios::binary);

	if (!streamIn.is_open()) {
		std::cerr << "Unable to open file! [read]" << imgName << "\n";
		exit(0);
	}

	for (auto& i : header)
	{
		i = streamIn.get();
	}

	m_width = *(reinterpret_cast<int*>(&header[18]));
	m_height = *(reinterpret_cast<int*>(&header[22]));
	m_bitDepth = (static_cast<int>(header.at(28)));

	if (m_bitDepth <= 8)
	{
		streamIn.read(colorTable.data(), colorTable.size());
	}
	buffer.resize(m_width * m_height);
	streamIn.read((char*)(&buffer[0]), buffer.size());


	for (std::vector<unsigned char>::iterator it = buffer.begin(); it != buffer.end(); it++)
	{
		if (*it < edge_elements[0]) //used *it (with asterisk), because it's an iterator
		{
			edge_elements[0] = *it;
		}
		if (*it > edge_elements[1])
		{
			edge_elements[1] = *it;
		}
	}

	streamIn.close();
}
void imageDSP::writeImage(std::string _newName)
{
	outImgName = _newName;

	std::ofstream output(outImgName, std::ios::out | std::ios::binary);
	output.write(header.data(), header.size());

	if (this->m_bitDepth <= 8)
	{
		output.write(colorTable.data(), colorTable.size());
	}

	output.write((char*)(&buffer[0]), buffer.size());

	output.close();
}
void imageDSP::copyImageData(std::vector<unsigned char>& _destBuff)
{
	//_destBuff = buffer;
	long int sizeBuffer = buffer.size();
	_destBuff.resize(sizeBuffer);

	for (int i = 0; i < sizeBuffer; i++)
	{
		_destBuff[i] = (unsigned char)buffer[i];
	}

}
int imageDSP::getWidth()
{
	return m_width;
}
int imageDSP::getHeight()
{
	return m_height;
}
int imageDSP::getbitDepth()
{
	return m_bitDepth;
}
long int imageDSP::get_imgSize()
{
	return m_width * m_height;
}
void imageDSP::computeHistogram(int max_val)
{
	int i;
	int value = max_val + 1;
	int img_size = get_imgSize();
	int temp_size = 0;
	std::vector<long int> internalHistogram;

	internalHistogram.resize(value);

	for (i = 0; i < img_size; i++)
	{
		value = buffer.at(i);
		internalHistogram[value] = internalHistogram[value] + 1;
	}

	temp_size = internalHistogram.size();
	m_histogram.resize(temp_size);

	for (i = 0; i < temp_size; i++)
	{
		m_histogram[i] = (float)internalHistogram[i] / (float)img_size;
	}
	
}
void imageDSP::cumulativeFrequency(std::string _fileName)
{
	std::ofstream output(_fileName, std::ios::out | std::ios::binary);
	int n, sum = 0;
	int count = 1;
	long int imgSize = get_imgSize();
	//this->copyImageData(frequency);
	n = buffer.size();

	sort(buffer.begin(), buffer.end());
	if (output.is_open())
	{
		for (int i = 1; i <= n; i++)
		{
			if (i == n or buffer[i] != buffer[i - 1])
			{
				sum += count;
				output << (float)sum / imgSize << "\n";// << (float)frequency[i - 1] << "\n";
				count = 1;
			}
			else
			{
				count++;
			}
		}
	}
	else
		std::cout << "Erro ao abrir arquivo - CumulativeFrequency\n";

	output.close();
}
bool imageDSP::writeHistogram(std::string _fileName)
{
	std::ofstream output(_fileName, std::ios::out | std::ios::binary);
	
	int i = m_histogram.size();
	

	if (output.is_open())
	{
		for (int j = 0; j < i; j++)
		{
			output << m_histogram[j] << "\n";
		}
	}
	else
	{
		std::cout << "Error - WriteFile\n";
		return 0;
	}


	return 1;
}
void imageDSP::equalizeHistogram(int max_val)
{
	int sum = 0;
	int total = get_imgSize();
	int n_bins = max_val+1 ;
	std::vector<int> hist(n_bins, 0);
	std::vector<unsigned char> lut(n_bins, 0);
	

	for (int i = 0; i < total; ++i) {
		hist[buffer[i]]++;
	}

	int i = 0;
	while (!hist[i]) ++i;

	if (hist[i] == total) {
		for (int j = 0; j < total; ++j) {
			buffer[j] = i;
		}
		return;
	}

	float scale = (n_bins - 1.f) / (total - hist[i]);
	
	i++;

	for (; i < hist.size(); ++i) {
		sum += hist[i];
		// the value is saturated in range [0, max_val]
		lut[i] = std::max(0, std::min(int(round(sum * scale)), max_val));
	}

	for (int i = 0; i < total; ++i) {
		buffer[i] = lut[backup_buffer[i]];
	}
}
void imageDSP::flipImage()
{
	std::reverse(buffer.begin(), buffer.end());

	writeImage("images/flipped.bmp");
}
void imageDSP::setNegative()
{

	int size = get_imgSize();

	for (int i = 0; i < size; i++)
	{
		buffer[i] = GREYSCALE - buffer[i];
	}

	writeImage("images/negative.bmp");
}
void imageDSP::rotateClockWise()
{
	int rows = getWidth();
	int columns = getHeight();
}