float calc(float a, float b)
{
    // Print the value that we received
    Print(">Received: " + a + ", " + b + "\n");
    
    // Print the current system time
    Print("System has been running for " + GetSystemTime()/1000.0 + " seconds\n");
    
    // Test cairomm-angelscript.lib
    int na = -2;
    int nb = 0;
    int nc = 0;
    
    nc = AddFour(nb);    
    Print("AddFour(): input=" + nb + ", output=" + nc + "\n");
    
    
    nb = TestDummy::AddThree(na);
    Print("TestDummy::AddThree(): input=" + na + ", output=" + nb + "\n");
    
    
    // Do the calculation and return the value to the application
    return a * b;
}
