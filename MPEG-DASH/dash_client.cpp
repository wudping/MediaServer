#include "http_client.hpp"
#include <iostream>
#include "opencv2/opencv.hpp"
//#include "http_server.hpp"
#include <string>
#include <unistd.h>
#include <fstream>
using namespace std;
using namespace cv;
// how to run :g++ -o test_client test_client.cpp `pkg-config opencv --cflags --libs` -std=c++11
// g++ http_client.cpp test_client.cpp -o client -o test_client `pkg-config opencv --cflags --libs` -std=c++11
int main() {

	{
		HttpClient client(6724, "localhost"); 
		client.get("/avengers_trailer.m3u8", "master.m3u8");
	}
	cout << "\nmaster fetched\n" << flush;
	Mat frame; 
	ifstream master_playlist("master.m3u8",ios::in);
	ifstream the_720_file;
	ifstream the_480_file;
	string a;
	string the_720_name;
	string the_480_name;
	bool the_720_open = false;
	bool the_480_open = false;
	while(! master_playlist.eof())
	{
		master_playlist>>a;
		if(a.length()>=4)
		{
			if(a.substr(a.length()-4,3) == "720")
				master_playlist>>the_720_name;
			else if(a.substr(a.length()-4,3) == "480")
				master_playlist>>the_480_name;
		}
	}
	master_playlist.close();
	cout << "\nmaster parsed\n" << flush;
	cout << the_720_name << the_480_name << "are the names\n" << flush;
	the_720_name = "/" + the_720_name;
	the_480_name = "/" + the_480_name;
	string choice;
	cout << "\nPlay in 720p or 480p? : ";
	cin >> choice;
	cout << "\nchoice is : " << choice << "\n" << flush;
	//cout << "\nPress 7 or 4 to switch to 720p or 420p anytime\n";
	if(choice.substr(0,3) == "720")
	{
		HttpClient client(6724, "localhost"); 
		client.get(the_720_name.c_str(), "the_720.m3u8");
		the_720_file.open("the_720.m3u8",ios::in);
		the_720_open = true;
		cout<<"\nFetched 720p playlist : " << the_720_name << "\n" << flush;
	}
	else
	{
		HttpClient client(6724, "localhost"); 
		client.get(the_480_name.c_str(), "the_480.m3u8");
		the_480_file.open("the_480.m3u8");
		the_480_open = true;
		cout<<"\nFetched 480p playlist : " << the_480_name << "\n" << flush;
	}
	cout << "\nplaylist fetched and running\n" << flush;
	float cur_duration = 0;
	while(1)
	{
		cout << "\n Getting cur file... " << cur_duration << "\n" << flush;
		string cur_play_file;
		float duration_passed = 0;
		float duration_of_cur_file = 0;
		if(choice.substr(0,3) == "720")
		{
			the_720_file.close();
			the_720_file.open("the_720.m3u8");
			the_720_file >> cur_play_file;
			while(!the_720_file.eof())
			{
				cout << "\nAccessing 720p playlist\n" << endl;
				if( cur_play_file.substr(0,8) == "#EXTINF:")
				{
					
					duration_of_cur_file = atof((cur_play_file.substr(8, cur_play_file.length()-10)).c_str());
					cout << "\n Duration here is : " << duration_of_cur_file << "\n"<< flush;
					duration_passed += duration_of_cur_file;
				}
				if(duration_passed > cur_duration)
				{
					the_720_file >> cur_play_file;
					break;
				}
				the_720_file >> cur_play_file;
			}
			if( the_720_file.eof() && duration_passed <= cur_duration)
				break;
		}
		else
		{
			the_480_file.close();
			the_480_file.open("the_480.m3u8");
			the_480_file >> cur_play_file;
			while(!the_480_file.eof())
			{
				cout << "\nAccessing 480p playlist\n" << endl;
				if( cur_play_file.substr(0,8) == "#EXTINF:")
				{
					duration_of_cur_file = atof((cur_play_file.substr(8, cur_play_file.length()-3)).c_str());
					duration_passed += duration_of_cur_file;
				}
				if(duration_passed > cur_duration)
				{
					the_480_file >> cur_play_file;
					break;
				}
				the_480_file >> cur_play_file;
			}
			if( the_480_file.eof() && duration_passed <= cur_duration)
				break;
		}
		duration_passed -= duration_of_cur_file;
		//cur_play_file = "/" + cur_play_file;
		cout<<"\n Fetching the video segment " << cur_play_file << "\n" << flush;
		{
			HttpClient client(6724, "localhost"); 
			client.get("/" + cur_play_file, cur_play_file);
		}
		VideoCapture cap(cur_play_file); 
		if(!cap.isOpened())
		{
			cout << "Error opening video stream" << endl; 
			return -1; 
		}
		int frame_width = cap.get(CV_CAP_PROP_FRAME_WIDTH); 
		int frame_height = cap.get(CV_CAP_PROP_FRAME_HEIGHT); 	
		int fps = cap.get(CV_CAP_PROP_FPS);
		while(duration_passed < cur_duration)
		{
			cap >> frame;
			duration_passed += 1000.0/fps;

		}
		while(1)
		{
			cap >> frame;	
			if(!frame.empty())
			{
				imshow( "Frame", frame );
				cur_duration += 1.0/fps;
				char c = (char)waitKey(1000.0/fps);
				if( c == '7')
				{
					choice = "720";
					if(the_720_open == false)
					{
						HttpClient client(6724, "localhost"); 
						client.get(the_720_name.c_str(), "the_720.m3u8");
						the_720_file.close();
				    	the_720_file.open("the_720.m3u8");
				    	the_720_open = true;
				    	break;
					}
				}
				else if( c == '4')
				{
					choice = "480";
					if(the_480_open == false)
					{
						HttpClient client(6724, "localhost"); 
						client.get(the_480_name.c_str(), "the_480.m3u8");
						the_480_file.close();
				    	the_480_file.open("the_480.m3u8");
				    	the_480_open = true;
				    	break;
					}
				}
			}
			else
				break;
		}
		cout << "\n Getting Next file...\n" << flush;
		//cap.release();  
	}  
	destroyAllWindows();
	return 0;
} 

