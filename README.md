# 2020FALL_DS-Transportation-system
Transportation System   
# 2020秋期末设计——交通查询系统

## 1.需求分析

1. 实现对城市信息的添加和删除；隐藏要求为删除城市时也要将相关班次信息删除；
2. 实现城市间班次信息的添加和删除；班次信息至少包括起始站和终点站、出发时间、到达时间、票价、载具种类（飞机/火车）；
3. 输入城市名，打印该城市出发的所有班次；追加实现了打印城市相关班次的所有信息，即 出发+到达；
4. 确定起始站、终点站、交通工具，计算最快到达和最省钱到达的决策；
5. 在第4点基础上，实现了只需起始站和终点站而无需指定交通工具的决策计算，使路线更加灵活；
6. 默认所有行程需在一天内结束；若考虑行程可能不在一天内结束，则需对其中一个函数进行调整，在下面的部分进行详细说明；

## 2.概要设计

### clock类

用clock类表示各个班次的时钟时间，包括成员函数display()、默认构造函数、复制构造函数等，对错误输入有处理能力；

### shift类

用shift类表示班次信息，包括载具类型、出发地、终点站、出发时间、到达时间、价格等，重载操作符==，成员函数print()输出班次信息；

### city类

使用string保存城市名；

vector< shift> train_departure: 从此出发的火车班次

vector< shift> train_arrival: 到达此处的火车班次

vector< shift> plane_departure: 从此出发的飞机班次

vector< shift> plane_arrival: 到达此处的飞机班次void

### 全局变量

queue< shift> que : 计算最优决策时保存路径；

int Min: 计算最优决策时保存最小代价；

vector< city> City :保存城市节点；

map< string, int> city_num : 将每个城市名对应不同的int类型编号；

int TAG[50] ：计算最优决策时记录各个城市是否访问过，默认城市数量<=50；

### void refresh_city()

在每次计算最优决策前，刷新相关辅助变量、数组的状态；

### void add_city() & void add_city(string s)

函数重载，功能都为添加空白城市节点；

add_city()的城市名在内部输入，在main()函数中调用；

add_city(string s)在其他函数中调用，更方便.

### void delete_city()

在main()函数中调用.

由于在各城市节点中既保存了出发班次，又保存了到达班次，所以delete_city除了删除城市节点外，还需在其他城市节点中删除所有相关的班次信息.

### void add_shift()

添加班次信息.找到相关的两个城市节点分别添加；如果城市节点不存在，则顺便添加城市节点.

### void delete_shift()

同样，要分别在相关的两个城市中删除班次信息。如果城市不存在/班次不存在，输出错误信息.

### int time_gap(clock a,clock b)

计算两个clock变量的差值，转化成分钟数输出；

### clock min2clock(int min)

将分钟数转化为时钟时间输出；

### void print()

输入城市名，输出所有相关班次时刻表；

若城市不存在，输出错误信息.

### void economical()

在main()函数中调用，作为计算最省钱决策的入口.

输入出发地、终点站、载具类型；若城市不存在，输出错误信息。

调用 eco_train() 或eco_plane 计算最省钱决策：只对eco_train 进行说明.

### bool eco_train(string from,string to,int cost,queue< shift> Q,clock clk)

由于班次有时间先后、载具类型等限制，所以采取了便于实现的深度优先搜索。

用递归的方式尝试所有满足条件的班次序列，当可以到达终点站时与已经记录的Min 比较。若代价更低，则用 cost和 Q 更新Min与que；

每次递归过程中，将当前城市作为from，目标城市作为 to，cost+下个班次的票价，以及 下个班次入队 ，下个班次的到达时间作为clk ，这些作为参数进入下次递归；

进行条件：下个班次的开始时间在clk之后；下个班次的到达城市没有被访问过.

### void fast()

与 economical()类似，作为计算最快路径的入口，调用fast_train() 和 fast_plane();

### bool fast_train(string from,string to,int cost,queue< shift> Q,clock clk)

与eco_train() 原理相同。不同的是每次作为参数的cost 为 cost+等待时间+下个班次时间，代表已用时间.

### void superfast()

不限制载具类型，计算最快决策的入口，

调用 fast_s()进行递归计算，原理相同；

### void supercheap()

不限制载具类型，计算最省钱决策的入口，调用cheap_s()进行递归计算，原理相同；

### void cities()

输出当前所有记录的城市节点，不含相关班次信息.

### int main()

通过menu选择功能；输入为11时结束程序.

## 3.源程序

随报告提交pj.cpp ,

测试输入、输出文件为 input.txt 、 output.txt. 

## 4.调试分析

使用文件输入/输出，便于调试.

