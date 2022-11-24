package test_project;

simple Node
{
    double x @unit(m)= uniform(0m,600m); // the x coordinate of the server
    double y @unit(m)= uniform(0m,600m); // the y coordinate of the server
    @signal[Energy](type="double");
    @statistic[remainEnergy](source="Energy"; title="Energy";record=vector);
    @display("i=misc/node2");
    gates:
        input in @directIn;
}
