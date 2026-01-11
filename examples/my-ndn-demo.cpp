#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ndnSIM-module.h"

namespace ns3 {

int
main(int argc, char* argv[])
{
  // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
  CommandLine cmd;
  cmd.Parse(argc, argv);
  // 1️⃣ 创建两个节点
  NodeContainer nodes;
  nodes.Create(2);

  // 2️⃣ 用点到点链路连接
  PointToPointHelper p2p;
  p2p.SetDeviceAttribute("DataRate", StringValue("1Mbps"));
  p2p.SetChannelAttribute("Delay", StringValue("10ms"));
  p2p.Install(nodes.Get(0), nodes.Get(1));

  // 3️⃣ 安装 NDN 栈（关键）
  ndn::StackHelper ndnHelper;
  ndnHelper.SetDefaultRoutes(true);
  ndnHelper.InstallAll();

  // 4️⃣ 安装 Consumer（节点 0）
  ndn::AppHelper consumer("ns3::ndn::ConsumerCbr");
  consumer.SetPrefix("/test");
  consumer.SetAttribute("Frequency", StringValue("1")); // 1 Interest/s
  consumer.Install(nodes.Get(0));

  // 5️⃣ 安装 Producer（节点 1）
  ndn::AppHelper producer("ns3::ndn::Producer");
  producer.SetPrefix("/test");
  producer.SetAttribute("PayloadSize", StringValue("512"));
  producer.Install(nodes.Get(1));

  // 6️⃣ 运行仿真
  Simulator::Stop(Seconds(5.0));
  Simulator::Run();
  Simulator::Destroy();

  return 0;
}

} // namespace ns3

int
main(int argc, char* argv[])
{
  return ns3::main(argc, argv);
}