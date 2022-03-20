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
	string folderName; // Result에 저장될 폴더이름(폴더는 미리 생성해 두어야 함)
};

// 조건 : json파일, json파일의 대상 png, Result폴더안에 목적폴더(folderName과 같아야함)
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

	// json 파일 읽어서 사진을 어디어디 자를지의 정보를 생성
	// 파일 주소 받고 오픈
	std::ifstream fin;
	fin.open(jsonAddr, std::ios::in);
	
	// json정보 일단 문자열로 받음
	std::string str;
	str.assign(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>());

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(str, root) == false)
	{
		cout << "Failed to parse Json : " << reader.getFormattedErrorMessages() << endl;
	}

	// JSON 파싱
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

// 리소스 주소, 자를 위치 정보 필요
void ImageCutToBmp(string& imgAddr, string& name, int x, int y, int w, int h, string& dstAddr)
{
	Mat img = imread(imgAddr);
	Mat rect_img;				// Cut 된 이미지

	// 영역 좌표
	int y1 = y;
	int y2 = y + h;
	int x1 = x;
	int x2 = x + w;

	rect_img = img(Range(y1, y2), Range(x1, x2));

	imshow("결과 이미지", rect_img);

	// 이미지 저장
	imwrite(dstAddr + name + ".bmp", rect_img);

	waitKey(0);

}