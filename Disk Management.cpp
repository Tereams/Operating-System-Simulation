#include<stdio.h>
#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

//文件信息表
typedef  struct  File
{
	char  name;			//文件名称
	int  size;					//文件大小
	vector<int> alloc;			//文件分配的相对磁盘号

};




/* 分配主存空间的函数，参数为：进程号，申请空间量
分配成功，返回分配区的起始地址，不成功：返回-1   */
int  allocate(vector<File>& filelist, int allocBlock[][16], int name, int applySize) {

	int toalloc = applySize;
	vector<int> alloc;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 16; j++) {
			//遍历位示图，找到没有分配的空间
			if (allocBlock[i][j] == 0) {
				alloc.push_back(i* 16 + j);
				toalloc--;
				allocBlock[i][j] = 1;
			}
			//分配足够多的空间后退出循环
			if (toalloc == 0) break;
		}
		if (toalloc == 0) break;
	}
	if (toalloc == 0) {
		//退出循环时，如果待分配的空间数为0，则说明分配已经结束，是因为toalloc=0而退出循环
		//生成有关的表项，加入文件列表
		File newfile;
		newfile.name = name;
		newfile.size = applySize;
		newfile.alloc = alloc;
		filelist.push_back(newfile);
		return 1;
	}
	else {
		//否则是因为遍历结束而退出循环，说明内存空间不足
		cout << "磁盘空间不足"<<endl;
		return -1;
	}
	return -1;
}	




//磁盘空间回收函数，参数为文件名
void  setfree(vector<File>& filelist, int map[][16], int name) {

	//遍历整个文件列表，找到待删除的文件名
	for (int i = 0; i < filelist.size(); i++) {
		if (filelist[i].name == name) {//如果找到相匹配的文件名
			for (int j = 0; j < filelist[i].alloc.size(); j++) {
				int no = filelist[i].alloc[j];
				//计算位示图中待删除的相应位置
				int x = no / 16;
				int y = no % 16;
				map[x][y] = 0;
			}
			filelist[i].alloc.clear();
			filelist.erase(filelist.begin() + i);
		}
	}
		return;
}




//输出文件列表和位示图
void   print(vector<File>  filelist,int map[][16]) {
	cout << "位示图" << endl;
	cout << "----------------" << endl;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 16; j++) {
			cout << map[i][j] << " ";
		}
		cout << endl;
	}
	cout << "文件列表" << endl;
	for (int i = 0; i < filelist.size(); i++) {
		cout << "name: " << filelist[i].name;
		cout << " size: " << filelist[i].size<<endl;
		cout << "----------------" << endl;
		for (int j = 0; j < filelist[i].alloc.size(); j++) {
			int no = filelist[i].alloc[j];
			int zhu = no / 16;
			int tou = no%16 / 4;
			int hao = no % 16 % 4;
			cout << " 相对磁盘号: " <<no<< " ";
			cout << "CHS：（" << zhu << "，" << tou <<"，" << hao<<")" << endl;
		}
		
	}
}

int  main()		//主函数
{
	//文件分配列表
	vector<File>  filelist;		
	//位示图
	int map[8][16];

	//位示图初始化，第一行默认被占用,其他区域空闲
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 16; j++) {
			map[i][j] = 0;
		}
	}
	for (int i = 0; i < 16; i++)
		map[0][i] = 1;
	print(filelist, map);

	char t;
	while (1)
	{
		char name = 0;
		int size = 0;

		//输入操作码
		cout << "请输入操作码：";
		cin >> t;
		switch (t)
		{
		case 'p':
			//进程申请磁盘空间
			//输入文件信息和磁盘申请量，判断是否合理
			cout << "请输入文件名：";
			cin >> name;
			cout << "请输入要分配的磁盘空间大小：";
			cin >> size;

			//做一个初步的判断，如果申请的空间比磁盘空间还大，就一定会申请失败
			//只是初步的判断
			if (size > 8*16) {
				cout << "申请空间过大！";
			}

			else {
				allocate(filelist, map, name, size);  //为文件分配磁盘空间
				print(filelist,map);  //输出文件信息
			}
			break;
		case 'r':
			//文件回收
			cout << "请输入文件名称：";
			cin >> name;
			setfree(filelist, map, name);//磁盘空间回收函数
			print(filelist, map);  //输出文件信息
			break;
		case 'l':
			print(filelist, map);  //输出文件信息
			break;
		case 'q':
			// 结束程序运行
			return 0;
		}
	}
}