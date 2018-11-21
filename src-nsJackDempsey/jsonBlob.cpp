/*
 * jsonBlob.cpp
 * Author: Jack Dempsey
 * Date: 10/14/2018
 * Description: This program will take in a single argument,
 * a .jpg file format and in return the user will be given metadata
 * from that jpg file in the form of a json file in the src directory
 * 
 * 
 */


#include <iostream>
#include <fstream>
#include <stdio.h>
#include "exif.h"
using namespace std;

unsigned char *openPic(char *picName){
	// Read the JPEG file into a buffer
  FILE *photo = fopen(picName, "rb");
  if (!photo) {
    printf("Can't open file.\n");
    return NULL;
  }
  fseek(photo, 0, SEEK_END);
  unsigned long fsize = ftell(photo);
  rewind(photo);
  unsigned char *buf = new unsigned char[fsize];
  if (fread(buf, 1, fsize, photo) != fsize) {
    printf("Can't read file.\n");
    delete[] buf;
    return NULL;
  }
  fclose(photo);
  return buf;
  
}
void writeInfo(ofstream *file, easyexif::EXIFInfo *result){
		*file<<"{\n";
		*file<<"\t\"Camera Info\": {\n";
	  *file<<"\t\t\"Camera make\" : \""<<result->Make.c_str()<<"\",\n";
	  *file<<"\t\t\"Camera model\": \""<<result->Model.c_str()<<"\",\n";
	  *file<<"\t\t\"Software\": \""<<result->Software.c_str()<<"\",\n";
	  *file<<"\t\t\"Bits per sample\": "<<result->BitsPerSample<<"\n";
	  *file<<"\t},\n";
	  
	  *file<<"\t\" Image Info:\": {\n";
	  *file<<"\t\t\"Image width\": "<<result->ImageWidth<<",\n";
	  *file<<"\t\t\"Image height\": "<<result->ImageHeight<<",\n";
	  *file<<"\t\t\"Image description\": \""<<result->ImageDescription.c_str()<<"\",\n";
	 *file<<"\t\t\"Image orientation\": \""<<result->Orientation<<"\",\n";
	 *file<<"\t\t\"Image copyright\": \""<<result->Copyright.c_str()<<"\",\n";
	  *file<<"\t\t\"Image date/time\": \""<<result->DateTime.c_str()<<"\"\n";
	  *file<<"\t},\n";
	   
	  *file<<"\t\"Timing Info:\": {\n";
	  *file<<"\t\t\"Original date/time\": \""<<result->DateTimeOriginal.c_str()<<"\",\n";
	  *file<<"\t\t\"Digitize date/time\": \""<<result->DateTimeDigitized.c_str()<<"\",\n";
	  
	  
	  *file<<"\t\t\"Subsecond time\": "<<result->SubSecTimeOriginal.c_str()<<",\n";
	  *file<<"\t\t\"Exposure time\": "<<(unsigned)(1.0 / result->ExposureTime)<<",\n";
	  *file<<"\t\t\"F-stop\": "<<result->FNumber<<",\n";
	  *file<<"\t\t\"Exposure program\": \""<<result->ExposureProgram<<"\",\n";
	  *file<<"\t\t\"ISO speed\": "<<result->ISOSpeedRatings<<",\n";
	  *file<<"\t\t\"Subject distance\": "<<result->SubjectDistance<<",\n";
	  *file<<"\t\t\"Exposure bias\": "<<result->ExposureBiasValue<<"\n";
	  *file<<"\t},\n";
	  
	   *file<<"\t\"GPS Info\": {\n";
	  *file<<"\t\t\"GPS Latitude deg \" : \""<<result->GeoLocation.Latitude<<" deg "<< 
	  result->GeoLocation.LatComponents.degrees<<" min "<< 
	  result->GeoLocation.LatComponents.minutes<<" sec "<< 
	  result->GeoLocation.LatComponents.seconds<<" dir: "<< 
	  result->GeoLocation.LatComponents.direction<<"\",\n";
	  *file<<"\t\t\"GPS Longitude\" : \""<< 
			 result->GeoLocation.Longitude<<" deg "<<result->GeoLocation.LonComponents.degrees<<" min "<<  
			 result->GeoLocation.LonComponents.minutes<<" sec "<< 
			 result->GeoLocation.LonComponents.seconds<<" dir: "<< 
			 result->GeoLocation.LonComponents.direction<<"\",\n";
	  *file<<"\t\t\"GPS Altitude\": "<<result->GeoLocation.Altitude<<",\n";
	  *file<<"\t\t\"GPS Precision (DOP)\": "<<result->GeoLocation.DOP<<"\n";
	  *file<<"\t},\n";
	  
	  
	  *file<<"\t\"Lense Info\": {\n";
	  *file<<"\t\t\"Lens min focal length\": "<<result->LensInfo.FocalLengthMin<<",\n";
	  *file<<"\t\t\"Lens max focal length\": "<<result->LensInfo.FocalLengthMax<<",\n";
	  *file<<"\t\t\"Lens f-stop min\": "<<result->LensInfo.FStopMin<<",\n";
	 *file<<"\t\t\"Lens f-stop max\": "<<result->LensInfo.FStopMax<<",\n";
	  *file<<"\t\t\"Lens make\": \""<<result->LensInfo.Make.c_str()<<"\",\n";
	  *file<<"\t\t\"Lens model\": \""<<result->LensInfo.Model.c_str()<<"\",\n";
	  *file<<"\t\t\"Lens focal length\": "<<result->FocalLength<<",\n";
	  *file<<"\t\t\"35mm focal length\": "<<result->FocalLengthIn35mm<<",\n";
	  *file<<"\t\t\"Focal plane XRes\": "<<result->LensInfo.FocalPlaneXResolution<<",\n";
	  *file<<"\t\t\"Focal plane YRes\": "<<result->LensInfo.FocalPlaneYResolution<<"\n";
	*file<<"\t},\n";
	
	*file<<"\t\"Other Info\" : {\n";
	*file<<"\t\t\"Flash used?\": \""<<result->Flash<<"\",\n";
	  *file<<"\t\t\"Flash returned light\": \""<<result->FlashReturnedLight<<"\",\n";
	  *file<<"\t\t\"Flash mode\": "<<result->FlashMode<<",\n";
	  *file<<"\t\t\"Metering mode\": "<<result->MeteringMode<<"\n";
	  *file<<"\t},\n";

	  *file<<"}";
}

