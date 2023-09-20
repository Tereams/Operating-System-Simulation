#include<stdio.h>
#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

//已分配分区管理可以采用结构数组实现
typedef  struct  AllocMem
{
	int  startAddress;			//分区开始地址
	int  size;					//分区大小
	int  processID;			//程序号
	
};

//空闲分区管理可以采用数组或链表实现
typedef  struct  FreeMem
{
	int  startAddress;			//空闲区开始地址
	int  size;					//空闲区大小
	int  state;				//空闲区状态：0为空表目，1为可用空闲区
} ;


/* 分配主存空间的函数，参数为：进程号，申请空间量
分配成功，返回分配区的起始地址，不成功：返回-1   */
int  allocate(vector<FreeMem>& freeBlock, vector<AllocMem>& allocBlock, int processID, int applySize) {

	for (int i = 0; i < freeBlock.size(); i++) {//遍历已经排序排好的空闲分区表
		if (freeBlock[i].size >= applySize) {//如果有满足条件的空闲分区
			
			AllocMem newblock;//创建分配表项
			newblock.processID = processID;//进程号为输入的进程号
			newblock.size = applySize;//大小为申请的大小
			newblock.startAddress = freeBlock[i].startAddress;//起始地址为空闲分区的起始地址
			
			allocBlock.push_back(newblock);//放入已分配分区表

			int remain_size = freeBlock[i].size - applySize;//判断是否还有空间剩余
			if (remain_size > 0) {//若有剩余空间，就将剩余空间放回空闲分区表
				
				FreeMem remainblock;//创建空闲表项
				remainblock.size = remain_size;
				remainblock.state = 1;
				remainblock.startAddress = freeBlock[i].startAddress + applySize;//新的起始地址为原先的起始地址加上分配的大小
				
				freeBlock.push_back(remainblock);//新的表项放入空闲分区表，后续adjust函数会对其进行排序
			}
			freeBlock.erase(freeBlock.begin() + i);//将原有的空闲分区项删除
			return newblock.startAddress;//若完成上述步骤，则空间分配成功，返回分配的地址
		}
	}
	return -1;//若遍历整个空闲分区表还未找到能够分配的空闲分区，则分配失败，返回-1；
}




//主存回收函数，参数为进程号
void  setfree(vector<FreeMem>& freeBlock, vector<AllocMem>& allocBlock, int processID) {
	
	//遍历整个已分配的数组，找到待删除的进程号
	for (int i = 0; i < allocBlock.size(); i++) {
		if (allocBlock[i].processID == processID) {//如果找到相匹配的进程号
			//首先创建空闲分区项，假设其不存在上下邻接的情况进行初始化，后续再根据实际的上下邻接情况进行扩展
			FreeMem newblock;
			newblock.size = allocBlock[i].size;//没有上下邻接的情况时，空闲项的大小是将要释放的分区大小
			newblock.startAddress = allocBlock[i].startAddress;//没有上下邻接的情况时，空闲项的起始地址是将要释放的分区起始地址
			newblock.state = 1;

			for (int j = 0; j < freeBlock.size(); j++) {//遍历空闲分区表，判断是否存在上下邻接
				//判断上邻接
				if (freeBlock[j].startAddress + freeBlock[j].size == allocBlock[i].startAddress) {
					newblock.startAddress = freeBlock[j].startAddress;//若存在上邻接，则将起始地址改为上邻接的块的地址
					newblock.size += freeBlock[j].size;//空间加上上一个空闲块的大小
					freeBlock.erase(freeBlock.begin() + j);//删除已经被合并的空闲块
					j--;//由于上一行代码删除了线性表中的一个元素，其后的元素会前移，此行代码是为了保证循环可以正确的进行
				}
				//判断下邻接
				else if (freeBlock[j].startAddress == allocBlock[i].startAddress + allocBlock[i].size) {
					newblock.size += freeBlock[j].size ;//若存在下邻接，就加上下一个空闲块的大小
					freeBlock.erase(freeBlock.begin() + j);//删除已经被合并的空闲块
					j--;//由于上一行代码删除了线性表中的一个元素，其后的元素会前移，此行代码是为了保证循环可以正确的进行
				}
			}
			freeBlock.push_back(newblock);//新的空闲分区项加入表

			//从分配表中将相应的进程删除
			allocBlock.erase(allocBlock.begin() + i);
			return;
		}
	}
	cout << "进程号："<<processID<<"不存在！"<<endl;//如果没有通过if分支返回，则说明没有对应的进程号，输出错误信息
	return;
}

