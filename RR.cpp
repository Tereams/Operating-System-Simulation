#include<queue>
#include<iostream>
using namespace std;

typedef   struct  pcb		//进程控制块定义
{
	int  pid;         //进程ID
	int  runtime;			//运行时间
	int  arrivetime;		//到达时间
	int finishtime;  //进程结束时间
	int neededtime;  //进程运行还需要的时间
	char processstate;//进程状态

}  PCB;

unsigned long current;      //记录系统当前时间的变量
queue<PCB>  inputprocess();  //对任务表进行初始化，任务表：即记录任务何时到达以及需要运行多长时间的表


void showinit(queue<PCB> ans) {//打印各个PCB的初始状态，由于是初始状态，所以只需要展示pid、运行时间、到达时间、运行状态即可
	PCB temp;
	cout << "<--------------PCB初始状态---------------->"<<endl;
	while (!ans.empty()) {//输出执行的结果
		temp = ans.front();
		ans.pop();
		double mytime;
		cout << "pid: " << temp.pid << "--";//输出pid
		cout <<"运行时间: " << temp.runtime<<"--";//输出运行时间
		cout << "到达时间: " << temp.arrivetime<<"--";//输出到达时间
		cout << "运行状态: " << temp.processstate << endl;//输出运行状态
	}
}

void showready(queue<PCB> ans,int t) {//打印就绪队列的内容，只展示pid
	PCB temp;
	cout << "<--------------时间点t="<<t<<"---------------->" << endl;
	cout << "当t=" << t<< "时，就绪队列的状态为" << endl;
	cout << "pid: ";
	while (!ans.empty()) {//输出执行的结果
		temp = ans.front();
		ans.pop();
		double mytime;
		cout <<temp.pid << "-";//输出pid
	}
	cout << endl;
}

void showprocess(PCB a) {//打印某一个进程的状态
	cout << "执行结束后，该进程块的状态为：" << endl;
	cout << "pid: " << a.pid <<" ";//输出pid
	cout << "运行时间: " << a.runtime <<" ";//输出运行时间
	if (a.neededtime > 0) {//若还未执行完毕
		cout << "到达时间: " << a.arrivetime << " ";//输出到达时间;
		cout << "还需时间: " << a.neededtime << " ";//输出还需要的时间
		cout << "运行状态: " << a.processstate << endl;//输出运行状态
	}
	else {//若已经执行完毕
		cout << "到达时间: " << a.arrivetime << " ";//输出到达时间;
		cout << "完成时间: " << a.finishtime << " ";//输出完成时间
		cout << "运行状态: " << a.processstate << endl;//输出运行状态
	}
	
}

void showans(queue<PCB> ans) {//打印程序最后执行的结果，输出周转时间与平均周转时间
	PCB temp;
	cout << "<--------------执行结果---------------->" << endl;
	int sum1=0;
	double sum2 = 0;
	while (!ans.empty()) {//输出执行的结果
		temp = ans.front();
		ans.pop();
		double mytime;
		cout<< "pid" << temp.pid<<" ";//输出pid
		cout<< "周转时间" << temp.finishtime - temp.arrivetime<<" ";//输出周转时间
		mytime = (double)(temp.finishtime - temp.arrivetime) / temp.runtime;//输出平均周转时间
		cout<< "带权周转时间" << mytime<<endl;
		sum1 += temp.finishtime - temp.arrivetime;
		sum2 += mytime;
	}
	cout << "平均周转时间" << (double)sum1/5 << " " << "平均带权周转时间" << (double)sum2 / 5 << endl;;
}


void main()		//主函数，进程的调度直接在主函数中进行
{  	

	current = 1;//保存时间片的序号，时间点是从0开始计数的，时间片是从1开始计数的
	queue<PCB> myprocess;//初始化一个队列，用作任务表；任务表：即记录任务何时到达以及需要运行多长时间的表
	myprocess=inputprocess();
	showinit(myprocess);
	queue<PCB> ready;//初始化一个队列，用作就绪队列
	queue<PCB> ans;//初始化一个队列，用于保存结果
	PCB temp,todo;//临时变量

	//就绪队列初始化，将任务表中的第一项任务取出并且放入就绪队列
	temp = myprocess.front();
	ready.push(temp);
	myprocess.pop();

	//进程调度的开始
	while (true) {
		if (!ready.empty()) {//循环终止的条件，就绪队列（不是任务表队列）为空
			
			//打印就绪队列
			showready(ready,current-1);
			//取出队首任务
			todo = ready.front();
			cout << "在第"<<current<<"个时间片内，要执行的进程的进程号为：" << todo.pid << endl;
			ready.pop();

			//判断任务表中是否有任务到达，若有到达的任务，将该任务加入就绪队列
			//判断任务表是否为空
			if(!myprocess.empty()){
				temp = myprocess.front();
			}
			//任务表不为空是取队首任务的到达时间进行判断，若到达，则加入就绪队列
			if (temp.arrivetime == current) {
				ready.push(temp);
				myprocess.pop();
			}

			//若就绪队列的队首任务（前面pop出来的那个）在时间片后未执行完毕，就重新push回就绪队列去，等待执行
			todo.neededtime--;//当前任务的所需时间减1
			if (todo.neededtime > 0) {
				ready.push(todo);
			}
			//如果任务执行完毕，就将任务完成时的时间保存的finishtime中，并将其对应的PCB保存起来
			else {
				todo.finishtime = current;
				todo.processstate = 'C';
				ans.push(todo);
			}

			showprocess(todo);
			current++;
		}

		else {//循环终止的条件，就绪队列（不是任务表队列）为空
			break;
		}		
	}
	showans(ans);
	
}


queue<PCB>  inputprocess()
{   //进程任务表的初始化，初始化进程的到达时间以及执行完成所需要的时间
	queue<PCB> process;
	PCB p1,p2,p3,p4,p5;

	p1.pid = 1;
	p1.runtime = 3;
	p1.arrivetime = 0;
	p1.finishtime = 0;
	p1.processstate = 'R';
	p1.neededtime = p1.runtime;//还需要的时间初始与完成时间相同

	p2.pid = 2;
	p2.runtime = 6;
	p2.arrivetime = 2;
	p2.finishtime = 0;
	p2.processstate = 'R';
	p2.neededtime = p2.runtime;

	p3.pid = 3;
	p3.runtime = 4;
	p3.arrivetime = 4;
	p3.finishtime = 0;
	p3.processstate = 'R';
	p3.neededtime = p3.runtime;

	p4.pid = 4;
	p4.runtime = 5;
	p4.arrivetime = 6;
	p4.finishtime = 0;
	p4.processstate = 'R';
	p4.neededtime = p4.runtime;

	p5.pid = 5;
	p5.runtime = 2;
	p5.arrivetime = 8;
	p5.finishtime = 0;
	p5.processstate = 'R';
	p5.neededtime = p5.runtime;

	process.push(p1);
	process.push(p2);
	process.push(p3);
	process.push(p4);
	process.push(p5);

	return process;
}

