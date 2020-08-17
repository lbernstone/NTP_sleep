This example demonstrates how to keep NTP time synchronized over a number of deep sleep restarts.  On every start, it checks the time (time() is based on the rtc), and if it has been longer than a set period, will connect to WiFi in order to sync.  Otherwise it simply goes back to sleep.