//空闲分区表中的空闲区调整的函数
//按照不同的算法进行调整
//最佳适应算法，按空闲区大小从小到大排列
bool mycmp(FreeMem a, FreeMem b) {//自定义FreeMem的排序函数
	return a.size < b.size;
}
void  adjust(vector<FreeMem>& freeBlock) {
	//使用algorithm库函数进行排序，使用自定义的结构体排序函数，从小到大排列
	sort(freeBlock.begin(), freeBlock.end(), mycmp);
}


//输出空闲分区和已分配分区信息，包括分区号、起始地址、分区大小、（进程号）等
void   print(vector<FreeMem>  freeBlock, vector<AllocMem> allocBlock) {
	cout << "空闲区域表" << endl;
	for (int i = 0; i < freeBlock.size(); i++) {
		cout << "size: " << freeBlock[i].size;
		cout << " address: " << freeBlock[i].startAddress;
		cout << " state: " << freeBlock[i].state<<endl;
	}
	cout << "分配区域表" << endl;
	for (int i = 0; i < allocBlock.size(); i++) {
		cout << "size: " << allocBlock[i].size;
		cout << " address: " << allocBlock[i].startAddress;
		cout << " pid: " << allocBlock[i].processID << endl;
	}
}

int  main()		//主函数
{

	vector<FreeMem>  freeBlock;		//空闲分区表
	vector<AllocMem> allocBlock;    //已分配分区表
 
	//输入内存容量；
	cout << "请输入内存容量：";
	int memSize;
	std::cin >> memSize;

	//对空闲分区表进行初始化，初始条件下，空闲分区表中应该只有一条记录
	FreeMem initblock;
	initblock.size = memSize;//初始大小应与内存的大小相同
	initblock.startAddress = 0;//初始记录的起始地址默认为0
	initblock.state = 1;//初始状态设置为可用空闲区

	freeBlock.push_back(initblock);

	print(freeBlock,allocBlock);  //输出分区信息

	char t;
	while (1)
	{
		int pid = 0;
		int size = 0;

		//输入操作码
		cout << "请输入操作码：";
		cin >> t;
		switch (t)
		{
		case 'p':
			//进程申请内存空间
			//输入进程信息和内存申请量，判断是否合理
			cout << "请输入进程号：";
			cin >> pid;
			cout << "请输入要分配的空间大小：";
			cin >> size;

			//做一个初步的判断，如果申请的空间比内存空间还大，就一定会申请失败
			//只是初步的判断
			if (size > memSize) {
				cout << "申请空间过大！";
			}

			else {

				allocate(freeBlock, allocBlock, pid, size);  //为进程分配内存
				adjust(freeBlock);//按照不同的算法进行调整，最佳适应算法，按空闲区大小从小到大排列
				print(freeBlock, allocBlock);  //输出分区信息
			}
			break;
		case 'r':
			//作业回收
			cout << "请输入进程号：";
			cin >> pid;
			setfree(freeBlock, allocBlock, pid);//主存回收函数
			adjust(freeBlock);//按照不同的算法进行调整，最佳适应算法，按空闲区大小从小到大排列
			print(freeBlock, allocBlock);  //输出分区信息
			break;
		case 'l':
			print(freeBlock, allocBlock);  //输出分区信息
			break;
		case 'q':
			// 结束程序运行
			return 0;
		}
	}
}