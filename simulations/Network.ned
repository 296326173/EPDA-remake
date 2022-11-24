package test_project.simulations;
import test_project.*;
network Network
{
parameters:
//	单位：
//	n = 10^-9
//	p = 10^-12
//--------------------------------------------------------------------------
//	E_DA 				:  5nJ/bit
//	E_mp 				:  0.0013pJ/bit/m^4  	//信号放大能耗-多径衰减模型
//	E_fs 				:  10pJ/bit/m^2			//信号放大能耗-自由空间模型单位
//	D0   				:  sqrt(E_fs/E_mp)/m   	//距离的初始阈值，d0 = sqrt(E_fs/E_mp)
//	E_elec 				:  50nJ/bit				//射频能耗系数
//	ControlPacketLength :  25B   				//控制报文长度  简写CPL
//	DataMessageLength   :  512B  				//数据报文长度  简写DPL
//  bitRate				:  1MB/s				//比特率,计算包的传输时延用,光速是定值，就不在这里设置
 
     int NodeNumber = default(500);				//节点的数量
//     int NodeDieNumber = default(0);			//死亡节点的数量，改用静态变量记录
     
     double Probability = default(0.3);			//节点初始化的时候，被选为簇头的概率。
     int SlicingNumber = default(3);			//切片的数量
     double InitEnergy = default(0.5);			//节点的初始能量
     double E_DA = default(5e-9);				//数据融合能耗(nJ/bit)
     double E_mp = default(1.3e-15);    		//信号放大能耗-多径衰减模型(pJ/bit/m^4)
     double E_fs = default(1e-11);				//信号放大能耗-自由空间模型单位(pJ/bit/m^2)  10pJ
     double D0 = default(87.705801930702921);	//距离的初始阈值，d0，单位m ,为sqrt(E_fs/E_mp);
     double MaxDistance = default(175.411603861405842);  //节点能广播的最大距离，这里设置为2倍的d0
     double E_elec = default(5e-8);				//射频能耗系数(nJ/bit) 50nJ
     
     int ControlPacketLength = default(20);		//控制报文长度，单位B
     int DataMessageLength = default(512);     	//数据的长度 单位B  DPL
     
     int BitRate = 1<<20;						//1<<10等于 1K,这里选择每秒传输1MB
     
//     int appendNetworkNumber = default(0);		//加入网络中的节点数量，改用静态变量记录
submodules:
    nodes[NodeNumber]: Node {
    parameters:
        @display("i=old/ball,,0;is=s");
    }
    server: Server {
    parameters:
        @display("i=device/antennatower");
    }
}
