package test_project;

simple Server
{
    double x @unit(m) = default(300m); // the x coordinate of the server
    double y @unit(m) = default(300m); // the y coordinate of the server
    @signal[PacketLossRate](type="double");
    @statistic[LossRate](source="PacketLossRate";title="PacketLossRate";record=vector);
    gates:
        input in @directIn;
}