### 添加/删除城市

![1609221321(1)](C:\Users\39470\Desktop\运行截图\1609221321(1).png)

5-打印所有城市节点。可以看到删除testcity成功

![1609221602(1)](C:\Users\39470\Desktop\运行截图\1609221602(1).png)



![1609221835(1)](C:\Users\39470\Desktop\运行截图\1609221835(1).png)

### 添加班次信息

![1609223936(1)](C:\Users\39470\Desktop\运行截图\1609223936(1).png)

### 打印时刻表

![1609222726(1)](C:\Users\39470\Desktop\运行截图\1609222726(1).png)

北京+上海 两个城市

![1609222812(1)](C:\Users\39470\Desktop\运行截图\1609222812(1).png)

### 计算决策

使用深度优先搜索，时间复杂度为O（n）,最差情况要遍历所有节点、尝试所有路径.

![1609224042(1)](C:\Users\39470\Desktop\运行截图\1609224042(1).png)

#### 最快路径

##### 指定交通工具

合肥->重庆  火车  2：00出发

![1609224162(1)](C:\Users\39470\Desktop\运行截图\1609224162(1).png)

武汉->北京  飞机  5：00出发

![1609224690(1)](C:\Users\39470\Desktop\运行截图\1609224690(1).png)

##### 不限制交通工具

上海->多伦多 11：00出发

![1609224951(1)](C:\Users\39470\Desktop\运行截图\1609224951(1).png)

#### 最省钱路径

##### 指定交通工具

上海->武汉  火车

![image-20201229150024355](C:\Users\39470\AppData\Roaming\Typora\typora-user-images\image-20201229150024355.png)

武汉->多伦多 飞机

![image-20201229150130764](C:\Users\39470\AppData\Roaming\Typora\typora-user-images\image-20201229150130764.png)

##### 不限制交通工具

武汉 ->多伦多

![image-20201229150251424](C:\Users\39470\AppData\Roaming\Typora\typora-user-images\image-20201229150251424.png)

### 删除城市+班次

![1609225547(1)](C:\Users\39470\Desktop\运行截图\1609225547(1).png)

首先删除 北京->上海的一个飞机班次，打印北京时刻表

然后删除上海节点 再打印北京时刻表 观察到所有上海相关班次已删除

![1609225673(1)](C:\Users\39470\Desktop\运行截图\1609225673(1).png)



![1609225743(1)](C:\Users\39470\Desktop\运行截图\1609225743(1).png)

### 问题

1. 在menu选项时尝试输入字母等不符合要求的字符会导致死循环。尝试用char/string保存选项，判断是否满足条件；满足时才转化为int类型，进入switch，否则要求重新输入；
2. 第一次测试计算决策函数时，会输出错误“Nonexistent City”.检查后发现是忘记在调用递归函数前运行  refresh_city（）；
3. 发现输出路径时只满足可以到达，但不一定是代价最小路径；原因是每次递归返回前将相应城市的TAG置0，但在满足条件的最后一次递归返回时并没有这一语句，导致每次只输出第一次成功尝试的路径；
4. 删除城市节点应将所有其他城市节点中 关联班次信息全部删除。但发现当存在多个相同出发地、终点站的班次时，只会删除一个班次；原因是delete_city（）中检查城市进行删除操作时 某if语句后没有加{ }，导致直接退出循环.

### 改进/其他设计

##### 1

本来考虑到在计算最优决策，每个班次有时间上的限制，且两个城市间可能会有多个班次，所以选择便于实现的深度优先搜索；

完成后发现难以用dijkstra算法实现。首先由于两个城市间有多个班次，难以确定边的权值；若选择当前最小代价确定权值，后续的的班次也有相应的时间限制：其出发时间要晚于当前班次的抵达时间，所以并不能确定这一班次满足条件。

##### 2

可以实现两天或以上的行程规划.  

首先修改函数 void time_gap(clock a,clock b):

```c++
void time_gap(clock a, clock b)
{
	if(a>b) return 24*60-(a.clk2num()-b.clk2num);
    else return b.clk2num() - a.clk2num();
}
```

然后在计算决策时取消 后一班次出发时间晚于当前班次到达时间的限制即可。

可能造成的结果是两个班次相差时间太长，不合常理；但在班次样例足够多的情况下，最小代价路径一般不会出现这种情况。

实现原理较为简单，但要修改的函数较多，所以不在提交代码中体现。

##### 3

在提交代码中实现了不限制交通工具计算最小代价决策的功能,

体现在函数 superfast( )  /  supercheap( )  中.

 
#### 戴旭书 19307130122  
#### 2020/12/28 22：01：41
