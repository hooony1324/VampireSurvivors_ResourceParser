#include <iostream>
#include <fstream>
#include "json/json.h"
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

void ResourceParse(string& Addr, string& dstAddr);
void ImageCutToBmp(string& addr, string& name, int x, int y, int w, int h, string& dstAddr);

struct RscObj
{
	string Addr;
	string folderName; // Result�� ����� �����̸�(������ �̸� ������ �ξ�� ��)
};

// ���� : json����, json������ ��� png, Result�����ȿ� ��������(folderName�� ���ƾ���)
int main(int ac, char** av)
{

	vector<RscObj> vector;
	//vector.push_back(RscObj({ "../APIResources/img/characters" , "characters" }));
	//vector.push_back(RscObj({ "../APIResources/img/enemies" , "enemies" }));
	//vector.push_back(RscObj({ "../APIResources/img/illustrations" , "illustrations" }));
	//vector.push_back(RscObj({ "../APIResources/img/items" , "items" }));
	//vector.push_back(RscObj({ "../APIResources/img/UI" , "UI" }));
	//vector.push_back(RscObj({ "../APIResources/img/vfx" , "vfx" }));
	//vector.push_back(RscObj({ "../APIResources/tilesets/LibraryTexturePacked" , "tilesets" }));

	for (RscObj obj: vector)
	{
		string dstAddr = "../APIResourceParsed/" + obj.folderName + "/";
		ResourceParse(obj.Addr, dstAddr);
	}

	return 0;
}

void ResourceParse(string& Addr, string& dstAddr)
{
	string jsonAddr = Addr + ".json";
	string imgAddr = Addr + ".png";

	// json ���� �о ������ ����� �ڸ����� ������ ����
	// ���� �ּ� �ް� ����
	std::ifstream fin;
	fin.open(jsonAddr, std::ios::in);
	
	// json���� �ϴ� ���ڿ��� ����
	std::string str;
	str.assign(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>());

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(str, root) == false)
	{
		cout << "Failed to parse Json : " << reader.getFormattedErrorMessages() << endl;
	}

	// JSON �Ľ�
	Json::Value textures = root["textures"];
	Json::Value frames = (*textures.begin())["frames"];
	for (auto it = frames.begin(); it != frames.end(); ++it)
	{		
		string imgName = (*it)["filename"].asString();
		int x = (*it)["frame"]["x"].asInt();
		int y = (*it)["frame"]["y"].asInt();
		int w = (*it)["frame"]["w"].asInt();
		int h = (*it)["frame"]["h"].asInt();

		ImageCutToBmp(imgAddr, imgName, x, y, w, h, dstAddr);
	}

}

// ���ҽ� �ּ�, �ڸ� ��ġ ���� �ʿ�
void ImageCutToBmp(string& imgAddr, string& name, int x, int y, int w, int h, string& dstAddr)
{
	Mat img = imread(imgAddr);
	Mat rect_img;				// Cut �� �̹���

	// ���� ��ǥ
	int y1 = y;
	int y2 = y + h;
	int x1 = x;
	int x2 = x + w;

	rect_img = img(Range(y1, y2), Range(x1, x2));

	imshow("��� �̹���", rect_img);

	// �̹��� ����
	imwrite(dstAddr + name + ".bmp", rect_img);

	waitKey(0);

}