using namespace std;

int main(int argc, char **argv)
{

	//argument handling
	if(argc !=2){
		cout<<"This program only takes a single argument, a .jpg file"<<endl;
		return 0;
	}
	else{
		//check that it is .jpg
		string file = argv[1];
		int size = file.length();
		string filetype = &file[size-4];
		if(filetype != ".jpg"){
			cout<<"need a .jpg file"<<endl;
		}
		else{//good to go
			unsigned char *picInfo = openPic(argv[1]);
			if(!picInfo){
				cout<<"Problem opening pic"<<endl;
				return -1;
			}
			  // Read the JPEG file into a buffer
			  FILE *fp = fopen(argv[1], "rb");
			  if (!fp) {
				printf("Can't open file.\n");
				return -1;
			  }
			  
			  fseek(fp, 0, SEEK_END);
			  unsigned long fsize = ftell(fp);
			  rewind(fp);
			  unsigned char *buf = new unsigned char[fsize];
			  if (fread(buf, 1, fsize, fp) != fsize) {
				printf("Can't read file.\n");
				delete[] buf;
				return -2;
			  }
			  fclose(fp);
			  
			  // Parse EXIF
			  easyexif::EXIFInfo result;
			  int code = result.parseFrom(buf, fsize);
			  delete[] buf;
			  if (code) {
				printf("Error parsing EXIF: code %d\n", code);
				return -3;
			  }
			  
			  //create and write to new file
			  	string fileName = argv[1];
				int size = file.length();
				fileName = fileName.substr(0, fileName.size()-4);
			ofstream file;
			fileName = string(fileName) + ".json";
			file.open (fileName);
			writeInfo(&file, &result);
			
			file.close();
			cout<<"New json data can be found in: src/"<<fileName<<endl;
			  
			  
			
			
		}
	}
	
	return 0;
}

